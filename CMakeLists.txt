cmake_minimum_required(VERSION 3.17)
project(TiledKernel C CXX)

option(USE_CUDA "Support CUDA GPU" OFF)

option(BUILD_TEST "Build test code" ON)
option(BUILD_ASAN "Build code whith ASAN" OFF)

option(DEBUG "Debug mode" OFF)

if(DEBUG)
  message("Debug mode is enabled.")
  add_definitions(-DDEBUG)
  set(CMAKE_BUILD_TYPE Debug)
else()
  message("Debug mode is disabled.")
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 

################################################################################
# ASAN Check
################################################################################
if(BUILD_ASAN)
  set(CMAKE_ASAN_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_ASAN_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_ASAN_FLAGS}")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(include)

include_directories(3rd-party/fmt/include)
add_subdirectory(3rd-party/fmt)
add_definitions(-D FMT_HEADER_ONLY)

include_directories(3rd-party/fmtlog)
add_definitions(-D FMTLOG_HEADER_ONLY)


if(BUILD_TEST)
    include_directories(3rd-party/googletest/googletest/include)
    add_subdirectory(3rd-party/googletest)
endif()

add_compile_options(-Wall)
add_compile_options(-Werror)

enable_testing()


file(GLOB_RECURSE SOURCE 
  src/*.cc
  src/graph/*.cc
)

add_library(tiledkernel SHARED ${SOURCE})

add_compile_options(-Wall)
add_compile_options(-Werror)

target_link_libraries(tiledkernel PRIVATE fmt)

function(build_test files)
  # Non-recursive glob for skip failed tests
  file(GLOB TEST_SOURCES ${files})
  foreach(testsourcefile ${TEST_SOURCES})
    get_filename_component(testname ${testsourcefile} NAME_WE)
    add_executable(${testname} ${testsourcefile})
    target_link_libraries(${testname} tiledkernel GTest::gtest_main)
    add_test(NAME ${testname} COMMAND ${testname})
  endforeach(testsourcefile ${TEST_SOURCES})
endfunction()

if(BUILD_TEST)
    build_test(tests/graph/*.cc)
    # if(USE_CUDA)
    #   build_test(tests/codegen/*.cc)
    # endif()
endif()