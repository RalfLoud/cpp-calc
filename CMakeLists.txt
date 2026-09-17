cmake_minimum_required(VERSION 3.28)
project(cpp_calc CXX)

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

set(SOURCES
    main.cpp
    src/application.cpp
    include/operation.hpp
    include/task.hpp
    include/application.hpp
)

add_executable(cpp_calc ${SOURCES})

target_include_directories(cpp_calc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(cpp_calc PRIVATE math_lib)

find_program(CLANG_FORMAT_PATH NAMES clang-format REQUIRED)

add_custom_target(run_clang_format ALL
    COMMAND ${CLANG_FORMAT_PATH} --dry-run --Werror -style=file ${SOURCES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Running clang-format check..."
)

add_dependencies(cpp_calc run_clang_format)

find_program(CLANG_TIDY_PATH NAMES clang-tidy REQUIRED)

set_target_properties(cpp_calc PROPERTIES
    CXX_CLANG_TIDY "${CLANG_TIDY_PATH};-config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy;--warnings-as-errors=*"
)

install(TARGETS cpp_calc
    DESTINATION bin
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/resources/help.txt
    DESTINATION bin
)