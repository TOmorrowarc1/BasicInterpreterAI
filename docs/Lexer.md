## Lexer 模块

### 职责概览

Lexer 负责把一行 BASIC 源码解析成 Token 序列，服务于 `Parser` 构造语句/表达式以及 `main()` 中分流。核心职责：

- 按字符扫描输入行，识别关键字、标识符、数字、运算符与括号；
- 构建 `TokenStream`，供 `Parser` 与 `main()` 以 `peek` / `get` 的方式逐个消费；
- 遇到非法字符时中止并抛出词法错误。

### 依赖关系

- 对 `Program`、`Statement`、`Expression` 无直接依赖，仅与 `Parser` 交互；
- 仅使用标准库。

### 数据结构与核心接口

```cpp
enum class TokenType {
	LET, PRINT, INPUT, END, REM, GOTO, IF, THEN,
	RUN, LIST, CLEAR, QUIT, HELP,
	IDENTIFIER, NUMBER, 
  PLUS, MINUS, MUL, DIV, GREATTHAN, LESSTHAN, LEFT_PAREN, RIGHT_PAREN,
	COMMA, UNKNOWN // 这两类Token暂时不会出现
};

struct Token {
	TokenType type;
	std::string text; // 原始片段
};

class TokenStream {
public:
	const Token* peek() const;      // 查看当前 token；无则返回 nullptr
	const Token* get();             // 取出并前进；无则返回 nullptr
	bool empty() const;             // 是否耗尽
	void reset();                   // 重置到流起点

private:
	std::vector<Token> tokens;
	int cursor = 0;
};

class Lexer {
public:
	TokenStream tokenize(const std::string& line) const;
};
```

### 接口实现 tokenize()

1. 输入行按字符迭代，跳过空白；
2. 若遇字母，继续读取字母组成单词，区分大小写：
   - 与关键字表匹配，决定是否为关键字；
   - 否则识别为 `IDENTIFIER`；
3. 若遇数字，读取连续数字组成 `NUMBER`，检测是否超过 `int` 范围；
4. 若遇 `+ - * / = < >` 等字符，生成相应运算符；括号 `(` `)` 映射为 `LEFT_PAREN`、`RIGHT_PAREN`；
5. 遇到无法识别字符，抛出 `LexicalError`（自定义异常，包含位置与字符）。

最终构建 `TokenStream` 并返回。

### 错误处理要点

- 提供两类异常：`LexicalError`（未知字符）与 `NumberOverflow`（超出 int 范围），由`tokenize()`抛出；

### 最小测试建议

- 单行关键字、标识符、数字混合的切分；
- REM 行：关键字后整行视为注释；
- 运算符及括号组合，如 `A=(B+3)*2`；
- 错误字符（例如 `@`）触发异常；
- 连续空格及 Tab 的容错行为。
