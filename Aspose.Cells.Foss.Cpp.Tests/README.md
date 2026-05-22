# Aspose.Cells.Foss.Cpp.Tests

GTest-based verification project for the generated C++ port.

Build from the repository root:

```powershell
cmake -S .\workspace\Aspose.Cells.Foss.Cpp.Tests -B .\workspace\Aspose.Cells.Foss.Cpp.Tests\build\debug-fc
cmake --build .\workspace\Aspose.Cells.Foss.Cpp.Tests\build\debug-fc --config Debug -- /p:VcpkgApplocalDeps=false
```

Run:

```powershell
ctest --test-dir .\workspace\Aspose.Cells.Foss.Cpp.Tests\build\debug-fc -C Debug --output-on-failure
```

The test project links the generated `aspose_cells_foss` library by adding
`workspace/Aspose.Cells.Foss.Cpp` as a CMake subdirectory. It uses CMake
FetchContent to download GoogleTest v1.14.0 during configure by default, which
keeps the GoogleTest runtime configuration aligned with the local build. To use
an installed GTest package instead, configure with
`-DASPOSE_CELLS_FOSS_TESTS_USE_SYSTEM_GTEST=ON`.
