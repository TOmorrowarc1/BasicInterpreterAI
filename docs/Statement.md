### Statement 类设计

`Statement` 类是项目中用于表示和处理各种语句的核心类。它定义了所有支持的语句类型的基类和派生类，每个派生类对应一种具体的语句类型，封装了该语句的相关数据和行为。

`Statement` 类的继承结构如下：
```
Statement (抽象基类)
  ├── RemStmt (注释语句)
  ├── LetStmt (赋值语句)
  ├── PrintStmt (打印语句)
  ├── InputStmt (输入语句)
  ├── EndStmt (结束语句)
  ├── GotoStmt (跳转语句)
  └── IfStmt (条件跳转语句)
```

每个派生类都实现了基类 `Statement` 中的纯虚函数 `execute(VarState &varState, Program &program)`，用于执行该语句的具体逻辑。以下是各个派生类的简要说明：

#### Statement 类
```hpp
class Statement {
private:
  std::string source_; // 原始输入行，用于text()
public:
    explicit Statement(std::string source);
    virtual ~Statement() = default;
    virtual void execute(VarState &state, Program &program) const = 0; // 执行语句
    const std::string &text() const noexcept;
};
```

#### RemStmt 类
```hpp
class RemStmt : public Statement {
private:
    std::string comment_; // 注释内容
public:
    RemStmt(std::string source, std::string comment);
    void execute(VarState &state, Program &program) const override; // 执行注释语句（无操作）
    const std::string &comment() const noexcept;
};
```

#### LetStmt 类
```hpp
class LetStmt : public Statement {
private:
    std::string name_; // 变量名
    Expression *expression_; // 赋值表达式
public:
    LetStmt(std::string source, std::string name, Expression *expression);
    ~LetStmt();
    void execute(VarState &state, Program &program) const override; // 执行赋值语句
};
```

#### PrintStmt 类
```hpp
class PrintStmt : public Statement {
private:
    Expression *expression_; // 打印表达式
public:
    PrintStmt(std::string source, Expression *expression);
    ~PrintStmt();
    void execute(VarState &state, Program &program) const override; // 执行打印语句
};
```

#### InputStmt 类
```hpp
class InputStmt : public Statement {
private:
    std::string name_; // 变量名
public:
    InputStmt(std::string source, std::string name);
    void execute(VarState &state, Program &program) const override; // 执行输入语句
};
```

#### EndStmt 类
```hpp
class EndStmt : public Statement {
public:
    explicit EndStmt(std::string source);
    void execute(VarState &state, Program &program) const override; // 执行结束语句
};
```

#### GotoStmt 类
```hpp
class GotoStmt : public Statement {
private:
    int targetLine_; // 目标行号
public:
    GotoStmt(std::string source, int targetLine);
    void execute(VarState &state, Program &program) const override; // 执行跳转语句
    int target() const noexcept;
};
```

#### IfStmt 类
```hpp
class IfStmt : public Statement {
private:
    Expression *left_; // 左表达式
    Expression *right_; // 右表达式
    char op_; // 比较操作符（=、<、>）
    int targetLine_; // 目标行号
public:
    IfStmt(std::string source, Expression *left, char op, Expression *right, int targetLine);
    ~IfStmt();
    void execute(VarState &state, Program &program) const override; // 执行条件跳转语句
};
```