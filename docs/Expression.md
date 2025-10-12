### Expression 类设计

`Expression` 类用于表示和计算表达式。表达式可以是整数常量、变量或二元运算。我们使用树结构来表示表达式，其中每个节点都是一个 `Expression` 对象。

Expression 类的继承关系如下：

```
Expression (抽象基类)
├── ConstExpression (表示整数常量)
├── VariableExpression (表示变量)
└── CompoundExpression (表示二元运算)
```

#### Expression 类
```cpp
class Expression {
public:
    virtual ~Expression() {}
    virtual int evaluate(const VarState& varState) const = 0; // 计算表达式的值
};
```

#### ConstExpression 类
```cpp
class ConstExpression : public Expression {
private:
    int value; // 整数常量的值
public:
    ConstExpression(int val);
    int evaluate(const VarState& varState);
}
```

#### VariableExpression 类
```cpp
class VariableExpression : public Expression {
private:
    std::string name; // 变量名
public:
    VariableExpression(const std::string& varName);
    int evaluate(const VarState& varState);
}
```

#### CompoundExpression 类
```cpp
class CompoundExpression : public Expression {
private:
    std::unique_ptr<Expression> left; // 左子表达式
    std::unique_ptr<Expression> right; // 右子表达式
    char op; // 运算符（+、-、*、/）
public:
    CompoundExpression(std::unique_ptr<Expression> &&lhs, char oper, std::unique_ptr<Expression> &&rhs);
    int evaluate(const VarState& varState);
}
```