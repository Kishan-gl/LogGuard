#include "LogAnalyzer.h"
#include "LogParser.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

void print_usage(std::ostream& out) {
    out << "Usage: logguard [--fail-on-error] <logfile>\n"
        << "       logguard --help\n";
}

void print_summary(const Summary& summary) {
    std::cout << "=== LogGuard Report ===\n"
              << "Total entries: " << summary.total << '\n'
              << "INFO:  " << summary.info << '\n'
              << "WARN:  " << summary.warn << '\n'
              << "ERROR: " << summary.error << "\n\n"
              << "Component Summary:\n";

    if (summary.components.empty()) {
        std::cout << "  (no entries)\n";
    } else {
        for (const auto& [name, counts] : summary.components) {
            std::cout << "  " << name << ": "
                      << counts.info << " info, "
                      << counts.warn << " warnings, "
                      << counts.error << " errors\n";
        }
    }
    std::cout << "\nOverall Health: "
              << health_status_name(summary.health_status()) << '\n';
}

}  // namespace

int main(int argc, char* argv[]) {
    if (argc == 2 && std::string(argv[1]) == "--help") {
        print_usage(std::cout);
        return 0;
    }

    bool fail_on_error = false;
    std::string filename;

    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--fail-on-error" && !fail_on_error) {
            fail_on_error = true;
        } else if (!arg.empty() && arg[0] == '-') {
            std::cerr << "Error: unknown or repeated option: " << arg << '\n';
            print_usage(std::cerr);
            return 1;
        } else if (filename.empty()) {
            filename = arg;
        } else {
            std::cerr << "Error: provide exactly one log file\n";
            print_usage(std::cerr);
            return 1;
        }
    }

    if (filename.empty()) {
        std::cerr << "Error: missing log file\n";
        print_usage(std::cerr);
        return 1;
    }

    std::ifstream input(filename);
    if (!input) {
        std::cerr << "Error: cannot open file: " << filename << '\n';
        return 1;
    }

    LogAnalyzer analyzer;
    std::string line;
    std::size_t line_number = 0;

    while (std::getline(input, line)) {
        ++line_number;
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();  // Accept CRLF files too.
        }
        try {
            analyzer.add(parse_log_line(line));
        } catch (const std::invalid_argument& error) {
            std::cerr << "Error: line " << line_number << ": " << error.what() << '\n';
            return 1;
        }
    }

    if (input.bad()) {
        std::cerr << "Error: failed while reading file: " << filename << '\n';
        return 1;
    }

    print_summary(analyzer.summary());
    if (fail_on_error && analyzer.summary().error > 0) {
        return 2;
    }
    return 0;
}
