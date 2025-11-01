#pragma once

#include <memory>
#include <string>

#include "Expression.hpp"

class Program;
class VarState;

class Statement {
public:
  explicit Statement(std::string source);
  virtual ~Statement() = default;

  virtual void execute(VarState &state, Program &program) const = 0;

  const std::string &text() const noexcept;

private:
  std::string source_;
};

class RemStmt : public Statement {
public:
  RemStmt(std::string source, std::string comment);

  void execute(VarState &state, Program &program) const override;

  const std::string &comment() const noexcept;

private:
  std::string comment_;
};

class LetStmt : public Statement {
public:
  LetStmt(std::string source, std::string name, Expression *expression);
  ~LetStmt();
  void execute(VarState &state, Program &program) const override;

private:
  std::string name_;
  Expression *expression_;
};

class PrintStmt : public Statement {
public:
  PrintStmt(std::string source, Expression *expression);
  ~PrintStmt();
  void execute(VarState &state, Program &program) const override;

private:
  Expression *expression_;
};

class InputStmt : public Statement {
public:
  InputStmt(std::string source, std::string name);

  void execute(VarState &state, Program &program) const override;

private:
  std::string name_;
};

class EndStmt : public Statement {
public:
  explicit EndStmt(std::string source);

  void execute(VarState &state, Program &program) const override;
};

class GotoStmt : public Statement {
public:
  GotoStmt(std::string source, int targetLine);

  void execute(VarState &state, Program &program) const override;

  int target() const noexcept;

private:
  int targetLine_;
};

class IfStmt : public Statement {
public:
  IfStmt(std::string source, Expression *left, char op, Expression *right,
         int targetLine);
  ~IfStmt();
  void execute(VarState &state, Program &program) const override;

private:
  Expression *left_;
  Expression *right_;
  char op_;
  int targetLine_;
};
