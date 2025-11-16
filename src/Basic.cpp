#include <cstdio>
#include <iostream>
#include <string>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Program.hpp"
#include "Token.hpp"
#include "utils/Error.hpp"

void printHelp() {
  std::cout << "支持的命令:\n";
  std::cout << "  RUN - 开始执行程序\n";
  std::cout << "  LIST - 列出当前所有的程序行\n";
  std::cout << "  CLEAR - 清除当前所有的程序行\n";
  std::cout << "  QUIT - 退出解释器\n";
  std::cout << "  HELP - 打印帮助信息\n";
  std::cout << "\n程序指令:\n";
  std::cout << "  [行号] REM <注释> - 注释行\n";
  std::cout << "  [行号] LET <变量> = <表达式> - 赋值语句\n";
  std::cout << "  [行号] PRINT <表达式> - 打印表达式值\n";
  std::cout << "  [行号] INPUT <变量> - 输入变量值\n";
  std::cout << "  [行号] END - 结束程序\n";
  std::cout << "  [行号] GOTO <行号> - 跳转到指定行\n";
  std::cout
      << "  [行号] IF <表达式1> <操作符> <表达式2> THEN <行号> - 条件跳转\n";
  std::cout << "  [行号] INDENT - 进入新的作用域块\n";
  std::cout << "  [行号] DEDENT - 退出当前作用域块\n";
  std::cout << "\n立即执行指令 (不带行号):\n";
  std::cout << "  LET <变量> = <表达式>\n";
  std::cout << "  PRINT <表达式>\n";
  std::cout << "  INPUT <变量>\n";
  std::cout << "  INDENT\n";
  std::cout << "  DEDENT\n";
}

int main() {
  Lexer lexer;
  Parser parser;
  Program program;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }

    try {
      // 词法分析
      TokenStream tokens = lexer.tokenize(line);

      // 检查是否为解释器指令
      if (!tokens.empty()) {
        const Token* firstToken = tokens.peek();
        if (firstToken->type == TokenType::RUN) {
          program.run();
          continue;
        } else if (firstToken->type == TokenType::LIST) {
          program.list();
          continue;
        } else if (firstToken->type == TokenType::CLEAR) {
          program.clear();
          continue;
        } else if (firstToken->type == TokenType::QUIT) {
          break;
        } else if (firstToken->type == TokenType::HELP) {
          printHelp();
          continue;
        }
      }

      // 解析行
      ParsedLine parsedLine = parser.parseLine(tokens, line);

      // 处理解析结果
      if (parsedLine.lineNumber.has_value()) {
        if (parsedLine.statement == nullptr) {
          // 删除行
          program.removeStmt(parsedLine.lineNumber.value());
        } else {
          // 添加或替换行
          program.addStmt(parsedLine.lineNumber.value(),
                          std::move(parsedLine.statement));
        }
      } else if (parsedLine.statement != nullptr) {
        program.execute(std::move(parsedLine.statement));
      }
    } catch (const BasicError& e) {
      std::cout << e.message() << "\n";
    }
  }
  return 0;
}