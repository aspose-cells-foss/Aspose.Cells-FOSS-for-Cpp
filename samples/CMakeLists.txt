cmake_minimum_required(VERSION 3.16)
project(Aspose.Cells.Foss.Cpp.Samples LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ASPOSE_CELLS_FOSS_CPP_ROOT
    "${CMAKE_CURRENT_LIST_DIR}/../Aspose.Cells.Foss.Cpp"
    CACHE PATH "Path to the Aspose.Cells.Foss.Cpp target project")
set(ASPOSE_CELLS_FOSS_SAMPLES_WORKING_DIR
    "${CMAKE_CURRENT_LIST_DIR}/../..")
set(ASPOSE_CELLS_FOSS_SAMPLES_OUTPUT_DIR
    "output"
    CACHE STRING "Repository-root-relative directory for samples output files")
set(SRC_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")
include_directories("${SRC_ROOT}/include")

add_subdirectory("${ASPOSE_CELLS_FOSS_CPP_ROOT}" "${CMAKE_BINARY_DIR}/aspose_cells_foss")

file(GLOB_RECURSE SOURCE 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
add_executable(${PROJECT_NAME} ${SOURCE})

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        ASPOSE_CELLS_FOSS_SAMPLES_OUTPUT_DIR="${ASPOSE_CELLS_FOSS_SAMPLES_OUTPUT_DIR}")

target_link_libraries(${PROJECT_NAME} PRIVATE aspose_cells_foss)
