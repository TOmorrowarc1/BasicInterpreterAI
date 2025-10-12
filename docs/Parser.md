## Parser 模块

### 职责概览
Parser 接收 `Lexer` 生成的 `TokenStream`，并将词法单元转换为语句 (`Statement`) 与表达式 (`Expression`) 对象。核心职责包含：
- 区分带行号的程序语句与立即执行命令；
- 解析 `LET`、`PRINT`、`INPUT`、`GOTO`、`IF`、`REM`、`END` 等语句并构造对应对象；
- 构建算术表达式树，支持变量、整数常量与二元运算；
- 将解析结果交还 `Program`，由其存入 `Recorder` 或直接执行。

### 依赖关系
- `Token` 模块中的 `TokenType`、`Token`、`TokenStream`；
- `Statement` 派生类构造函数（例如 `LetStmt`、`IfStmt`）；
- `Expression` 派生类及其构造函数；
- 标准库 `<memory>`、`<optional>`、`<stdexcept>`。

### 公共接口
```cpp
struct ParsedLine {
    std::optional<int> lineNumber;           // 若存在表示程序行，否则为立即执行程序行
    std::unique_ptr<Statement> statement;    // 解析出的语句，nullptr 代表删除
};

class Parser {
public:
    ParsedLine parseLine(TokenStream& tokens) const;            // 解析整行输入
    std::unique_ptr<Statement> parseStatement(TokenStream&) const; // 解析立即命令

private:
    // 语句解析分发
    std::unique_ptr<Statement> parseLet(TokenStream&) const;
    std::unique_ptr<Statement> parsePrint(TokenStream&) const;
    std::unique_ptr<Statement> parseInput(TokenStream&) const;
    std::unique_ptr<Statement> parseGoto(TokenStream&) const;
    std::unique_ptr<Statement> parseIf(TokenStream&) const;
    std::unique_ptr<Statement> parseRem(TokenStream&) const;
    std::unique_ptr<Statement> parseEnd(TokenStream&) const;

    // 表达式解析
    std::unique_ptr<Expression> parseExpression(TokenStream&) const;
    std::unique_ptr<Expression> parseExpression(TokenStream&, int precedence) const;

    int getPrecedence(TokenType op) const;
    int parseLiteral(const std::string& text) const;
};
```
- 当 `lineNumber` 存在且 `statement == nullptr` 时表示“删除该行”，反之代表一个立即执行指令。

### 语句解析流程
1. **行号判定**：首 token 为 `NUMBER` 时解析为行号，记录在 `lineNumber`。若后续无 token，表示删除对应行。
2. **关键字分派**：读取下一个 token 判断语句类型，不匹配支持关键字则抛出 `SyntaxError`。
3. **参数解析**：
   - `LET`：变量名 → `=` → 表达式；
   - `PRINT`：单表达式；
   - `INPUT`：单变量名（是否支持多变量需确认）；
   - `GOTO`：行号；
   - `IF`：表达式 → 比较符 → 表达式 → `THEN` → 行号；
   - `REM`：剩余 token 拼接为注释；
   - `END`：无参数。
4. **终结校验**：解析完成后若仍有剩余 token，抛出 `SyntaxError`。

### 表达式解析
- 采用递归下降 + 优先级爬升（Pratt Parsing）：
  1. 读取左操作数：数字 → `ConstExpression`，标识符 → `VariableExpression`，左括号 → 递归解析；
  2. 查看下一个 token 是否为运算符（`+ - * /`），依据 `getPrecedence` 判断是否展开；
  3. 满足条件则消费运算符，解析右操作数（带更高优先级），生成 `CompoundExpression`；
  4. 重复直到遇到更低优先级运算符或流结束。
- `parseLiteral` 将数字文本转换为 `int`，溢出时抛出异常。

### 错误处理
- Parser 抛出 `SyntaxError`，包含错误信息与位置（依赖 `Token.column`）；
- 常见错误：缺少关键字或符号（如 `=`、`THEN`）、行号/常量格式不合法、IF 比较符无效；
- 错误在上层（`main` 或 `Program`）捕获，输出后终止当前输入行。

### 最小测试建议
- `LET A = 1 + 2 * 3` 解析为正确 AST；
- `IF X < Y THEN 100` 生成 `IfStmt`，目标行号正确；
- `10 REM comment` 返回 `lineNumber=10` 且语句为 `RemStmt`；
- 单独 `10` 表示删除；
- 无效语句（如缺参数的 `GOTO`）抛出 `SyntaxError`；
- 表达式括号嵌套与空白处理。

### 待确认问题
1. 比较符是否允许在表达式中出现，还是仅限 IF？


