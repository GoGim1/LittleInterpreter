include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

find_package(Threads)

add_executable(Lexer_Test 
                LexerTest.cc 
                ../src/Lexer.cc)
target_link_libraries(Lexer_Test gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})