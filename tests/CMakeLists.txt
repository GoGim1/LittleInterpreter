include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

find_package(Threads)

add_executable(Lexer_Test 
                LexerTest.cc 
                ../src/Lexer.cc
                ../src/Error.cc)
target_link_libraries(Lexer_Test gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})


add_executable(Ast_Test 
                AstTest.cc 
                ../src/Ast.cc
                ../src/Environment.cc
                ../src/Error.cc)
target_link_libraries(Ast_Test gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})


add_executable(Parser_Test 
                ../src/Lexer.cc
                ../src/Ast.cc
                ../src/Parser.cc
                ../src/Environment.cc
                ../src/Error.cc
                ParserTest.cc 
                )
target_link_libraries(Parser_Test gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})