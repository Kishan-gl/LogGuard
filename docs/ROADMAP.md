# Roadmap

The log parser and its unit tests are included in the current version. Next, I'll add the analyzer and CLI.

## Next steps

- Add severity and component counts, along with health status rules and C++ tests.
- Build the CLI to read a log file and print the summary.
- Add a sample log file and Python component tests.
- Set up GitHub Actions for building and testing on Linux.
- Finish the README, contributor guide, and design essay.

## Current scope

For the first version, LogGuard will only handle one file at a time using the defined log format. Results will be printed in the terminal.

If a log entry is invalid, processing will stop instead of skipping it.

Live monitoring, dashboards, and support for multiple log formats are out of scope for now.

## Ideas for later

- JSON output.
- Filtering by severity or component.
- Option to skip invalid entries and report how many were skipped.
- Support for other log formats.
- Performance testing with larger log files.
