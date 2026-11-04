# LogGuard

LogGuard is a small C++17 project for reading and analyzing software logs. The idea is to make it easier to find warnings and errors without having to go through log files manually.

It reads a log file, checks each entry, and prints a summary of messages by severity and component.

## Project overview

The parser, analyzer, and CLI are implemented, along with 15 C++ unit tests and 13 Python component tests.

The repository includes a Linux CI workflow, a [contributor guide](CONTRIBUTING.md),
and a [design essay](docs/DESIGN_ESSAY.md).

## Build and test

Clone the repository with Git and enter the project folder:

```bash
git clone https://github.com/Kishan-gl/LogGuard.git
cd LogGuard
```

The project requires Linux, a C++17 compiler, and CMake 3.16 or newer. The default build also requires Python 3, and running the component tests requires pytest.

On Ubuntu, install the required tools:

```bash
sudo apt update
sudo apt install build-essential cmake python3 python3-pytest
```

From the project folder, build and run the tests:

```bash
cmake -S . -B build
cmake --build build
(cd build && ctest --output-on-failure)
```

CTest runs the parser, analyzer, and Python component tests.

The [GitHub Actions workflow](.github/workflows/ci.yml) is configured to build
and run the same tests on Linux for pushes and pull requests.

To build just the application without Python or the tests:

```bash
cmake -S . -B build-no-tests -DBUILD_TESTING=OFF
cmake --build build-no-tests
```

## Run

From the project folder:

```bash
./build/logguard examples/navigation.log
./build/logguard --help
./build/logguard --fail-on-error examples/navigation.log
```

The sample log has 5 entries: 2 INFO, 1 WARN, and 2 ERROR messages. This gives an overall health status of `UNHEALTHY`.

Other possible statuses are `HEALTHY`, `WARNING`, and `NO_DATA`.

The application uses these exit codes:

- `0` — File processed successfully.
- `1` — Invalid arguments, file errors, or invalid log entries.
- `2` — File processed successfully, but errors were found in the log when `--fail-on-error` is used.

Without `--fail-on-error`, a log containing errors still returns `0`. The health status is based on the messages in the log, not the actual condition of the software.

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

## Project structure

```text
LogGuard/
├── .github/workflows/ # Linux build and tests
├── include/          # Header files
├── src/              # Parser, analyzer, and CLI
├── tests/            # C++ unit tests
│   └── component/    # Python CLI tests
├── examples/         # Sample log file
├── docs/             # Requirements, roadmap, and design essay
├── CMakeLists.txt
├── CONTRIBUTING.md
├── README.md
└── .gitignore
```
