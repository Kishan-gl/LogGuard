#pragma once

#include "LogEntry.h"

#include <cstddef>
#include <map>
#include <string>

struct ComponentCounts {
    std::size_t info = 0;
    std::size_t warn = 0;
    std::size_t error = 0;
};

enum class HealthStatus {
    NoData,
    Healthy,
    Warning,
    Unhealthy
};

struct Summary {
    std::size_t total = 0;
    std::size_t info = 0;
    std::size_t warn = 0;
    std::size_t error = 0;
    std::map<std::string, ComponentCounts> components;    //key = component name , value = component counter

    HealthStatus health_status() const;
};

class LogAnalyzer {
public:
    void add(const LogEntry& entry);
    const Summary& summary() const;

private:
    Summary summary_;
};

std::string health_status_name(HealthStatus status);
