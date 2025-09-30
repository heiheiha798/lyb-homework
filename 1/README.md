# 简易 DPLL 求解器

这是一个简单的 C++ 实现的 **DPLL 算法** SAT（可满足性）问题求解器。它从标准的 **DIMACS CNF 文件**中读取问题实例。

-----

## 编译代码

要编译此项目，请使用提供的 `buidl.sh` 脚本。

```bash
./buidl.sh
```

编译成功后，可执行文件将被放置在 `./build/bin/` 目录下，名称为 `dpll_solver`。

-----

## 运行测试

您可以使用一个标准的 DIMACS CNF 文件作为输入来运行求解器。

**用法:**

```bash
./build/bin/dpll_solver <dimacs_file>
```

**示例:**

```bash
./build/bin/dpll_solver unif-c500-v250-s453695930.cnf

./build/bin/dpll_solver unif-c1200-v600-s351323111.cnf
```

预期得到如下输出
```bash

```

程序将输出问题的可满足性（**SAT** 或 **UNSAT**）以及如果可满足，还会输出一个**赋值**。它同时也会显示运行所需的**耗时**。
