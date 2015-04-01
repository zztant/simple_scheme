4.1
重写了GC，之前的算法有问题，不能解决循环引用，现在按照sicp上的方法完成了GC，解决了一个坑爹的BUG，查了我整整半天，就是制造新的空表对象时，一定要初始化一下它的car指针为0，不然可能会发生恰好空表car的值和brokenheart的地址相同的情况，这时会导致空表不被回收。

注意GC运行时一定要保证所有的寄存器都在ROOT节点中。

计划：
1.实现eval,然后处理一下递归程序，即dum和rap两个指令如何运用。
2.重新看书，尝试理解continuation，call/cc，cps
3.实现宏
4.实现错误处理

3.31

执行指令过程完成，将符号表与垃圾回收分离。
发现GC存在BUG，找了半天找不到，蛋疼。

3.30
计划：  为了后续工作可以顺利进行，先把词法分析器完善，确保可以返回所有可用的scheme对象。
        可以先实现GC，再处理VM。

稍微完善了一下词法分析器，GC编码并调试完成，接下来为虚拟机的指令编码。
完成了指令集的编码
完成了将AST编译到虚拟机指令的过程（部分）

明天开始做虚拟机执行指令的过程

3.29
实现了
lambda,and,or,if,以及一些谓词的过程。
可以对复合过程进行求解，可以实现递归程序了。

接下来思考怎么实现GC以及如何设计基于栈的虚拟机以及相应指令格式，
由于没有基础，下一步开始之前可能要花一点时间。

看了一些资料，初步想法是：
1.虚拟机使用SECD machine。(stack,env,code,dump)。
2.对于GC，使用复制收集算法，将申请的内存分为mem-car和mem-cdr两块。
3.把寄存器和调用栈(也就是SECD machine的四个寄存器)作为垃圾回收的root引用。
4.接下来深入思考一下如何实现SECD machine。

指令类型：
nil 空指针压栈
ldc 常量实参压栈
ld  变量压栈(pair保存在env中的位置)
sel 分支指令，两个指令列表，一个布尔值
join dump中的指令弹出

3.28
把昨天的推倒了重做，已经实现基本加减乘除运算的表达式求值
symbol-table和env已经构造。
实现了define,quote,set这4个基本过程
明天完成对lambda表达式和复合运算的求值。

