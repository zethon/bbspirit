project(tests)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

set(BBSPIRIT_FILES
    ../include/BBDocument.h
    ../include/bbspirit.hpp
)

set(MISC_FILES
    test_parse.cpp
)

add_executable(test_parse
    main.cpp
    ${BBSPIRIT_FILES}
    ${MISC_FILES}
)

target_link_libraries(test_parse
    PUBLIC
        ${CONAN_LIBS}
)

add_test(NAME test_parse
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_parse
)