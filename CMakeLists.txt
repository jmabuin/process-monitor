cmake_minimum_required(VERSION 3.16)

# this has to be set before project()
#if(NOT DEFINED CMAKE_CUDA_ARCHITECTURES)
#    set(CMAKE_CUDA_ARCHITECTURES 86)
#endif()

#project(process_monitor LANGUAGES CXX CUDA)
project(process-monitor)

set(CMAKE_CXX_STANDARD 17)

set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(THREADS_PREFER_PTHREAD_FLAG ON)

message(PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")
message(CMAKE_MODULE_PATH="${CMAKE_MODULE_PATH}")

include_directories(./include)

find_package(Threads REQUIRED)
#find_package(CUDA REQUIRED)

set(PAPI_STATIC_LIBRARY ${CMAKE_SOURCE_DIR}/third-party/libs/libpapi.a)

#message(PAPI_PREFIX_DIRS="${PAPI_PREFIX_DIRS}")
#message(PAPI_LIBRARIES="${PAPI_LIBRARIES}")
#message(PAPI_STATIC_LIBRARIES="${PAPI_STATIC_LIBRARIES}")
#message(PAPI_INCLUDE_DIRS="${PAPI_INCLUDE_DIRS}")
message(PAPI_STATIC_LIBRARY="${PAPI_STATIC_LIBRARY}")

include_directories(${PAPI_INCLUDE_DIRS})

#find_package(Procps REQUIRED)

set(PROCPS_STATIC_LIBRARY ${CMAKE_SOURCE_DIR}/third-party/libs/libproc2.a)

#message(PROCPS_PREFIX_DIRS="${PROCPS_PREFIX_DIRS}")
#message(PROCPS_LIBRARIES="${PROCPS_LIBRARIES}")
#message(PROCPS_STATIC_LIBRARIES="${PROCPS_STATIC_LIBRARIES}")
#message(PROCPS_INCLUDE_DIRS="${PROCPS_INCLUDE_DIRS}")
message(PROCPS_STATIC_LIBRARY="${PROCPS_STATIC_LIBRARY}")

include_directories(${PROCPS_INCLUDE_DIRS})

add_executable(process-monitor src/main.cpp
        include/Config.h
        include/Args.h
        include/Globals.h
        include/third-party/nlohmann/json.h
        include/third-party/procps/misc.h
        include/third-party/procps/readproc.h
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
        include/StringUtils.h)

target_link_libraries(process-monitor ${PAPI_STATIC_LIBRARY})
target_link_libraries(process-monitor ${PROCPS_STATIC_LIBRARY})
target_link_libraries(process-monitor Threads::Threads)
#target_link_libraries(process_monitor ${CUDA_LIBRARIES})