#include "LogParser.h"

#include <cctype>
#include <stdexcept>
#include <string>

namespace {

std::string trim(const std::string& value) {
    const auto start = value.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    const auto end = value.find_last_not_of(" \t\r\n");
    return value.substr(start, end - start + 1);
}

bool is_leap_year(int year) {
    return year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
}

bool valid_timestamp(const std::string& value) {          //this checks the structure of the timestamp
    // Expected format: YYYY-MM-DD HH:MM:SS
    if (value.size() != 19 || value[4] != '-' || value[7] != '-' ||
        value[10] != ' ' || value[13] != ':' || value[16] != ':') {
        return false;
    }

    for (std::size_t i = 0; i < value.size(); ++i) {            //this check whether the remaing char are numbers or not
        if (i == 4 || i == 7 || i == 10 || i == 13 || i == 16) {
            continue;
        }
        if (!std::isdigit(static_cast<unsigned char>(value[i]))) {
            return false;
        }
    }

    const int year = std::stoi(value.substr(0, 4));
    const int month = std::stoi(value.substr(5, 2));
    const int day = std::stoi(value.substr(8, 2));
    const int hour = std::stoi(value.substr(11, 2));
    const int minute = std::stoi(value.substr(14, 2));
    const int second = std::stoi(value.substr(17, 2));

    if (year == 0 || month < 1 || month > 12 || hour > 23 ||
        minute > 59 || second > 59) {
        return false;
    }

    const int days_in_month[] = {31, 28, 31, 30, 31, 30,
                                 31, 31, 30, 31, 30, 31};
    int max_day = days_in_month[month - 1];
    if (month == 2 && is_leap_year(year)) {
        max_day = 29;
    }
    return day >= 1 && day <= max_day;
}

Severity parse_severity(const std::string& value) {        //recives a string and returns a serverity enum value
    if (value == "INFO") {
        return Severity::Info;
    }
    if (value == "WARN") {
        return Severity::Warn;
    }
    if (value == "ERROR") {
        return Severity::Error;
    }
    throw std::invalid_argument("severity must be INFO, WARN, or ERROR");
}

}  // namespace

LogEntry parse_log_line(const std::string& line) {
    const auto first = line.find('|');
    const auto second = first == std::string::npos ? first : line.find('|', first + 1);
    const auto third = second == std::string::npos ? second : line.find('|', second + 1);

    if (third == std::string::npos) {
        throw std::invalid_argument("expected timestamp|severity|component|message");
    }

    const std::string timestamp = line.substr(0, first);
    if (!valid_timestamp(timestamp)) {
        throw std::invalid_argument("invalid timestamp (expected YYYY-MM-DD HH:MM:SS)");
    }

    const Severity severity = parse_severity(line.substr(first + 1, second - first - 1));
    const std::string component = trim(line.substr(second + 1, third - second - 1));
    const std::string message = trim(line.substr(third + 1));

    if (component.empty()) {
        throw std::invalid_argument("component must not be empty");
    }
    if (message.empty()) {
        throw std::invalid_argument("message must not be empty");
    }

    return {timestamp, severity, component, message};
}
