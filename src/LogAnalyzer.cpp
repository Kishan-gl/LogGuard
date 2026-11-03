#include "LogAnalyzer.h"

HealthStatus Summary::health_status() const {
    if (total == 0) {
        return HealthStatus::NoData;
    }
    if (error > 0) {
        return HealthStatus::Unhealthy;
    }
    if (warn > 0) {
        return HealthStatus::Warning;
    }
    return HealthStatus::Healthy;
}

void LogAnalyzer::add(const LogEntry& entry) {  //recicves a comletea logentry
    ++summary_.total;                                      //increase the total count by 1
    ComponentCounts& counts = summary_.components[entry.component];

    switch (entry.severity) {
        case Severity::Info:
            ++summary_.info;                   //increase the overall count ( in this case info )
            ++counts.info;                     // increase the coutn for that particular component
            break;
        case Severity::Warn:
            ++summary_.warn;
            ++counts.warn;
            break;
        case Severity::Error:
            ++summary_.error;
            ++counts.error;
            break;
    }
}

const Summary& LogAnalyzer::summary() const {             //return the whole summary the total count and the individual count of each case (info, warn, ...)
    return summary_;
}

std::string health_status_name(HealthStatus status) {
    switch (status) {
        case HealthStatus::NoData:
            return "NO_DATA";
        case HealthStatus::Healthy:
            return "HEALTHY";
        case HealthStatus::Warning:
            return "WARNING";
        case HealthStatus::Unhealthy:
            return "UNHEALTHY";
    }
    return "UNKNOWN";
}
