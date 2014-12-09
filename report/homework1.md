Homework1: Branch Predictor
=============
组员：[杨俊睿](www.yangjunrui.com)(1200012860) [刘当一](https://github.com/liudangyi)(1100011625)

# 目标
1. 掌握 gem5 的配置、启动、运行 benchmarks 等操作。
2. 在 gem5 中“自己实现”三种以上不同的转移预测策略（如永远取下一个地址等），并比较对于性能的影响，提交代码和实验报告。

# 算法选择
我们选择了三种算法，分别是 `always_true`，`fake_random` 与 `always_back`。其中：

* `always_true` 在 `lookup` 函数中永远返回`true`。
* `fake_random` 在`lookup` 函数中判断指令地址，如果是第奇数条指令则返回 `true`，如果是第偶数条指令则返回 `false`。
* `always_back` 会判断跳转地址与当前指令地址，如果跳转地址比当前指令地址小则跳转，否则不跳转。为此，需要重写 `predict_inorder`，将当前 `thread_id` 传入 `lookup` 函数，并在 `lookup` 函数中查询 BTB，如果查询到且比当前指令地址小则跳转。

# 工作

## 配置环境
* 修改了部分与OS X 10.10、 Apple LLVM version 6.0 (clang-600.0.54) 不兼容的部分，最终修改结果在{`Darwin MacBook-Pro.local 14.0.0 Darwin Kernel Version 14.0.0: Fri Sep 19 00:26:44 PDT 2014; root:xnu-2782.1.97~2/RELEASE_X86_64 x86_64`, `Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn) Target: x86_64-apple-darwin14.0.0 Thread model: posix`}下测试通过。

## 测试脚本
* 编写编译脚本[make.sh](https://github.com/valkjsaaa/gem5-mod-for-pku-class-04830140/blob/master/make.sh)
* 编写测试脚本[test.sh](https://github.com/valkjsaaa/gem5-mod-for-pku-class-04830140/blob/master/test.sh)（测试时，自动抽取与 Branch Predictor 相关的内容到一个有时间戳的文件中，同样在/m5out/目录下）

## 编写算法
* 永远跳转 always_true 
* 向前跳转 always_back
* 伪随机跳转 fake_random

## 改进
* 修改了 [/src/cpu/pred/bpred_unit.hh](https://github.com/valkjsaaa/gem5-mod-for-pku-class-04830140/blob/master/src/cpu/pred/bpred_unit.hh) ，把该类的 `Private` 改成 `Protected`， 把 `predictInOrder` 改成了虚函数，方便实现更多功能的 Branch Predictor。
* 修正了 [/src/cpu/inorder/resources/execution_unit.cc](https://github.com/valkjsaaa/gem5-mod-for-pku-class-04830140/blob/master/src/cpu/inorder/resources/execution_unit.cc) 中一个输出错误。

# 修改过程
1. 根据 bpred_unit.hh 选择需要改写的函数，添加新类 AlwaysBP，保存成 my_pred_always_true.hh 与 my_pred_always_true.cc 至 src/cpu/pred/ 目录下。
2. 添加 my_pred_always_true.cc 至 src/cpu/pred/SConscript 中。
3. 修改 src/cpu/pred/bpred_unit.cc，添加 AlwaysBP。
4. 修改 src/cpu/pred/BranchPredictor.py，选择 always 方式。
5. 使用 scons build/ALPHA/gem5.opt -j 4 重新编译 gem5.opt。
6. 运行 build/ALPHA/gem5.opt configs/spec/se.py --bench=bzip2 --cpu-type=inorder --caches -I 10000000 进行 benchmark。
7. 查看 m5out/stats.txt 文件，从 system.cpu.branchPred.condIncorrect 得出结论。
8. 选择其他算法，重复1-7。

# 测试方法
1. 修改 src/cpu/pred/BranchPredictor.py 为 always 或 back 或 random。
2. 到根目录执行 make.sh && test.sh。
3. 到 /m5out/ 文件夹查看最新输出。

# 结果分析
使用 bzip2 作为 benchmark，执行 10000000 条指令，最终结果如下

| predType     | lookups | condPredicted | condIncorrect | BTBLookups | BTBHits |
|--------------|---------|---------------|---------------|------------|---------|
| tournament   | 871351  | 870371        | 781751        | 870709     | 767260  |
| 2bit_local   | 871394  | 870465        | 781759        | 869758     | 767336  |
| always_true  | 871494  | 870556        | 781819        | 871279     | 767534  |
| fake_random  | 871045  | 870005        | 781736        | 105603     | 104471  |
| always_back  | 871494  | 870556        | 781819        | 768098     | 767534  |
| always_false | 882234  | 881118        | 781692        | 860        | 330     |  

结论：benchmark并不适用测试branch predictor (benchmark太差...)
