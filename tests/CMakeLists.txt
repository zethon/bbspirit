project(tests)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

set(BBSPIRIT_FILES
    ../include/bbspirit/bbspirit.hpp
)

set(MISC_FILES
    test_misc.cpp
)

add_executable(test_misc
    main.cpp
    ${BBSPIRIT_FILES}
    ${MISC_FILES}
)

target_link_libraries(test_misc
    PUBLIC
        ${CONAN_LIBS}
)

add_test(NAME test_misc
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_misc
)