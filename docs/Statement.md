### Statement 类设计

`Statement` 类是项目中用于表示和处理各种语句的核心类。它定义了所有支持的语句类型的基类和派生类，每个派生类对应一种具体的语句类型，封装了该语句的相关数据和行为。

`Statement` 类的继承结构如下：
```
Statement (抽象基类)
├── SequentialStmt (顺序执行语句)
  ├── RemStmt (注释语句)
  ├── LetStmt (赋值语句)
  ├── PrintStmt (打印语句)
  ├── InputStmt (输入语句)
  ├── EndStmt (结束语句)
├──ControlStmt (控制流语句)
  ├── GotoStmt (跳转语句)
  └── IfStmt (条件跳转语句)
```

每个派生类都实现了基类 `Statement` 中的纯虚函数 `execute(VarState &varState, Program &program)`，用于执行该语句的具体逻辑。以下是各个派生类的简要说明：

#### Statement 类
```hpp
class Statement {
private:
  std::string originalLine; // 原始输入行，用于print()
public:
    virtual ~Statement() {}
    virtual void execute(VarState &varState, Program &program) const = 0; // 执行语句
};
``` 

#### RemStmt 类
```hpp
class RemStmt : public Statement {
private:
    std::string comment; // 注释内容
public:
    RemStmt(const std::string& originalLine, const std::string &comm);
    void execute(VarState &varState, Program &program) const override; // 执行注释语句（无操作）
};
```

#### LetStmt 类
```hpp
class LetStmt : public Statement {
private:
    std::string varName; // 变量名
    std::unique_ptr<Expression> expr; // 赋值表达式
public:
    LetStmt(const std::string& originalLine,const std::string &var, std::unique_ptr<Expression> expression);
    void execute(VarState &varState, Program &program) const override; // 执行赋值语句
};
```

#### PrintStmt 类
```hpp
class PrintStmt : public Statement {
private:
    std::unique_ptr<Expression> expr; // 打印表达式
public:
    PrintStmt(const std::string& originalLine,std::unique_ptr<Expression> expression);
    void execute(VarState &varState, Program &program) const override; // 执行打印语句
};
```

#### InputStmt 类
```hpp
class InputStmt : public Statement {
private:
    std::string varName; // 变量名
public:
    InputStmt(const std::string& originalLine,const std::string &var); 
    void execute(VarState &varState, Program &program) const override; // 执行输入语句
};
```

#### EndStmt 类
```hpp
class EndStmt : public Statement {
public:
    EndStmt(const std::string& originalLine);
    void execute(VarState &varState, Program &program) const override; // 执行结束语句
};
```

#### GotoStmt 类
```hpp
class GotoStmt : public Statement {
private:
    int targetLine; // 目标行号
public:
    GotoStmt(const std::string& originalLine,int line);
    void execute(VarState &varState, Program &program) const override; // 执行跳转语句
};
```

#### IfStmt 类
```hpp
class IfStmt : public Statement {
private:
    std::unique_ptr<Expression> left; // 左表达式
    std::unique_ptr<Expression> right; // 右表达式
    std::string op; // 比较操作符（=、<、>）
    int targetLine; // 目标行号
public:
    IfStmt(const std::string& originalLine,std::unique_ptr<Expression> lhs, const std::string &oper, std::unique_ptr<Expression> rhs, int line);
    void execute(VarState &varState, Program &program) const override; // 执行条件跳转语句
};
```