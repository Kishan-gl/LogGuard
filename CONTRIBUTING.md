# Contributing

Keep changes small and easy to understand. Read the [requirements](docs/REQUIREMENTS.md)
before changing the log format or command-line behavior.

## Setup

Use Linux, a C++17 compiler, CMake 3.16 or newer, Python 3, and pytest.
On Ubuntu:

```bash
sudo apt update
sudo apt install git build-essential cmake python3 python3-pytest
git clone https://github.com/Kishan-gl/LogGuard.git
cd LogGuard
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
(cd build && ctest --output-on-failure)
```

Run the example from the project folder:

```bash
./build/logguard examples/navigation.log
```

## Code structure

- `include/LogEntry.h` defines one parsed record and its severity.
- `src/LogParser.cpp` splits and validates a log line.
- `src/LogAnalyzer.cpp` counts messages and determines health status.
- `src/main.cpp` handles arguments, file reading, output, and exit codes.
- `tests/test_parser.cpp` and `tests/test_analyzer.cpp` test the C++ logic.
- `tests/component/test_cli.py` runs the CLI and checks its results.

The parser and analyzer can be tested without launching the CLI.
See the [design essay](docs/DESIGN_ESSAY.md) for the reasons behind this structure.

## Testing changes

Rebuild after editing C++ files, then run all tests:

```bash
cmake --build build
(cd build && ctest --output-on-failure)
```

CTest runs three suites containing 15 C++ tests and 13 Python tests.
To run them separately from the project folder:

```bash
./build/test_parser
./build/test_analyzer
LOGGUARD_BIN="$PWD/build/logguard" python3 -m pytest -q tests/component
```

The C++ tests use small check functions in `tests/test_helpers.h`.
Python tests create temporary log files and run the built application.
The GitHub Actions workflow is configured to build and run all tests on Linux
for pushes and pull requests.

## Before submitting a change

- Keep parsing changes in the parser and counting rules in the analyzer.
- Add tests for behavior you change, including invalid input where relevant.
- Update usage instructions when arguments, output, or exit codes change.
- Use four spaces for indentation and follow the existing naming style.
- Keep generated builds, caches, and editor settings out of commits.
- Explain what changed and which checks you ran in the commit or pull request.
