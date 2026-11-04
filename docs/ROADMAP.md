# Roadmap

The first version includes the parser, analyzer, CLI, sample log, and C++ and Python tests. A Linux CI workflow, contributor guide, and design essay are also included.

## Current scope

LogGuard currently supports one log file at a time using the defined format. The results are printed in the terminal.

If a log entry is invalid, processing stops instead of skipping it.

Live monitoring, dashboards, and multiple log formats are not supported in this version.

The health status only reflects message severity. A filename starting with a
dash needs a path prefix such as `./`; the CLI does not support `--` to end options.
Performance on large files has not been benchmarked, and dependency versions
are not fully pinned.

## Ideas for later

- JSON output.
- Filtering by severity or component.
- Option to skip invalid entries and show how many were skipped.
- Support for other log formats.
- Performance testing with larger log files.
- More boundary-case tests and a larger C++ test framework if the suite grows.
