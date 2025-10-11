## VarState 模块设计

### 模块职责

VarState 用于集中管理解释器在执行期间产生的全部**变量状态**。它提供统一的读写接口，确保不同语句、表达式对变量值的访问保持一致，并负责：

- 追踪所有变量的当前值，变量名大小写敏感但保持原始拼写以便回显。
- 为 `LET`、`INPUT` 等语句提供赋值接口，为表达式求值提供只读访问接口。
- 在 `Program::clear()`、`RUN` 前置初始化等场景下提供一次性清空能力，保证执行环境可重复。
- 为调试、LIST 扩展等功能提供可枚举的变量视图，并支持快照以便实现撤销或测试。


### 数据模型

- **存储结构**：使用 `std::unordered_map<std::string, int>` 维护变量名到整数值的映射：
  ```cpp
  std::unordered_map<std::string, int> var_states;
  ```
- **类型约束**：所有变量为 32 位带符号整型，对应 BASIC 规范。若未来扩展为长整型或浮点型，可通过模板或变体封装。
- **生命周期**：变量在首次赋值 (`LET`/`INPUT`) 时创建；`clear()` 将整体删除；单变量删除目前不开放接口。


### 对外接口

VarState 以轻量级类形式暴露接口，供 `Program`、`Statement`、`Expression` 调用。

| 方法签名 | 语义 | 典型调用方 |
| --- | --- | --- |
| `void setValue(const std::string& name, int value);` | 更改变量。 | `LetStatement`, `InputStatement` |
| `int getValue(const std::string& name) const;` | 查询变量，若不存在则抛出 `RuntimeError`（`Program` 捕获并终止执行）。 | `Expression::evaluate`, `IfStatement` |
| `std::vector<std::pair<std::string, int>> snapshot() const;` | 返回按名称升序的变量快照，用于调试或单元测试，默认按 `rawName` 排序。 | `Program::listVars`（潜在扩展）、测试桩 |

> 说明：`RuntimeError` 类型由 Program 模块统一定义，VarState 只负责构造异常并抛出。


### 交互流程

1. **赋值流程**：
	- Parser 将 `LET`/`INPUT` 生成对应 Statement；
	- Statement 在执行时调用 `VarState::setValue()`；
	- VarState 更新映射。

2. **表达式求值**：
	- `Expression::evaluate` 中读取变量时，通过传入的 `VarState` 调用 `getValue`；
	- 若变量未初始化，VarState 抛出异常，Program 捕获后输出 `VARIABLE NOT DEFINED` 类错误并终止本次 RUN。


### 错误处理

- **未定义变量**：`getValue` 检测不到变量时抛出 `RuntimeError`，错误信息格式建议为 `"VARIABLE <name> NOT DEFINED"`；

### 测试

- 覆盖测试建议：
  - `setValue` + `getValue` 正常流程；
  - 未定义变量访问抛错；
  - 名称大小写混用的兼容性；
