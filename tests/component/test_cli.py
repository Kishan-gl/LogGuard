"""Black-box tests: run the compiled application like an actual CLI user."""

import os
import subprocess
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
BINARY = Path(os.environ.get("LOGGUARD_BIN", ROOT / "build" / "logguard"))


def run_cli(*args):
    return subprocess.run(
        [str(BINARY), *(str(arg) for arg in args)],
        capture_output=True,
        text=True,
        check=False,
    )


def log_file(tmp_path, text):
    path = tmp_path / "test.log"
    path.write_text(text, encoding="utf-8")
    return path


def test_example_file_produces_expected_report():
    result = run_cli(ROOT / "examples" / "navigation.log")
    assert result.returncode == 0
    assert "Total entries: 5" in result.stdout
    assert "INFO:  2" in result.stdout
    assert "WARN:  1" in result.stdout
    assert "ERROR: 2" in result.stdout
    assert "Audio: 1 info, 1 warnings, 0 errors" in result.stdout
    assert "Navigation: 1 info, 0 warnings, 2 errors" in result.stdout
    assert "Overall Health: UNHEALTHY" in result.stdout
    assert result.stderr == ""


def test_component_order_is_stable(tmp_path):
    path = log_file(
        tmp_path,
        "2026-11-02 10:00:00|INFO|Zebra|started\n"
        "2026-11-02 10:00:01|INFO|Audio|started\n",
    )
    result = run_cli(path)
    assert result.returncode == 0
    assert result.stdout.index("  Audio:") < result.stdout.index("  Zebra:")


def test_info_only_is_healthy(tmp_path):
    path = log_file(tmp_path, "2026-11-02 10:00:00|INFO|Audio|started\n")
    result = run_cli(path)
    assert result.returncode == 0
    assert "Overall Health: HEALTHY" in result.stdout


def test_warning_only_has_warning_status(tmp_path):
    path = log_file(tmp_path, "2026-11-02 10:00:00|WARN|Audio|slow\n")
    result = run_cli(path)
    assert result.returncode == 0
    assert "Overall Health: WARNING" in result.stdout


def test_fail_on_error_flag_sets_exit_code():
    result = run_cli("--fail-on-error", ROOT / "examples" / "navigation.log")
    assert result.returncode == 2
    assert "Overall Health: UNHEALTHY" in result.stdout


def test_fail_on_error_allows_warning_only(tmp_path):
    path = log_file(tmp_path, "2026-11-02 10:00:00|WARN|Audio|slow\n")
    result = run_cli(path, "--fail-on-error")
    assert result.returncode == 0


def test_invalid_line_reports_line_number_without_partial_report(tmp_path):
    path = log_file(
        tmp_path,
        "2026-11-02 10:00:00|INFO|Audio|started\n"
        "2026-11-02 10:00:01|DEBUG|Audio|trace\n",
    )
    result = run_cli(path)
    assert result.returncode == 1
    assert "line 2" in result.stderr
    assert "severity" in result.stderr
    assert result.stdout == ""


def test_missing_file_is_a_clear_error(tmp_path):
    result = run_cli(tmp_path / "missing.log")
    assert result.returncode == 1
    assert "cannot open file" in result.stderr
    assert result.stdout == ""


def test_empty_file_returns_no_data(tmp_path):
    result = run_cli(log_file(tmp_path, ""))
    assert result.returncode == 0
    assert "Total entries: 0" in result.stdout
    assert "Overall Health: NO_DATA" in result.stdout


def test_crlf_line_endings_are_supported(tmp_path):
    path = tmp_path / "windows.log"
    path.write_bytes(b"2026-11-02 10:00:00|INFO|Audio|started\r\n")
    result = run_cli(path)
    assert result.returncode == 0
    assert "Total entries: 1" in result.stdout


def test_messages_can_contain_pipe_characters(tmp_path):
    path = log_file(tmp_path, "2026-11-02 10:00:00|INFO|Audio|left|right\n")
    result = run_cli(path)
    assert result.returncode == 0
    assert "Total entries: 1" in result.stdout


def test_help_and_invalid_arguments():
    help_result = run_cli("--help")
    assert help_result.returncode == 0
    assert "Usage: logguard" in help_result.stdout

    no_args = run_cli()
    assert no_args.returncode == 1
    assert "missing log file" in no_args.stderr

    unknown = run_cli("--unknown")
    assert unknown.returncode == 1
    assert "unknown or repeated option" in unknown.stderr

    duplicate = run_cli("--fail-on-error", "--fail-on-error", "something.log")
    assert duplicate.returncode == 1


def test_extra_file_is_rejected(tmp_path):
    path = log_file(tmp_path, "")
    result = run_cli(path, path)
    assert result.returncode == 1
    assert "exactly one log file" in result.stderr
