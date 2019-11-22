cmake_minimum_required(VERSION 3.10)

project(szstools_old VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_DEBUG_FLAGS "${CMAKE_CXX_DEBUG_FLAGS} -0g -Wall")
set(CMAKE_CXX_RELEASE_FLAGS "${CMAKE_CXX_RELEAS_FLAGS} -O3 -march=native")

find_package(GTest)

if(NOT GTEST_FOUND)
    set(BUILD_TESTING OFF)
endif()

# Set up testing framework
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND GTEST_FOUND)
    include(CTest)

    # Prepare the "GTest" library
    include_directories(${GTEST_INCLUDE_DIRS})

    # Add testing directories
    add_subdirectory(szstools_test)

    # Add tests
    add_test(
            NAME
                TestRarcDump
            COMMAND
                dump-test
    )
endif()


find_package(Boost 1.29.0 REQUIRED)

add_library(szstools STATIC
        szstools/szstools.cxx szstools/szstools.hxx
        szstools/RarcDump.cxx szstools/RarcDump.hxx
        szstools/FileEntry.cxx szstools/FileEntry.hxx
        szstools/RarcHeader.hxx szstools/RarcNode.hxx
)

add_executable(yay0dec szstools/yay0dec.cxx szstools/yay0dec.hxx)
target_include_directories(yay0dec SYSTEM PRIVATE ${Boost_INCLUDE_DIRS})