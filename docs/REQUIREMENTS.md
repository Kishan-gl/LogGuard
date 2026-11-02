# Requirements

LogGuard is being developed to read a log file and provide a simple summary of warnings, errors, and other messages.

The goal is to keep the tool small and easy to use. These are the requirements planned for the first version. Only the log parser and its C++ unit tests are included in this version so far.

## Functional requirements

Once completed, LogGuard should be able to:

- Accept a log file path through the command line and provide a `--help` option.
- Read and validate each log entry.
- Count the total number of `INFO`, `WARN`, and `ERROR` messages.
- Show message counts for each component in alphabetical order.
- Display an overall health status based on the messages:
  - `NO_DATA` when the file is empty.
  - `UNHEALTHY` when errors are found.
  - `WARNING` when warnings are found but no errors.
  - `HEALTHY` when only informational messages are present.
- Show clear error messages for invalid arguments, missing or unreadable files, and invalid log entries.
- Include the line number when an invalid entry is found, without displaying an incomplete report.
- Return exit code `0` when processing succeeds and `1` for input or usage errors.
- With `--fail-on-error`, return exit code `2` after successful processing if the log contains errors.

The health status is based only on the log messages and does not necessarily represent the actual condition of the software.

## Log format

Each log entry should follow this format:

```text
YYYY-MM-DD HH:MM:SS|LEVEL|COMPONENT|MESSAGE
```

Example:

```text
2026-11-02 10:30:03|ERROR|Navigation|GPS unavailable
```

The input must follow these rules:

- Dates and times must be valid, including leap years and 24-hour time.
- Supported levels are `INFO`, `WARN`, and `ERROR`, written in uppercase.
- Component names and messages cannot be empty.
- Extra spaces at the beginning and end of component names and messages are removed.
- Additional `|` characters are allowed inside the message.
- Both LF and CRLF line endings are supported.
- Empty files are allowed, but blank lines inside a file are considered invalid.

## Non-functional requirements

- The project should build and run on Linux using C++17 and CMake 3.16 or newer.
- The application should use the C++ standard library without additional runtime dependencies.
- Log files should be processed one line at a time instead of loading the entire file into memory.
- Only the necessary counters should be stored while processing.
- The same input should always produce the same output.
- Important logic should be covered by C++ unit tests, while the command-line application should be tested using Python.
- Instructions for building, running, testing, and contributing to the project should be documented.

## Completion goals

The first version will be considered complete when the application can read the sample log file, generate the expected summary, and pass all C++ and Python tests.

A new user should also be able to build and run the project by following the README.

Features such as live monitoring, graphical dashboards, automatic log format detection, and processing multiple files are not planned for this version.

Future improvements are listed in the [Roadmap](ROADMAP.md).
