include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

find_package(Threads)

add_executable(LexerTest 
                LexerTest.cc 
                ../src/Lexer.cc
                ../src/Error.cc)
target_link_libraries(LexerTest gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})


add_executable(AstTest 
                AstTest.cc 
                ../src/Ast/Ast.cc
                ../src/Environment.cc
                ../src/Error.cc)
target_link_libraries(AstTest gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})


add_executable(ParserTest 
                ../src/Lexer.cc
                ../src/Ast/Ast.cc
                ../src/Parser.cc
                ../src/Environment.cc
                ../src/Error.cc
                ParserTest.cc 
                )
target_link_libraries(ParserTest gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

add_executable(EnvironmentTest 
                ../src/Lexer.cc
                ../src/Ast/Ast.cc
                ../src/Parser.cc
                ../src/Environment.cc
                ../src/Error.cc
                EnvironmentTest.cc 
                )
target_link_libraries(EnvironmentTest gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})

add_executable(AstFuncTest 
                AstFuncTest.cc 
                ../src/Error.cc
                ../src/Environment.cc
                ../src/Ast/Ast.cc
                ../src/Ast/AstFunc.cc)
target_link_libraries(AstFuncTest gtest gtest_main ${CMAKE_THREAD_LIBS_INIT})