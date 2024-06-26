cmake_minimum_required(VERSION 3.16)

# this has to be set before project()
#if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
#    set(CMAKE_CUDA_ARCHITECTURES 86)
#endif()

#project(process_monitor LANGUAGES CXX CUDA)
project(process-monitor)
set(CMAKE_CXX_STANDARD 17)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

enable_testing()
include(GoogleTest)


set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(THREADS_PREFER_PTHREAD_FLAG ON)

message(PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
message(CMAKE_MODULE_PATH="${CMAKE_MODULE_PATH}")

include_directories(./include)
include_directories(./third-party/pfs/include)

find_package(Threads REQUIRED)
#find_package(CUDA REQUIRED)

set(PAPI_STATIC_LIBRARY ${CMAKE_SOURCE_DIR}/third-party/libs/libpapi.a)

message(PAPI_STATIC_LIBRARY="${PAPI_STATIC_LIBRARY}")

include_directories(${PAPI_INCLUDE_DIRS})

set(PFS_STATIC_LIBRARY ${CMAKE_SOURCE_DIR}/third-party/libs/libpfs.a)

message(PFS_STATIC_LIBRARY="${PFS_STATIC_LIBRARY}")

add_library(libprocess-monitor include/Config.h
        include/Args.h
        include/Globals.h
        include/third-party/nlohmann/json.h
        src/Config.cpp
        src/ProcessInfo.cpp
        include/ProcessInfo.h
        include/CpuMeasure.h
        include/MemoryMeasure.h
        include/PapiMapEvents.h
        src/PapiInfo.cpp
        include/PapiInfo.h
        include/PapiMeasure.h
        src/EnergyInfo.cpp
        include/EnergyInfo.h
        include/EnergyMeasure.h
        src/ProcessUtils.cpp
        include/ProcessUtils.h
        src/StringUtils.cpp
        include/StringUtils.h
        include/IoMeasure.h
        src/Runner.cpp
        include/Runner.h
        include/ISourceInfo.h)

target_link_libraries(libprocess-monitor ${PAPI_STATIC_LIBRARY})
target_link_libraries(libprocess-monitor ${PFS_STATIC_LIBRARY})
target_link_libraries(libprocess-monitor Threads::Threads)

add_executable(process-monitor src/main.cpp)

target_link_libraries(process-monitor libprocess-monitor)

# Generic tests
file(GLOB test_files "tests/*.cpp")

add_executable(GlobalTests ${test_files})

target_link_libraries(GlobalTests gtest gtest_main gmock gmock_main)
target_link_libraries(GlobalTests libprocess-monitor)

gtest_add_tests(TARGET GlobalTests
        TEST_SUFFIX .GlobalTests
        TEST_LIST   noArgsTests
)