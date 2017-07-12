cmake_minimum_required(VERSION 2.8)
project(gen_dao)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "please into build dir to build!")
endif()

if(NOT ACL_ROOT)
    message(FATAL_ERROR "libraft depend on acl.need define ACL_ROOT to build"
            "eg: cmake .. -DACL_ROOT=/home/akzi/acl-dev/acl")
endif()

set(depend_libs
        ${ACL_ROOT}/lib_acl_cpp/lib/lib_acl_cpp.a
        ${ACL_ROOT}/lib_acl/lib/lib_acl.a
        pthread
        z
        dl)

include_directories(include
        ${ACL_ROOT}/lib_acl/include
        ${ACL_ROOT}/lib_acl_cpp/include)

set(src ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(sources ${src})

foreach(iter ${sources})
	aux_source_directory(${iter} SOURCES)
endforeach()

add_executable(cowboy ${SOURCES})
target_link_libraries(cowboy ${depend_libs})
