# C++ Review Policy

This file describes project-specific rules for the AI C++ reviewer.

The reviewer must treat this document as project policy, not as a
replacement for evidence from the actual source code.

---

## Ownership

Ownership must be explicit.

A raw pointer is considered non-owning unless the surrounding code
clearly establishes otherwise.

Prefer RAII for owned resources.

Do not introduce ownership relationships that are difficult to reason
about.

---

## Lifetime

Resource lifetime must be explicit.

This is particularly important for:

- GPU resources
- command buffers
- descriptor resources
- images
- buffers
- memory allocations
- asynchronous work

The reviewer should pay particular attention to lifetime relationships
crossing frame boundaries or asynchronous execution.

---

## Error Handling

Important failures must not be silently ignored.

Vulkan return values should be checked where appropriate.

Do not recommend exceptions merely because an error exists; consider
the engine's established error-handling strategy.

---

## Dependencies

Lower-level systems should not unnecessarily depend on higher-level
gameplay systems.

Renderer code should not depend directly on gameplay code unless the
dependency is intentional and documented.

Avoid circular subsystem dependencies.

---

## Interfaces

Public interfaces should have a clear responsibility.

Avoid exposing implementation details unnecessarily.

Avoid making Vulkan implementation details leak through unrelated
engine subsystems without a clear architectural reason.

---

## Performance

Performance findings should be based on plausible execution behavior.

Do not report a theoretical micro-optimization as a significant issue
without evidence that the code is likely to be performance-sensitive.

Pay particular attention to:

- allocations in frame loops
- unnecessary copies
- synchronization
- repeated resource lookup
- unnecessary virtual dispatch in hot paths
- accidental CPU/GPU synchronization

---

## Concurrency

Pay particular attention to:

- shared mutable state
- lifetime across worker threads
- command recording
- resource creation/destruction
- queues
- asynchronous GPU work

Do not claim a data race without identifying the conflicting accesses.

---

## Review Philosophy

Prefer:

1. correctness
2. lifetime and ownership
3. concurrency
4. architectural integrity
5. meaningful performance concerns
6. maintainability

Avoid noisy style commentary.

It is acceptable to return:

"No significant issues found."

The reviewer should prefer missing a speculative minor issue over
reporting a large number of unsupported warnings.
