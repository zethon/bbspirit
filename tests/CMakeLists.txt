project(tests)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

set(BBSPIRIT_FILES
    ../include/BBDocument.h
    ../include/bbspirit.hpp
)

add_executable(test_parse
    main.cpp
    test_parse.cpp
    ${BBSPIRIT_FILES}
)

target_link_libraries(test_parse
    PUBLIC
        ${CONAN_LIBS}
)

add_test(NAME test_parse
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_parse
)

add_executable(test_format
    main.cpp
    test_format.cpp
    ${BBSPIRIT_FILES}
)

target_link_libraries(test_format
    PUBLIC
        ${CONAN_LIBS}
)

add_test(NAME test_format
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_format
)