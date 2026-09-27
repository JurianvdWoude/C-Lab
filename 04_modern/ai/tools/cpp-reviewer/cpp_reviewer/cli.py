from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

from .reviewer import CppReviewer


def build_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description=(
            "Read-only AI C++ change reviewer."
        )
    )

    parser.add_argument(
        "--repo",
        type=Path,
        default=Path.cwd(),
        help=(
            "Path to the Git repository. "
            "Defaults to the current directory."
        ),
    )

    parser.add_argument(
        "--json",
        action="store_true",
        help="Print raw structured JSON.",
    )

    return parser


def print_human(result: dict) -> None:
    print()
    print("C++ REVIEW")
    print("==========")
    print()
    print(result["summary"])
    print()

    findings = result["findings"]

    if not findings:
        print("No findings.")
        return

    print(f"Findings: {len(findings)}")
    print()

    for index, finding in enumerate(
        findings,
        start=1,
    ):
        print(
            f"[{index}] "
            f"{finding['severity']} / "
            f"{finding['confidence']}"
        )

        print(
            f"{finding['title']}"
        )

        print(
            f"{finding['file']}:"
            f"{finding['line']}"
        )

        print(
            f"Category: "
            f"{finding['category']}"
        )

        print()
        print(
            f"Evidence:\n"
            f"{finding['evidence']}"
        )

        print()
        print(
            f"Explanation:\n"
            f"{finding['explanation']}"
        )

        print()
        print(
            f"Recommendation:\n"
            f"{finding['recommendation']}"
        )

        print()
        print("-" * 72)
        print()


def main() -> int:
    parser = build_parser()
    args = parser.parse_args()

    try:
        reviewer = CppReviewer(
            args.repo
        )

        result = reviewer.review()

        data = {
            "summary": result.summary,
            "findings": result.findings,
        }

        if args.json:
            print(
                json.dumps(
                    data,
                    indent=2,
                )
            )
        else:
            print_human(data)

        # Non-zero exit only for HIGH findings.
        high_count = sum(
            1
            for finding in result.findings
            if finding["severity"] == "HIGH"
        )

        return 1 if high_count else 0

    except KeyboardInterrupt:
        print(
            "\nReview cancelled.",
            file=sys.stderr,
        )
        return 130

    except Exception as exc:
        print(
            f"Review failed: {exc}",
            file=sys.stderr,
        )
        return 2


if __name__ == "__main__":
    raise SystemExit(
        main()
    )
