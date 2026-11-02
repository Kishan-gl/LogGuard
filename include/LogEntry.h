#pragma once

#include <string>

enum class Severity {
    Info,
    Warn,
    Error
};

struct LogEntry {
    std::string timestamp;
    Severity severity;
    std::string component;
    std::string message;
};
