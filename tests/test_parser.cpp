#include "LogParser.h"
#include "test_helpers.h"

#include <stdexcept>
#include <string>

//total 9 test cases

namespace {

void expect_invalid(const std::string& line, const std::string& expected_reason) {
    try {
        parse_log_line(line);
    } catch (const std::invalid_argument& error) {
        check(std::string(error.what()).find(expected_reason) != std::string::npos,
              "wrong error message for: " + line);
        return;
    }
    throw std::runtime_error("expected invalid log line: " + line);
}

void parses_normal_entry() {
    const LogEntry entry = parse_log_line(
        "2026-11-02 10:30:03|ERROR|Navigation|GPS unavailable");
    check(entry.timestamp == "2026-11-02 10:30:03", "timestamp mismatch");
    check(entry.severity == Severity::Error, "severity mismatch");
    check(entry.component == "Navigation", "component mismatch");
    check(entry.message == "GPS unavailable", "message mismatch");
}

void parses_each_severity() {
    const std::string prefix = "2026-11-02 10:30:03|";
    check(parse_log_line(prefix + "INFO|Audio|started").severity == Severity::Info,
          "INFO was not parsed");
    check(parse_log_line(prefix + "WARN|Audio|slow").severity == Severity::Warn,
          "WARN was not parsed");
    check(parse_log_line(prefix + "ERROR|Audio|failed").severity == Severity::Error,
          "ERROR was not parsed");
}

void trims_fields_and_preserves_message_pipes() {
    const LogEntry entry = parse_log_line(
        "2026-11-02 10:30:03|INFO|  Audio  |  left|right  ");
    check(entry.component == "Audio", "component whitespace not trimmed");
    check(entry.message == "left|right", "message pipe or whitespace lost");
}

void rejects_missing_fields() {
    expect_invalid("", "expected timestamp");
    expect_invalid("2026-11-02 10:30:03|INFO|Audio", "expected timestamp");
    expect_invalid("no delimiters", "expected timestamp");
}

void rejects_invalid_severity() {
    expect_invalid("2026-11-02 10:30:03|DEBUG|Audio|started", "severity");
    expect_invalid("2026-11-02 10:30:03|info|Audio|started", "severity");
}

void rejects_empty_component_or_message() {
    expect_invalid("2026-11-02 10:30:03|INFO|   |started", "component");
    expect_invalid("2026-11-02 10:30:03|WARN|Audio| \t ", "message");
}

void rejects_invalid_timestamp_shape() {
    expect_invalid("2026/11/02 10:30:03|INFO|Audio|started", "timestamp");
    expect_invalid("2026-11-02 10:30|INFO|Audio|started", "timestamp");
    expect_invalid("2026-11-02 10:XX:03|INFO|Audio|started", "timestamp");
}

void checks_calendar_dates() {
    const std::string suffix = "|INFO|Audio|started";
    parse_log_line("2024-02-29 10:00:00" + suffix);
    expect_invalid("2025-02-29 10:00:00" + suffix, "timestamp");
    expect_invalid("2026-04-31 10:00:00" + suffix, "timestamp");
    expect_invalid("0000-01-01 10:00:00" + suffix, "timestamp");
    expect_invalid("2026-00-01 10:00:00" + suffix, "timestamp");
    expect_invalid("2026-13-01 10:00:00" + suffix, "timestamp");
}

void checks_clock_time() {
    const std::string suffix = "|INFO|Audio|started";
    expect_invalid("2026-11-02 24:00:00" + suffix, "timestamp");
    expect_invalid("2026-11-02 10:60:00" + suffix, "timestamp");
    expect_invalid("2026-11-02 10:30:60" + suffix, "timestamp");
}

}  // namespace

int main() {
    return run_tests({
        {"normal entry", parses_normal_entry},                 //test name , function to execute
        {"severities", parses_each_severity},
        {"whitespace and pipe in message", trims_fields_and_preserves_message_pipes},
        {"missing fields", rejects_missing_fields},
        {"invalid severity", rejects_invalid_severity},
        {"empty fields", rejects_empty_component_or_message},
        {"timestamp format", rejects_invalid_timestamp_shape},
        {"calendar date", checks_calendar_dates},
        {"clock time", checks_clock_time},
    });
}
