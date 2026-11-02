# LogGuard

LogGuard is a small C++17 project for reading and analyzing software logs. The idea is to make it easier to find warnings and errors without having to go through log files manually.

This version starts with the log parser. The analyzer and command-line tool will be added next.

## Current progress

The log parser is implemented, along with 9 C++ unit tests.

For now, it can parse individual log lines, separate their fields, and check whether the input is valid. The analyzer and CLI are not included in this version yet.

## Build and test

The project is developed for Linux and requires a C++17 compiler and CMake 3.16 or newer.

On Ubuntu, install the required tools:

```bash
sudo apt install build-essential cmake
```

To build the project and run the tests:

```bash
cmake -S . -B build
cmake --build build
cd build
ctest --output-on-failure
```

At this stage, only the parser and its unit tests are built. Python is not required yet.

## Log format

Each log entry follows this format:

```text
TIMESTAMP|LEVEL|COMPONENT|MESSAGE
```

For example:

```text
2026-11-02 10:30:03|ERROR|Navigation|GPS unavailable
```

The supported levels are `INFO`, `WARN`, and `ERROR`. The parser checks the timestamp and required fields before accepting an entry.

More details are available in [Requirements](docs/REQUIREMENTS.md). Planned improvements are tracked in the [Roadmap](docs/ROADMAP.md).
