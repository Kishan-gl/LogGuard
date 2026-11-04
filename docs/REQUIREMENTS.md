# Requirements

LogGuard is a small tool for reading software logs and showing a summary of warnings, errors, and other messages.

The goal is to provide a simple command-line tool that validates log entries and reports message counts and overall health. The scope is limited to one local log file at a time.

## Functional requirements

LogGuard should be able to:

- Accept a log file path through the command line and provide a `--help` option.
- Read and validate each log entry.
- Count the total number of `INFO`, `WARN`, and `ERROR` messages.
- Show message counts for each component in alphabetical order.
- Display an overall health status:
  - `NO_DATA` when the file is empty.
  - `UNHEALTHY` when errors are found.
  - `WARNING` when warnings are found but no errors.
  - `HEALTHY` when only informational messages are present.
- Show clear error messages for invalid arguments, missing or unreadable files, and invalid log entries.
- Include the line number for invalid entries without printing an incomplete report.
- Return exit code `0` for successful processing and `1` for input or usage errors.
- Return exit code `2` after successful processing if `--fail-on-error` is enabled and the log contains errors.

The health status is based on the log messages, not necessarily the actual condition of the software.

## Log format

Each log entry follows this format:

```text
YYYY-MM-DD HH:MM:SS|LEVEL|COMPONENT|MESSAGE
```

Example:

```text
2026-11-02 10:30:03|ERROR|Navigation|GPS unavailable
```

The input must follow these rules:

- Dates and times must be valid, including leap years and 24-hour time.
- Levels must be `INFO`, `WARN`, or `ERROR`, written in uppercase.
- Component names and messages cannot be empty.
- Leading and trailing spaces in component names and messages are removed.
- Additional `|` characters are allowed inside the message.
- Both LF and CRLF line endings are supported.
- Empty files are allowed, but blank lines are considered invalid.

## Non-functional requirements

- Build and run on Linux using C++17 and CMake 3.16 or newer.
- Use the C++ standard library without extra runtime dependencies.
- Process log files one line at a time instead of loading the entire file into memory.
- Keep component names and counters for the report instead of storing every log entry.
- Produce the same output for the same input.
- Test the main logic with C++ unit tests and the CLI with Python component tests.
- Provide instructions for building, running, testing, and contributing to the project.

## Completion goals

The first version is considered complete when LogGuard can process the sample log file, print the expected report, and pass all C++ and Python tests.

A new user should also be able to build and run it by following the README.

Live monitoring, dashboards, automatic log format detection, and processing multiple files are outside the scope of this version.

Future improvements are listed in the [Roadmap](ROADMAP.md).
