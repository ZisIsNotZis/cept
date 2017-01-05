# Simplified C Compiler/Transpiler

This repo (will) contain 3 transpiler/compiler implementations of my own programming language (not named it). 3 comes from difficulty of directly implementing the compiler or the whole transpiler. In the following text I will give out
* Desires
* Concepts
* Examples with comments
* Differences of three transpilers/compilers
* Their current status
* Others

# Desire

The initial design of my language comes from the strong desires of

* **kill unnecessary symbols**: A typical program will have half of its lines containing round brackets "( )", even more containing ". , : = ; _ ", and a lot containing "[ ] { } < > + - * / ^ & | ! * ". On typical qwerty keyboard, half of them need to press shift (and at least I press shift use left hand and I'm right handed). These things DO make programmers feel tired, and some of them aren't really necessary (or have better ways).

* **make programming more about concept than code**: For the past, compilers were not strong/clever enough to optimize programs or parse abstract things, and all the useful concepts were not there, so people have to explicitly write out what do they want. For now, things have changed. It WOULD BE better if we directly write out concepts rather than advanced assembly. It's more debug-able, more optimize-able, more portable, more readable, and more write-able. I like studying different programming languges. I more or less checked out the top 30 programming language in TIOBE index and find they are more or less detailed. So I believe "conceptual" programming language is a good try.

* **integrate good syntax**. There ARE good programming languages (not C++), but there AREN'T best ones. There are always some programming language better than other in some aspect. Once a programming language officially comes out, it "patches" rather than "changes", and that makes it be complex and not beautiful. I believe we can revert the whole basis of a language, as long as we have an old version available(like python 2/3). 

* **better parallel model**: By this point, I mean though there are a lot of programming languages supporting parallel programming, and some of them are even meant for that (like Go, Rust), they are still "traditional". The program base is still linear. But what if we changes it? What if we make parallel the default? For this part, I'm also thinking about it.

* **more math concept**: I'm personally a Physics and Computer Science double major. By physics, it's actually math everyday. But I do like it since there are a lot of interesting concepts and methods there. I believe it would be nice if we put these concepts (like operator, tensor, matrix) into programming. Operator can be just treated as functions, so it's more like infix way of writing function, which will be convenient in some condition I believe.

#Concept

Combining all these desires, my language have following grammar (compared to C++)

* **Outline syntax**: Like python, "curly brackets" are decided by indentation (tab not space), and "semi columns" are decided by line changing. As a result, code MUST be tab indented and two sentences CANNOT be written on the same line, which makes code looking much better.

* **Inline syntax**: Like some language (I forgot the name). There's no need to write bracket as long as there is no confusion. If there is, compiler will throw errors. To provide arguments to function, just space them after function name like `func a b c func2`(= `func(a,b,c,func2())`in c). If argument also needs arguments, the compiler will not throw error as long as there is only one way to interpret this (after searching through all candidates and type checking). Use square bracket in mylang rather than curly bracket (avoid pressing shift), and use it the math way: `f1 a b [f2 c] d` rather than `f1[a b f2[c] d]`. Array index uses the same grammar, which means you can also choose not to write bracket.

* **Auto/inline declare**: Like python, variable can be used without declaring, in this case the type will be full text deducted (unlike auto in C++ which is initializer deducted). To force a type, use `type;var` wherever the variable is used.

	* **Type**: Type can either be value, set, or symbols. For value, it means copy type. In mylang, there's no such thing as type. Mylang comes with some default type which is actually global variable. They are optimized out as long as they're not used, and they can be used. For explicit range, it specifies what could a variable hold. When debugging, violation will be reported. The format is`from,to`, like `0,inf;a`, to limit a to non-negative integer, or `int+(0,inf)*im:a` to limit a to positive imaginary axes integer. For symbols, the format looks like `emum invalid jan feb mar apr may jun jul aug sep oct nov dec;curMon`. When and only when talking about curMon, these symbols can be used to represent 0~12 (no need for ::). 

	* **Special type**: There's two special type called `auto` and `code` . `auto` is the same as C++, but it CANNOT be `code`. `code`  holds, code, of course. It's like `#define` and `inline` in C but more functional form. Attempt to assign "code" at runtime will result in compiler error.

	* **Qualifier**: In C++, due to industrial requirement, there are a lot of qualifier/limits that can be applied to variable/function/classes. In mylang, they are not designed YET, probably if some of them is really useful like const, just copy from C++ (or rust).

	* **Discuss**: C/C++ prohibited use without declare for industrial reason, but since python allows that and many people use it, it's at least not really bad. Also, compiler can help suggesting possible typo (by checking name similarity),  your code editor can help correcting typo, and strong type checking can also help.

  * **TBD - runtime type deduction**: Since mylang has a intime compiler, doing runtime type deduction according to user input don't need much work. But it's TBD weather do we need that function.

  * **TBD - runtime code assigning**: Since again mylang has a intime compiler, it's actually possible to assign `code` at runtime. This is bad in two ways: 1. Frequent usage will make mylang even slower than script language. 2. Now basically the original source code need to be shipped with the compiled result to make `code` recompiling work. But it is cool at least, and it might be useful in some aspect. So it's TBD weather to have this functionality.

* **Variable**: Like C++, variable name contains letter, number, and underline with first letter non-number. Specially, we can use single underline ( _ ) to hold some variable space while we don't care about that variable.

	* **TBD**: Go has a interesting design: only first-letter-capitalized variable will be exported / is public. This is good since people indeed seldom use the capitalization state of first-letter to differentiate things, and this shrinks the grammar. This is bad because it might make people uncomfortable with how they like to name things (like `myFunction`, `MyFunction`, and `my_function`).
	* 
* **Dynamic**: Mylang has dynamic compiler as well as static compiler. Static compiler compiles code to easy-to-load binary structure, and optimizes the program in the range it can. Dynamic compiler finishes the compiling of codes and continues optimizing. Here are some of the possible optimization:

	* **User input optimization**: Like runtime type deduction above, programs CAN be optimized even more if data is provided. 

	* **Hardware/OS dependent optimization**: Like even-more-runtime java,  optimize the program based on CPU/GPU spec (even including how much cycle does add/multiplication need so that compiler can choose the fastest method), CPU/GPU resource available, number of threads running in OS, and etc. 

	* **Statistical optimization**: Dynamic compiler can collect data from executable (in light weight from like some CPU support). By gathering these data, dynamic compiler will understand data level/time consumption for all parts of the program. Then it can optimize allocate algorithm, allocate constant, cache size, optimize some code over other and etc. This might sound too-advanced, but we should go toward this direction rather than avoiding trying to let compiler do auto-optimization.

	* **User need optimization**: Especially for simulator and game, user can specify the drawing quality based on their need. The dynamic compiler can directly do that (using fast math that might loss precision, throw away a lot of sanity check, etc), while without dynamic compiler it would make programming even more tiring.

	* **Discuss**: Static compiler, like all other normal compilers, itself won't encrypt the code. As a result, unencrypted program can always be decompiled, in more or less readable form. Since in mylang, runtime compiler needs almost all the info (except formatting off the code and variable names, etc), code static compiler generates would probably be more readable. But that is not a problem. Statically use dynamic compiler to create normal executable. Then put some encryption shell on it. This would prevent dynamic optimization, but it's at least as good as all other programming languages.

* **Span**: Span expands element/set/array into segments and applies "child code" separately to each segment. It's like iterating but it's not guaranteed to be in order (it might run in parallel depending on dynamic compiling and hardware), and it's not necessarily physical (it can be pure abstract like math, i.e. spanning a infinite set). The format of span is the spanned value, then codes tabbed after that value line, (see example). Span will create an auto-named element reference. The naming starts from i, and goes to j, k, l, ..., z. Note the direction is "reversed" in the code (see example). There is also a special operator `#` which can be appiled to these i,j,k s, which will return the index number (see example).

	* **Array**: Array is spanned into it's elements. In principle, the whole program will be converted into (high-dimensional) matrix operations. Dynamic compiler will dynamically optimize/recompile the code and figure out best thread number on CPU/GPU separately.

		* **Optimize equation**: If some part of the code is understood-able by compiler, the corresponding matrix operations will be converted to simple tasks, like what math softwares do: optimize the equation before plugging in data. A example would be `A -> mut 2 -> plus 3 -> negate -> pow 3 -> map A -> sort by result -> collect first 10 in A` (this is not what mylang looks like). Both compilers will examine the math properties of these functions and find out only doing sort on A is enough, and only the first 10 elements are needed. There are a lot of math identities that can be used to optimize a program, not discussing too much here.

		* **Optimize range**: If some part of the code computes some data but only few elements in that is needed, all unnecessary computations will be removed both compile-time and dynamically. A example would be `A -> whatever -> whatever -> collect first 10`. Static compiler will change that to doing all the operations only on first 10 (as long as it do not involve reordering), Another example would be `A -> whatever -> collect random 10`. The static compiler will change that to pick 10 random number first, then only doing things on these 10 position. A third example would be `A -> whatever -> collect first 10 prime`. Static compiler will convert to stop the process after the 10th prime comes out". These optimizations can be written by us easily of course, but then it would lack the beautifulness of math. My philosophy is making things pure and beautiful, leave the compiler do everything else.

		* **Optimize precision**, when spanning real interval like 0.0~1.0, if no other optimization can change this to a finite problem, a small delta value would be auto decided. Dynamic compiler will change the delta value if it feels (or the user reports) the answer is not right.

	* **Set**: Set is like array but not physical in memory. Usually it has infinite members. It's format is  `from=0,to,step=1` where the side two can be skipped.

	* **Number-castable element**: Number-castable elements are spanned into range from 0 to them, like 3 is "0,1,2,3", 5.1 is "0,0+delta,0+2delta,...,5.1", inf is "0,1,2,...,inf".

	* **Non-number-castable element**: They will span into compiler error.

* **Arbitrary precision**: Mylang internally supports arbitrary precision (since it's math-y). For technical details, high precision support can be "in compiler", or "hidden header". Mylang generally specifies number range rather than machine type, so arbitrary precision would be used if the compiler feels like it. Some compiler flag will be available to be able to tell the compiler how fast do you want the program to be. 

* **Function**: Syntax for defining function is
```
returnType arg1Type arg2Type;funcName = arg1Name arg2Name
	...yourTABBEDcode...
	funcName=...   // this is return 
```
like auto/inline declare above, types do not have to be written. Function will be overloaded if required. The type (without name) here is `returnType arg1Type arg2Type`. If some function takes a function as argument and want to write out the argument types, do it like
```
vec int  vec int  [bool int int]; qsort = a f
	...
```
The quick sort takes vector `vec int` and function `bool int int`, returns vector `vec int`.  Note `vec` is a function which takes at least a type (a `auto`) and returns a vector container that holds the type. Since it must be given one argument, there won't be ambiguity and it will pass compile. But there's no need to write out the types.
```
qsort = a f
   ...myCode...
   qsort ... ...
   qsort ... ...
```
Makes everybody's life easier. Function returning non-void will hold a variable with the same name as the function name. To return something, just write your return value on that name. It's a stack space so you can store temporary things with that if you want. If you don't write anything, the compiler will deduct the `auto` to `void` which means return nothing. Functions can be defined in-function. To write a lambda, use square brackets as we always use square brackets to solve syntax problem:
```
qsort dat [ f = a b
   f = a > b ] maybeAThirdArgument
```
Do NOT write like this:
```
qsort dat f = a b
	f = a > b
maybeAThirdArgument
```
This means define a function called qsort which takes arguemt dat, f, a,and b, and call a function called maybeAThirdArgument.

* **Object oriented programming**: Mylang prefers non-strict OOP. OOP do makes programmers' life easier, but strict OOP don't (Java is great example).  Here is the syntax
```
class myClass myClassInstance
	time;lastMessage
	vec string;messages
	int;orderInView
	pushMessage = _
	popMessage =
myClass pushMessage message =
	...yourCode...
myClass popMessage =
	...yourCode...
```
Note I didn't omit anything here. Even if you just want to write the function prototype, you have to write the equal. If no tabbed code is after that, compiler will treat it as a prototype. Class function code can either be written inside or outside. In C++ sometimes it have to be written outside due to how the compiler works, but mylang compiler works with "accept unknown things until it's deducted to be wrong" philosophy. So it's your choice to write wherever (unless you are making library). Also note that myClass has a syntax sugar. Here it creates two variable: myClass (just for fun) and myClassInstance. The non-sugar from would be: 
```
[class
	time;lastMessage
	vec string;messages
	int;orderInView
	pushMessage = _
	popMessage =
];myClass myClassInstance
...thingsAbove...
```
which is ugly. Also of course you can leave the class definition as
```
	class myClassInstance
		lastMessage
		messages
		orderInView
		pushMessage
		popMessage
```
This is legal. In this case pushMessage and popMessage will be unknown state until somebody fills it's content and compiler realizes they are functions. That's all about class. The default is public, you can use private keyword
```
...yourVars...
private
...yourPrivateVars...
```
to make things private (also protected). There's no inheritance (since that's old technique and templates are sort of better). 

* **Pointer**: Pointers are still being designed. The borrowing mechanism in Rust is a great idea. I'm not sure if it's the best idea and I'm still thinking about it. If it is, probably we will use this mechanism for pointer. Please refer to rust website for details. The idea is quite simple: there's no such thing as pointer, everything is either copied by value or borrowed by reference. The compiler will check the borrowing to decide where to release some resource.

* **Main**: Mylang don't have main, as it don't have to, argc, argv are called `argc` and `argv`.

* **Parallel**: I was talking about better parallel programming model in the Desire section above. I kind of had some ideas, but really developed, so I'll finish this part some day in the future.

# Examples with comments

## Christmas tree
```	
10					// iterate from 0 to 10
	5-i/2           // iterate from 0 to 5-i/2 where
					// i is the iterator of upper level
					// (the 0~10) one 
		print ' '
	i               // iterate from 0 to i where i is
					// the same as above
		print '*'
	print '\n'
print 'merry X'mas\n'
```
This program was used to test the first implementation of transpiler. It's available in "xmas.Cpp" (with a little bit different syntax due to limitation of first implementation, and some old philosophy which is removed)

## Selection Sort
```
[vec int 10];a            // define a as vector containing
                          // 10 ints. Note the [ ] can be 
                          // removed which won't resut in
                          // ambiguity (yet)
	read a                // like cin
a                         // span a
	a                     // span a again
		#i>#j             // if index of i is
						  // greater than
						  // index of j
			atom          // atom keyword will make
						  // operation underneath atomic 
 				i>j           // if i is greater than j
					swap i j  // exactly the swap in C++
					          // note here is swapping
					          // reference, not tmp var
a
	print a ' ' 
```
Here `#i>#j` line must be there since two spans are independent. Of course you can write things expicitly as
```
a.size-1
	i,a.size
		a[i] > a[j]
			swap a[i] a[j]
```
But this is not beautiful, as you can see. Leave the compiler to do all the optimization, and just write what you want.

#SomeFileAnalizeProgram

Before giving out the program we should talk about the problem. It's basically about card games. In card games, a deck is a combination of cards people use to play this game. Suppose I have some number of decks files in a folder, each file contains number representation of card id one by one. Now I want to find out according to these decks, what are the top 10 frequently combined cards with a card:
```
[sparseMatrix 99999999 99999999]; dat 
// create a sparse matrix for storing data
system "find deck"
// span through the system call find deck
// which basically spans through each file name
	vec int; deck
// re-initialize deck vector to store new deck
// deck have to beb stored to update dat
	istream_iterator int istream i.name
// span ints in each file
		deck
// span deck
			dat[deck,j] +=1
			dat[j,deck] +=1
// update the relations
		deck push_back j
// update deck
istream_iterator int cin
// span user input until EOF
	result = idKeptSort dat[i]
// find out top 10
	10
		print result.id ' ' result.val ' '
```
All those function names above are made up. I haven't designed STL yet and probably will just copy that from C++.

## More examples will come

# Differences of three transpilers/compilers

* The first transpiler implementations is more like "fast word processing". It tries to convert mylang to C++ line-by-line without any kind of memory from last line or ability to see future lines (except indentation information). In this case, what we can do is really limited since we don't even know the type of a symbol or was it defined before. More importantly, we don't know weather will some value be spanned or not because we can't read the next line. So here are the grammar changes:

	* If a whole line only contains a single element (string, char, number, variable, or things bracketed with brackets), it is a span.

	* Since C++ do not support `123.begin()` or `123.end()`, we have to sometimes convert span to `.begin.end`, sometimes to `for(;;)`. But we can't know what's the type of some variable. So I decided that things starting with number, dot or negate will be a number span. If you want to force something to a number span, put 0+ before the value. If you want to force something to be begin end span, put a space (space is not treated as indentation).

	* Everything else is fine for now. The current code inside is not this standard since there are some philosophy changes. I will update the transpiler in the future.

*  The second transpire implementation is total. According to the plan, it should do all the type checking, variable range checking, sanitizer inserting and etc (but not complicated ones). It should compile all correct program and should report most errors by itself (not by gcc finding out bug in translated file)

* The third implementation is the compiler. I don't want to write this at all since it is already really hard to write code for oldest standard x86, not mentioning newest x86_64 and all the optimizations, and maybe multi-platform, and all security mechanisms.  It's just NOT a good idea to write a compiler by yourself. I might try to use LLVM if that's convenient enough, or I will keep everything in transpiler.

# Their current status

* First implementation: Standard not up-to-date, function not fully implemented

* Second implementation: not written

*  Third implementation: not written 

# Others

Advises are always welcomed (zisisnotzis@gmail.com). I know this "create a programming language" is not really recommended (since it needs a lot of work) and my focused points are not following industrial trends, but I will keep up with industrial trends in the future.
