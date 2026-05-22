cmake_minimum_required(VERSION 3.16)
project(Aspose.Cells.Foss.Cpp.Tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ASPOSE_CELLS_FOSS_CPP_ROOT
    "${CMAKE_CURRENT_LIST_DIR}/../Aspose.Cells.Foss.Cpp"
    CACHE PATH "Path to the Aspose.Cells.Foss.Cpp target project")
set(ASPOSE_CELLS_FOSS_TEST_WORKING_DIR
    "${CMAKE_CURRENT_LIST_DIR}/../..")
set(ASPOSE_CELLS_FOSS_TEST_OUTPUT_DIR
    "workspace/temp"
    CACHE STRING "Repository-root-relative directory for test output files")

enable_testing()

add_subdirectory("${ASPOSE_CELLS_FOSS_CPP_ROOT}" "${CMAKE_BINARY_DIR}/aspose_cells_foss")

option(ASPOSE_CELLS_FOSS_TESTS_USE_SYSTEM_GTEST
    "Use an installed GTest package instead of FetchContent" OFF)

if(ASPOSE_CELLS_FOSS_TESTS_USE_SYSTEM_GTEST)
    find_package(GTest CONFIG QUIET)
endif()

if(NOT TARGET GTest::gtest_main)
    include(FetchContent)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    if(MSVC)
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    endif()
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE)
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(aspose_cells_foss_cpp_tests
    tests/WorkbookPortingTests.cpp
    tests/Support/TestSupport.cpp
    tests/Support/PackageTestSupport.cpp
    tests/Support/ScenarioFactories.cpp
    tests/Unit/CellDataUnitTests.cpp
    tests/Unit/WorksheetFeatureUnitTests.cpp
    tests/Unit/WorkbookMetadataUnitTests.cpp
    tests/Golden/CellDataGoldenTests.cpp
    tests/Golden/OpenXmlFeatureGoldenTests.cpp
    tests/Golden/WorkbookMetadataGoldenTests.cpp
    tests/Compatibility/CompatibilityTests.cpp)

target_compile_definitions(aspose_cells_foss_cpp_tests
    PRIVATE
        ASPOSE_CELLS_FOSS_TEST_OUTPUT_DIR="${ASPOSE_CELLS_FOSS_TEST_OUTPUT_DIR}")

target_include_directories(aspose_cells_foss_cpp_tests
    PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/tests")

target_link_libraries(aspose_cells_foss_cpp_tests
    PRIVATE
        aspose_cells_foss
        GTest::gtest_main)

set_target_properties(aspose_cells_foss_cpp_tests PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${ASPOSE_CELLS_FOSS_TEST_WORKING_DIR}")

include(GoogleTest)
gtest_discover_tests(aspose_cells_foss_cpp_tests
    WORKING_DIRECTORY "${ASPOSE_CELLS_FOSS_TEST_WORKING_DIR}")
