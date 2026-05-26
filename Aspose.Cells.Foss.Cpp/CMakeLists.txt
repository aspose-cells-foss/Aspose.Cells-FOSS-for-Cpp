cmake_minimum_required(VERSION 3.16)
project(aspose_cells_foss_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/toolchain-detect.cmake)

option(PORTER_SANITIZE "Enable sanitizer (address/undefined)" "")
option(ASPOSE_CELLS_FOSS_BUILD_SHARED "Build Aspose.Cells.FOSS as a shared library" OFF)

if(PORTER_SANITIZE STREQUAL "address")
    if(MSVC)
        add_compile_options(/fsanitize=address)
    else()
        add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address)
    endif()
elseif(PORTER_SANITIZE STREQUAL "undefined")
    if(MSVC)
        message(FATAL_ERROR "UndefinedBehaviorSanitizer is not supported by this MSVC target template")
    else()
        add_compile_options(-fsanitize=undefined -fno-omit-frame-pointer)
        add_link_options(-fsanitize=undefined)
    endif()
endif()

file(GLOB_RECURSE ASPOSE_CELLS_FOSS_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
)
set(aspose_cells_foss_library_type STATIC)
if(ASPOSE_CELLS_FOSS_BUILD_SHARED)
    set(aspose_cells_foss_library_type SHARED)
endif()

add_library(aspose_cells_foss ${aspose_cells_foss_library_type} ${ASPOSE_CELLS_FOSS_SOURCES})
target_include_directories(aspose_cells_foss PUBLIC include)

if(WIN32 AND ASPOSE_CELLS_FOSS_BUILD_SHARED)
    set_target_properties(aspose_cells_foss PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

enable_testing()
add_subdirectory(tests)