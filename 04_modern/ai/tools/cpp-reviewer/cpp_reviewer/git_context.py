from __future__ import annotations

import subprocess
from dataclasses import dataclass
from pathlib import Path


CPP_EXTENSIONS = {
    ".c",
    ".cc",
    ".cpp",
    ".cxx",
    ".h",
    ".hh",
    ".hpp",
    ".hxx",
    ".ipp",
    ".inl",
}


@dataclass
class RepositoryContext:
    repository_root: Path
    status: str
    diff: str
    source_context: str
    project_policy: str


class GitContextError(RuntimeError):
    pass


class GitContextCollector:
    """
    Collects the minimum repository context required for a C++ review.

    This class is read-only.
    """

    def __init__(
        self,
        root: Path,
        max_context_chars: int = 180_000,
    ) -> None:
        self.root = root.resolve()
        self.max_context_chars = max_context_chars

    def collect(self) -> RepositoryContext:
        self._ensure_git_repository()

        status = self._git(
            ["status", "--short"]
        )

        diff = self._git(
            [
                "diff",
                "--no-ext-diff",
                "--unified=40",
                "HEAD",
                "--",
            ]
        )

        changed_files = self._changed_cpp_files(status, diff)

        source_context = self._collect_source_context(
            changed_files
        )

        policy = self._read_policy()

        return RepositoryContext(
            repository_root=self.root,
            status=status,
            diff=diff,
            source_context=source_context,
            project_policy=policy,
        )

    def _ensure_git_repository(self) -> None:
        result = subprocess.run(
            ["git", "rev-parse", "--show-toplevel"],
            cwd=self.root,
            capture_output=True,
            text=True,
        )

        if result.returncode != 0:
            raise GitContextError(
                "The current directory is not inside a Git repository."
            )

    def _git(self, arguments: list[str]) -> str:
        result = subprocess.run(
            ["git", *arguments],
            cwd=self.root,
            capture_output=True,
            text=True,
        )

        if result.returncode != 0:
            raise GitContextError(
                result.stderr.strip()
                or f"Git command failed: {' '.join(arguments)}"
            )

        return result.stdout

    def _changed_cpp_files(
        self,
        status: str,
        diff: str,
    ) -> list[Path]:
        paths: set[Path] = set()

        for line in status.splitlines():
            if len(line) < 4:
                continue

            relative = line[3:].strip()

            # Handle rename syntax conservatively.
            if " -> " in relative:
                relative = relative.split(" -> ")[-1]

            path = Path(relative)

            if path.suffix.lower() in CPP_EXTENSIONS:
                paths.add(path)

        for line in diff.splitlines():
            if line.startswith("+++ b/"):
                path = Path(line[6:])

                if path.suffix.lower() in CPP_EXTENSIONS:
                    paths.add(path)

        return sorted(paths)

    def _collect_source_context(
        self,
        changed_files: list[Path],
    ) -> str:
        files: list[Path] = []

        for relative in changed_files:
            path = self.root / relative

            if path.exists() and path.is_file():
                files.append(path)

            files.extend(
                self._related_files(path)
            )

        unique_files = []
        seen: set[Path] = set()

        for path in files:
            path = path.resolve()

            if path in seen:
                continue

            seen.add(path)
            unique_files.append(path)

        chunks: list[str] = []
        total = 0

        for path in unique_files:
            try:
                content = path.read_text(
                    encoding="utf-8",
                    errors="replace",
                )
            except OSError:
                continue

            chunk = (
                f"\n===== SOURCE: "
                f"{path.relative_to(self.root)} =====\n"
                f"{content}\n"
            )

            if total + len(chunk) > self.max_context_chars:
                break

            chunks.append(chunk)
            total += len(chunk)

        return "".join(chunks)

    def _related_files(self, path: Path) -> list[Path]:
        """
        Collect obvious local dependencies without attempting full
        C++ dependency analysis.

        AST/indexing can replace this later if it becomes necessary.
        """

        result: list[Path] = []

        if not path.exists():
            return result

        try:
            content = path.read_text(
                encoding="utf-8",
                errors="replace",
            )
        except OSError:
            return result

        for line in content.splitlines():
            line = line.strip()

            if not line.startswith("#include"):
                continue

            if '"' not in line:
                continue

            include = line.split('"', 2)[1]

            candidate = (
                path.parent / include
            ).resolve()

            if (
                candidate.exists()
                and self.root in candidate.parents
                and candidate.suffix.lower() in CPP_EXTENSIONS
            ):
                result.append(candidate)

        # Also look for the conventional matching header/source pair.
        stem = path.stem

        for candidate in path.parent.glob(
            f"{stem}.*"
        ):
            if (
                candidate.suffix.lower()
                in CPP_EXTENSIONS
            ):
                result.append(candidate)

        return result

    def _read_policy(self) -> str:
        path = self.root / ".ai" / "cpp-review.md"

        if not path.exists():
            return (
                "No project-specific C++ review policy exists yet."
            )

        return path.read_text(
            encoding="utf-8",
            errors="replace",
        )
