cmake_minimum_required(VERSION 3.28)
project(cpp_calc)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
        math-lib
        GIT_REPOSITORY https://github.com/RalfLoud/math-lib.git
        GIT_TAG main
)

FetchContent_MakeAvailable(math-lib)

add_executable(cpp_calc
        main.cpp
        src/application.cpp
        include/operation.hpp
        include/task.hpp
        include/application.hpp
)

target_link_libraries(cpp_calc PRIVATE math_lib)

target_include_directories(cpp_calc PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/help.txt
        ${CMAKE_CURRENT_BINARY_DIR}/help.txt
        COPYONLY
)

install(TARGETS cpp_calc
        DESTINATION bin
)

install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/help.txt
        DESTINATION bin
)

option(USE_CLANG_FORMAT_CHECK "Enable clang-format check" OFF)
option(USE_CLANG_FORMAT_FIX "Enable clang-format fix" OFF)
option(USE_CLANG_TIDY "Enable clang-tidy" OFF)

if(USE_CLANG_FORMAT_CHECK OR USE_CLANG_FORMAT_FIX)
    find_program(CLANG_FORMAT_PATH NAMES clang-format REQUIRED)

    message(STATUS "Found clang-format: ${CLANG_FORMAT_PATH}")

    file(GLOB_RECURSE CLANG_FORMAT_SRC CONFIGURE_DEPENDS
            ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
            ${CMAKE_CURRENT_SOURCE_DIR}/*.h
    )
endif()

if(USE_CLANG_FORMAT_FIX)
    add_custom_target(clang-format-fix
            COMMAND ${CLANG_FORMAT_PATH} -style=file -i ${CLANG_FORMAT_SRC}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Formatting cpp_calc with clang-format"
    )
endif()

if(USE_CLANG_FORMAT_CHECK)
    add_custom_target(clang-format
            COMMAND ${CLANG_FORMAT_PATH} --dry-run --Werror -style=file ${CLANG_FORMAT_SRC}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            COMMENT "Checking cpp_calc formatting with clang-format"
    )
endif()

if(USE_CLANG_TIDY)
    find_program(CLANG_TIDY_PATH NAMES clang-tidy REQUIRED)

    message(STATUS "Found clang-tidy: ${CLANG_TIDY_PATH}")

    set(CLANG_TIDY_COMMAND
            ${CLANG_TIDY_PATH}
            "-config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy"
    )

    set_target_properties(cpp_calc PROPERTIES
            CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}"
    )
endif()