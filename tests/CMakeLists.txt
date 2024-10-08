enable_testing()

find_package(Catch2 3 QUIET)

if(NOT Catch2_FOUND)

  Include(FetchContent)

  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.4.0 # or a later release
  )

  FetchContent_MakeAvailable(Catch2)
endif()

add_executable(parser_test parser_test.cpp)
target_link_libraries(parser_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(parser_test PRIVATE cDBParser)

add_executable(tokenizer_test tokenizer_test.cpp)
target_link_libraries(tokenizer_test PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tokenizer_test PRIVATE cDBParser)

add_test(NAME parser_test COMMAND parser_test)
add_test(NAME tokenizer_test COMMAND tokenizer_test)
