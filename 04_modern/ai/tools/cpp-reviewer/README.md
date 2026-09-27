# C++ Change Reviewer

A deliberately small, read-only AI reviewer for a C++ game engine.

## What it does

The reviewer:

1. Reads the current Git state.
2. Reads the Git diff against `HEAD`.
3. Finds changed C++ files.
4. Reads those files and obvious related headers/sources.
5. Reads `.ai/cpp-review.md`.
6. Sends the evidence to the configured model.
7. Produces structured review findings.

## What it does NOT do

The reviewer cannot:

- modify C++ source,
- modify headers,
- modify shaders,
- modify CMake,
- modify documentation,
- create commits,
- create branches,
- execute arbitrary shell commands,
- manage Scrum tasks.

It is intentionally read-only.

## Requirements

- Python 3.11+
- Git
- An OpenAI API key

## Installation

From:

    tools/cpp-reviewer/

run:

    python -m pip install -e .

Then configure:

    export OPENAI_API_KEY="your-key"

Optionally select a model:

    export OPENAI_MODEL="gpt-5.6-sol"

## Run

From the root of the engine:

    cpp-review

Or:

    python -m cpp_reviewer.cli

JSON output:

    cpp-review --json

Review another repository:

    cpp-review --repo /path/to/engine

## Typical workflow

Make a change:

    src/renderer/Texture.cpp
    src/renderer/Texture.hpp

Then run:

    cpp-review

The reviewer examines the diff and reports potential issues.

The developer decides what to do.

## Important

The reviewer is advisory.

A finding is not automatically a bug.

A clean review does not prove the code is correct.

The compiler, tests, static analysis, sanitizers, Vulkan validation
layers, and human engineering judgment remain authoritative sources of
evidence.

## Future extensions

Do not add these until the basic reviewer has been evaluated on real
engine changes:

- C++ AST indexing
- clang-tidy integration
- compiler diagnostics
- static analysis
- Vulkan specification retrieval
- historical review memory
- CI integration

The first goal is simply:

"Does this reviewer consistently find useful problems in my actual
changes without producing annoying false positives?"
