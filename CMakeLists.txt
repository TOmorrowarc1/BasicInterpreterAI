cmake_minimum_required(VERSION 3.10)
project(BasicInterpreter)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 包含目录
include_directories(include)

# 源文件
set(SOURCES
    Basic.cpp
    src/Expression.cpp
    src/Lexer.cpp
    src/Parser.cpp
    src/Program.cpp
    src/Recorder.cpp
    src/Statement.cpp
    src/Token.cpp
    src/VarState.cpp
    src/utils/Error.cpp
)

# 创建可执行文件
add_executable(basic ${SOURCES})

# 创建测试程序
add_executable(test_basic test_basic.cpp)
target_link_libraries(test_basic)

# 创建附着式测试程序
add_executable(attached_test attached_test.cpp)
target_link_libraries(attached_test)