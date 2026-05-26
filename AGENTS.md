# AGENTS.md

This repository contains Aspose.Cells FOSS for C++.

Purpose:
Provide a lightweight, MIT-licensed, dependency-minimal C++ library for reading, writing, and modifying Excel `.xlsx` workbooks.

This file defines operational instructions for AI coding agents.

---

# Repository Goals

Agents working in this repository should optimize for:

1. Correctness
2. API compatibility
3. Cross-platform support
4. Minimal dependencies
5. Deterministic builds
6. Readability
7. Backward compatibility

Do not optimize for:
- unnecessary abstractions
- speculative extensibility
- introducing heavy third-party libraries
- rewriting stable code

---

# Technology Assumptions

Language:
- C++17

Build:
- CMake

Primary Compilers:
- MSVC
- GCC
- Clang

Target Platforms:
- Windows
- Linux
- macOS

Testing:
- GTest

Package Style:
- Header + source
- No runtime Office dependency

---

# Project Architecture

Expected modules:

```text
src/
    aspose/cells_foss/
    aspose/cells_foss/core/
    aspose/cells_foss/xml/

include/
    aspose/cells_foss/

```

Rules:

- Public API only exposed under:
  include/aspose/cells_foss/

- Internal helpers stay under:
  src/

- Tests must never depend on private implementation.

- Avoid cyclic includes.

---

# Public API Rules

Preserve API stability.

When adding public API:

- prefer value semantics
- use const correctness
- use std::string_view for inputs
- avoid raw ownership
- prefer std::unique_ptr
- document exceptions

Do NOT:

- expose STL containers unnecessarily
- leak implementation details
- expose internal XML structures

Public naming:

```cpp
Workbook
Worksheet
Cells
Cell
Style
SaveOptions
LoadOptions
```

Methods:

```cpp
GetSettings()
GetProperties()
Add()
PutValue()
Save()
```

---

# Memory Management

Allowed:

```cpp
std::unique_ptr
std::shared_ptr
std::optional
```

Avoid:

```cpp
new
delete
malloc
free
```

No ownership ambiguity.

RAII preferred everywhere.

---

# Error Handling

Use:

```cpp
throw CellsException(...)
```

Do not:

- swallow exceptions
- return magic values
- use assertions for runtime failures

---

# Performance Expectations

Optimize:

- workbook loading
- worksheet traversal
- cell access
- save throughput

Avoid:

- premature caching
- unnecessary copies
- quadratic scans

Prefer:

```cpp
reserve()
emplace_back()
move semantics
```

Benchmark before major optimization.

---

# XLSX Compatibility

Assume XLSX compatibility is critical.

Agents must preserve:

- workbook structure
- sheet order
- cell references
- formulas
- styles
- hyperlinks
- validation rules

Do not silently drop unsupported content.

Unsupported features should:

- remain intact when possible
- emit warnings

---

# Documentation Rules

Update documentation when:

- public APIs change
- build steps change
- formats change

Keep docs:

- concise
- executable
- version independent

---

# Pull Request Rules

Agents should produce:

1. Summary
2. Design decisions
3. Test evidence
4. Compatibility notes

Example:

```text
Summary
- Added worksheet rename support

Design
- Introduced Worksheet::SetName()

Tests
- Added rename roundtrip tests

Compatibility
- No breaking changes
```

---

# Forbidden Changes

Agents must NOT:

- reformat unrelated files
- rename public APIs
- introduce global state
- add hidden telemetry
- add binary blobs
- disable tests
- modify license headers

---

# Completion Checklist

Before finishing:

[ ] Build succeeds
[ ] Tests pass
[ ] No compiler warnings
[ ] Public API reviewed
[ ] Examples updated
[ ] Documentation updated
[ ] No unnecessary dependencies