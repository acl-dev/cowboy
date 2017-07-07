cmake_minimum_required(VERSION 3.7)
project(dao_generator)

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

set(SOURCE_FILES
        include/dao_generator.h
        samples/demo/create_tables.hpp
        samples/demo/dao.hpp
        samples/demo/helper.hpp
        samples/demo/main.cpp
        samples/demo/mapper.hpp
        samples/demo/model.hpp
        src/dao_generator.cpp)

add_executable(dao_generator ${SOURCE_FILES})
target_link_libraries(dao_generator ${depend_libs})