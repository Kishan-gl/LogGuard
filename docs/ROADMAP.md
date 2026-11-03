# Roadmap

The parser, analyzer, and CLI are now implemented, along with a sample log file and C++ and Python tests.

## Next steps

- Set up GitHub Actions to build and test the project on Linux.
- Update the README and complete the contributor guide and design essay.
- Build and test the project from a fresh clone to make sure everything works.

## Current scope

LogGuard currently supports one log file at a time using the defined format. The results are printed in the terminal.

If a log entry is invalid, processing stops instead of skipping it.

Live monitoring, dashboards, and multiple log formats are not supported in this version.

## Ideas for later

- JSON output.
- Filtering by severity or component.
- Option to skip invalid entries and show how many were skipped.
- Support for other log formats.
- Performance testing with larger log files.
