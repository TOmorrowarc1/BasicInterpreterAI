#pragma once

#include <memory>
#include <optional>

#include "Token.hpp"

class Statement;
class Expression;

struct ParsedLine {
  std::optional<int> lineNumber;
  std::unique_ptr<Statement> statement;
};

class Parser {
 public:
  ParsedLine parseLine(TokenStream& tokens,
                       const std::string& originLine) const;

 private:
  std::unique_ptr<Statement> parseStatement(
      TokenStream& tokens, const std::string& originLine) const;
  std::unique_ptr<Statement> parseLet(TokenStream& tokens,
                                      const std::string& originLine) const;
  std::unique_ptr<Statement> parsePrint(TokenStream& tokens,
                                        const std::string& originLine) const;
  std::unique_ptr<Statement> parseInput(TokenStream& tokens,
                                        const std::string& originLine) const;
  std::unique_ptr<Statement> parseGoto(TokenStream& tokens,
                                       const std::string& originLine) const;
  std::unique_ptr<Statement> parseIf(TokenStream& tokens,
                                     const std::string& originLine) const;
  std::unique_ptr<Statement> parseRem(TokenStream& tokens,
                                      const std::string& originLine) const;
  std::unique_ptr<Statement> parseEnd(TokenStream& tokens,
                                      const std::string& originLine) const;
  std::unique_ptr<Statement> parseIndent(TokenStream& tokens,
                                         const std::string& originLine) const;
  std::unique_ptr<Statement> parseDedent(TokenStream& tokens,
                                         const std::string& originLine) const;

  std::unique_ptr<Expression> parseExpression(TokenStream& tokens) const;
  std::unique_ptr<Expression> parseExpression(TokenStream& tokens,
                                              int precedence) const;

  int getPrecedence(TokenType op) const;
  int parseLiteral(const Token* token) const;

  mutable int leftParentCount{0};
};
