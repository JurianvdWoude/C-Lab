from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
from typing import Any

from .git_context import GitContextCollector
from .llm import LLMReviewer
from .prompts import (
    SYSTEM_PROMPT,
    USER_PROMPT_TEMPLATE,
)


@dataclass
class ReviewResult:
    summary: str
    findings: list[dict[str, Any]]


class CppReviewer:
    """
    Read-only C++ change reviewer.

    This class has no filesystem write functionality.
    """

    def __init__(
        self,
        repository_root: Path,
    ) -> None:
        self.repository_root = (
            repository_root.resolve()
        )

        self.context_collector = (
            GitContextCollector(
                self.repository_root
            )
        )

        self.llm = LLMReviewer()

    def review(self) -> ReviewResult:
        context = self.context_collector.collect()

        if not context.diff.strip():
            return ReviewResult(
                summary=(
                    "No tracked changes were found in the Git diff."
                ),
                findings=[],
            )

        prompt = USER_PROMPT_TEMPLATE.format(
            project_policy=context.project_policy,
            status=context.status,
            diff=context.diff,
            source_context=context.source_context,
        )

        result = self.llm.review(
            system_prompt=SYSTEM_PROMPT,
            user_prompt=prompt,
        )

        return ReviewResult(
            summary=result["summary"],
            findings=result["findings"],
        )
