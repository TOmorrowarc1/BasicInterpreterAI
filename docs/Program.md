## Program 模块

### 职责概览

Program 作为解释器的运行核心，负责：

- 接收 `Parser` 解析出的语句并存入 `Recorder`；
- 提供 `run / list / clear` 三个接口给 `main`；
- 在执行期间维护程序计数器（PC）与变量状态 (`VarState`)；
- 捕获语句执行产生的运行时错误并终止当前 RUN。


### 依赖关系

- `Recorder`：保存非立即执行的语句集合，Program 在 RUN/LIST/CLEAR 时调用。
- `VarState`：保存变量表，RUN 时读写，CLEAR 时重置。
- `Statement`：通过多态接口执行语句。
- `Expression`：通过多态接口求值表达式。

### 数据结构与核心接口

```cpp
class Program {
public:
	void addStmt(int line, Statement stmt); // 委托 Recorder
	void removeStmt(int line);                 // 删除行号

	void run();   // 按行号升序执行，支持 GOTO/IF 改变 PC
	void list();  // 输出 `<line> <stmt>`，数据来自 Recorder
	void clear(); // 清空 Recorder 与 VarState

	void getPC(); // 获取当前行号，RUN 期间有效

	void changePC(int line); // 强制改变 PC，用于 GOTO/IF
private:
	Recorder recorder;
	VarState vars;
	int programCounter; // 当前行号；RUN 前设为最小行

	void execute(const Statement& stmt); // 执行一条语句
	int evaluate(const Expression& expr); // 对一个表达式进行求值
};
```

### RUN() 实现 
1. 若 `Recorder` 为空，直接返回。
2. 将 PC 设为最小行号。
3. 循环：
   - 通过行号获取 `Statement`；若不存在则抛出 `LINE NUMBER ERROR`。
   - 调用 `execute()`；语句的执行可修改 `programCounter`（例如 GOTO 返回新行号）。
   - 若未修改 PC，则移动到下一个行号；若 GOTO/IF 指向不存在行，抛出错误 `LINE NUMBER ERROR`。
4. 遇到 `END` 或执行完最后一行后退出，退出时调用 `vars.clear()` 清空变量；异常不必处理，向上传递到`main()`中统一输出并终止程序。


### LIST() / CLEAR() 实现

- `list()`：遍历 Recorder，输出 `<line>	<stmt.originLine()>`；语句的字符串形式由 `Statement` 派生类提供。
- `clear()`：调用 `recorder.clear()` 与 `vars.clear()`，重置 `programCounter` 为 0。


### 最小测试建议

- RUN 正常顺序执行、带 GOTO/IF 的跳转。
- LIST 输出升序且匹配输入。
- CLEAR 后 RUN 不执行任何内容，LIST 空。
- RUN 时变量状态不会保留下次 RUN。