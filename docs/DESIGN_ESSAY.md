# LogGuard: design choices

## Why this project

I chose a log analyzer because it solves a small debugging problem: getting
an overview of warnings and errors without counting them by hand. It also
has a clear input and output, which makes it practical to test. The first
version reads one local file in a documented format and prints counts by
severity and component.

I wanted to keep the project small enough to understand and explain. A CLI
fits this task because it can be run directly in a Linux terminal or used
by a script. A dashboard or live log collector would introduce more work
without helping the main requirement.

## Separating the responsibilities

The parser, analyzer, and CLI have different jobs. The parser checks one
line and returns a `LogEntry`. The analyzer takes valid entries and updates
the totals. The CLI handles arguments, opens the file, calls those two
parts, and prints the result.

This makes testing simpler. Parser tests can pass strings directly to a
function. Analyzer tests can use entries with known severities without
needing to parse text first. Python tests then run the actual CLI to check
that the parts work together. A parsing failure can be investigated
separately from a counting or file-reading problem.

The code reads one line at a time. After counting an entry, it does not
retain the full message or timestamp. Memory still grows with the number
of different component names and the longest input line, so this is not
a fixed-memory guarantee.

## Counting and handling bad input

The analyzer uses `std::map` to connect each component name to its counters.
It also gives alphabetical output without a separate sorting step. That
makes reports predictable and easier to compare. An `std::unordered_map`
could offer faster average lookups, but it would need extra work to print
the same sorted report. For this small tool, the simpler output handling
was a useful trade-off.

Invalid records stop processing. Silently skipping one could make a report
look complete even though some messages were missing. The CLI reports the
line number and prints no partial summary. This is strict, but the user
can see that the input needs attention. An empty file is allowed and returns
`NO_DATA`, because having no log entries does not necessarily mean the system is healthy.

The health rule is deliberately basic: errors take priority over warnings,
and warnings take priority over info. `UNHEALTHY` means the log contains an
error; it does not prove the software itself is broken. Normal processing
returns `0`. The optional `--fail-on-error` flag returns `2` after successful
processing if an error entry was found. Input or usage failures return `1`.

## Tools and tests

C++17 provides everything the implementation needs, including structured
bindings used when printing component counts. Requiring a newer standard
would not solve a current problem. The application uses the standard
library for strings, files, and containers.

CMake keeps the library, application, and test targets in one build setup.
CTest runs the two C++ test executables and the Python component suite.
The C++ checks are small functions that report a failure by throwing an
exception. They remain active in Release builds, unlike ordinary `assert`
checks when `NDEBUG` is defined. This avoids downloading a C++ test framework,
although a mature framework would offer better facilities for a larger suite.

There are 9 parser tests, 6 analyzer tests, and 13 Python component tests.
The Python tests use pytest and `subprocess` to check output, error messages,
and exit codes. Both levels matter: correct counting alone does not show
that the CLI handles a missing file or invalid arguments properly. The CI
workflow is kept to installing dependencies, building, and running these tests.

## Limitations and what I would improve

The main limitation is the custom log format. Existing application logs may
need conversion first. The tool also handles only one file, has no filters,
and produces only text output. Filenames starting with a dash need a path
prefix such as `./`; the CLI does not support an option-ending `--`.

With more time, I would first measure larger files and extend boundary-case
tests before making performance claims. JSON output, filters, and additional
formats remain possible future work in the roadmap. I would add them only
with a concrete use case and tests. The current simple health rule and strict
input handling are also limitations to explain clearly to users.

## How I worked on it

I used AI assistance for the initial project draft and implementation. I
then ran the project in WSL, studied the code and tests, added explanatory
comments, and reviewed and rewrote the documentation. This helped me connect
the implementation to its design choices. The parts I would keep are the
small scope, separate responsibilities, and tests at both levels.
