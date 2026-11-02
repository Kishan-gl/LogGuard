#pragma once

#include "LogEntry.h"

#include <string>

// Throws std::invalid_argument if the log line does not match the expected format.
LogEntry parse_log_line(const std::string& line);
