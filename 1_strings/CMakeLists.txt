set(BINARY operator_equal)

set(SOURCES
equal_basic_strings.cpp
)

set(BINARY2 cow)
set(SOURCES2 cow.cpp)

set(BINARY3 twine)
set(SOURCES3 string_twine.cpp)

add_executable(${BINARY} ${SOURCES})
add_executable(${BINARY2} ${SOURCES2})
add_executable(${BINARY3} ${SOURCES3})

target_link_libraries(${BINARY2} PUBLIC gtest)
target_link_libraries(${BINARY3} PUBLIC gtest)