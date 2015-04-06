4.5 

1.完成了define-syntax的过程，但是这个宏不卫生
2.加了init.scm 初始化解释器时把cond,let,let星，and,or等改成了宏定义，修正了之前and,or会把先把所有参数值算出来的错误。

3.解释器还没有实现尾递归，尝试理解一下什么是continuation-passing-style




4.4 

通过复制整个寄存器的方式实现了call/cc，这里面副作用会比较混乱。

实现了基本过程display,load和newline

接下来打算实现宏，这样就可以完成对cond,letrec,>=,<=，也就不需要再编译时进行翻译了。

宏变量的表示
(literals . trans-list)
这样一个binding中就是
(macro-name literals . trans);

语法
(define-syntax let
 (syntax-rules ()
  ( (let ((x e) ...) b1 b2 ...)
	((lambda (x ...) b1 b2 ...) e ...))))

匹配(let ((var1 1) (va2 2) (va3 3)) exp1 exp2 exp3)

有match:
 <((var 1) (var2 2) (var3 3)),((x e) ...)>
 < exp1,b1 >
 < (exp2 exp3),(b2 ...) >
 转换
(x ...)时，搜索match中x的位置,从对应位置得到
  (pattern2 template2)))
(var1 var2 var3)

语法(x1 x2 x3 ...)
匹配(t1 t2 t3 t4)
有match:
< t1,x1 >
< t2,x2 >
<(t3 t4),(x3 ...)>
转换
(x3 ...)时，搜索match中x3的位置，从对应位置得到
(t3 t4)


4.3

((  (call/cc (lambda (k) k))
    	(lambda (x) x)) "HEY!")


(let ( (x (call/cc (lambda (k) k))))
   (x (lambda (ignore) "hi")))

call/cc是一个函数，该函数以另一个lambda构造的复合过程为参数，这个复合过程有一个参数（我们假设为exit,它也是单参的函数），返回值为我们需要求值的语句（设为exp,该语句中可能存在exit）。
当调用call/cc时，exp中若遇到调用exit,则立即返回exit的参数，若求解exp的过程中没有调用exit,则正常返回exp的值。
其实这个exit就是continuation,代表call/cc的括号外面的下一步是做什么。

感觉做不下去了，先做出错处理，包括（变量是否被绑定，参数类型检查和参数数量的检查）

完成了出错提示，开始看R5S5，可能需要挺多时间。

整理了一下，还剩下下面5点没有处理

1.letrec如何实现
2.call/cc and CPS
3.尾递归的优化
4.define-syntax


4.2

1.处理递归调用 无法递归的原因：函数中没有包含自身的bind,可以同时思考处理互相递归的情况。

ldf是把函数定义时的环境加入闭包中，所以该环境中并没有对自身或者之后定义的函数的绑定，递归调用无法实现。

改进：其实是搜索环境中的绑定时，没有向上一级的环境搜索，注意该级环境被AP指令存在了dump中。已经可以实现互相递归，且不破坏闭包性质

2.实现了let和let星，想不出来letrec实现的方法。
3.发现了词法分析器的一个问题，强制保证括号匹配才行

4.1
重写了GC，之前的算法有问题，不能解决循环引用，现在按照sicp上的方法完成了GC，解决了一个坑爹的BUG，查了我整整半天，就是制造新的空表对象时，一定要初始化一下它的car指针为0，不然可能会发生恰好空表car的值和brokenheart的地址相同的情况，这时会导致空表不被回收。

注意GC运行时一定要保证所有的寄存器都在ROOT节点中。

计划：
1.实现eval,然后处理一下优化尾递归，即dum和rap两个指令如何运用。
2.重新看书，尝试理解continuation，call/cc，cps
3.实现宏
4.实现错误处理

最后把EVAL实现了，明天早上去润泽书局把递归搞出来,加油！

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

