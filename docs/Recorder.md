## Recorder 模块

### 职责概览

Recorder 负责维护解释器中“带行号的程序”，负责：
- 用行号索引存储语句对象，保证插入、覆盖与删除符合 BASIC 语义；
- 为 `Program` 提供按行号升序遍历能力，支撑 `RUN` 与 `LIST`；
- 提供清空接口，在 `CLEAR` 时清空状态。


### 依赖关系

- `Program`：唯一的直接调用者，负责驱动增删查遍历；
- `Parser`：通过 `Program::addStmt/removeStmt` 间接写入 Recorder；
- `Statement`：以不透明指针形式存放， Recorder 不关心其内部结构。

### 数据结构与核心接口

```cpp
class Recorder {
public:
	void add(int line, std::unique_ptr<Statement>&& stmt); // 插入或覆盖指定行
	void remove(int line);                 // 删除行，不存在则忽略
	const Statement* get(int line) const;  // 读取语句，不存在返回 nullptr
	bool hasLine(int line) const;          // 行号是否存在
	void clear();                          // 清空全部行
	std::vector<int> listLines() const;    // 返回升序行号，用于 LIST

	int nextLine(int line) const; // 返回大于 line 的最小行号，找不到返回 -1

private:
	std::map<int, std::unique_ptr<Statement>> lines;     // 核心存储：行号 -> 语句指针
};
```


### 操作流程

1. **录入/删除**：`Program::addStmt`/`removeStmt` 直接调用 `add`/`remove`，保持 Recorder 中的数据与最新输入同步。
2. **运行**：`Program::run` 通过 `begin()/end()` 遍历 `lines`，按 PC 决定执行顺序；跳转类语句可通过 `get` 查询目标行是否存在。
3. **列出**：`Program::list` 调用 `listLines()` 获取行号序列，再根据需要格式化输出。
4. **清空**：`Program::clear` 调用 `clear()`，随后变量由 `VarState::clear()` 处理，两者互不干扰。

### 最小测试建议

- 插入乱序行号并验证遍历结果为升序；
- 覆盖已有行后再读取，应返回最新语句；
- 删除行后再 `get` 返回 `nullptr`；
- `clear` 清空所有行，`listLines()` 返回空；
- `hasLine` 在存在/不存在场景下返回正确结果。