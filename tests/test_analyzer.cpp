#include "LogAnalyzer.h"
#include "test_helpers.h"

//total 6 test cases

namespace {

LogEntry make_entry(Severity severity, const std::string& component) {
    return {"2026-11-02 10:00:00", severity, component, "example message"};
}

void empty_summary_is_no_data() {
    LogAnalyzer analyzer;
    const Summary& summary = analyzer.summary();
    check(summary.total == 0, "empty total should be 0");
    check(summary.components.empty(), "empty summary should have no components");
    check(summary.health_status() == HealthStatus::NoData, "empty health status incorrect");
}

void info_only_is_healthy() {
    LogAnalyzer analyzer;
    analyzer.add(make_entry(Severity::Info, "Audio"));
    analyzer.add(make_entry(Severity::Info, "Audio"));
    const Summary& summary = analyzer.summary();
    check(summary.total == 2 && summary.info == 2, "info counts incorrect");
    check(summary.warn == 0 && summary.error == 0, "unexpected warn/error");
    check(summary.health_status() == HealthStatus::Healthy, "health should be healthy");
}

void warnings_change_status() {
    LogAnalyzer analyzer;
    analyzer.add(make_entry(Severity::Info, "Audio"));
    analyzer.add(make_entry(Severity::Warn, "Audio"));
    check(analyzer.summary().health_status() == HealthStatus::Warning,
          "warning status incorrect");
}

void errors_override_warnings() {
    LogAnalyzer analyzer;
    analyzer.add(make_entry(Severity::Warn, "Audio"));
    analyzer.add(make_entry(Severity::Error, "Navigation"));
    check(analyzer.summary().health_status() == HealthStatus::Unhealthy,
          "error should mark report unhealthy");
}

void counts_are_grouped_by_component() {
    LogAnalyzer analyzer;
    analyzer.add(make_entry(Severity::Info, "Audio"));
    analyzer.add(make_entry(Severity::Warn, "Audio"));
    analyzer.add(make_entry(Severity::Error, "Navigation"));
    analyzer.add(make_entry(Severity::Error, "Navigation"));
    const Summary& summary = analyzer.summary();
    check(summary.total == 4 && summary.info == 1 && summary.warn == 1 && summary.error == 2,
          "overall counts incorrect");
    check(summary.components.size() == 2, "expected two components");
    check(summary.components.at("Audio").info == 1, "audio info count incorrect");
    check(summary.components.at("Audio").warn == 1, "audio warning count incorrect");
    check(summary.components.at("Navigation").error == 2, "navigation error count incorrect");
}

void health_names_are_readable() {
    check(health_status_name(HealthStatus::NoData) == "NO_DATA", "no-data label");
    check(health_status_name(HealthStatus::Healthy) == "HEALTHY", "healthy label");
    check(health_status_name(HealthStatus::Warning) == "WARNING", "warning label");
    check(health_status_name(HealthStatus::Unhealthy) == "UNHEALTHY", "unhealthy label");
}

}  // namespace

int main() {
    return run_tests({
        {"empty summary", empty_summary_is_no_data},
        {"info only", info_only_is_healthy},
        {"warnings", warnings_change_status},
        {"errors override warnings", errors_override_warnings},
        {"component counts", counts_are_grouped_by_component},
        {"health labels", health_names_are_readable},
    });
}
