# Cept Programming Language Specification
    
## Symbol
Symbol is like variable in most programming languages but not necessarily exist in an executable or even mean anything. It is an abstract notation that will be resolved at compile time. Symbol live in it's and all it's child scopes. Child scope symbol auto overrides parent scope symbol with same name.

Notation:`[^ \t+-*/><\\=\[\];.]*` **except** number/string/range form
Yes, unlike most programming languages with `[A-Za-z_][A-Za-z0-9_]*`, symbol are designed that it's limited to as less as possible. Though, it's a bad practise to use wired punctuation or non-ASCII word as symbol.

Predefined symbols:
`int`,`int1`,`int2`,`int4`,`int8`,`int9`: First one auto converts to everything else this **and** next line. Second to fifth ones are `char`,`int`,`long`,`long long` in C. Last one is high definition integer.
`unt`,`unt1`,`unt2`,`unt4`,`unt8`,`unt9`: Unsigned version of above. First one auto converts to everything else this line.
`float`,`float4`,`float8`,`float9`: Float version of above. 
`ufloat`,`uloat4`,`uloat8`,`ufloat9`: Unsigned float version of above.
`comp`,`comp4`,`comp8`,`comp9`: Complex version of above.
`uomp`,`uomp4`,`uomp8`,`uomp9`: Unsigned complex version of above.
`char`: Alias of `unt1`.
`string`: Alias of `vec char` where `vec` is some STL.
`any`: Symbol that accepts **anything**, more like #define in C. Note this is not compiler magic!
`bool`: Alias of `enum false true` where `enum` is some STL working like `enum` in C. The order is important since `false` should be 0. 
STL: The tyes above are actually all defined in STL. There's no comipler type as there's no compiler keyword. STL contains a lot more than that, not listing here.

## Scope
Scope is like most programming languages but with different notation.

Notation: Tab child-scope code after parent-scope code to make a scope. Like python.
```
ParentScopeCode
    ChildScopeCode1
    ChildScopeCode2
```

## Symbol type/value

Symbol can (but don't have to) contain a value. When symbol contains a value, a type will also be auto-decided and binded to it. When compiler can't decide the type, a compiling error will be thrown and type must be explicitly written. Types once deducted **can't** be changed inside a scope.

Notation:`symbol=value`(auto decide) or `symbol=type;value` or `type;symbol=value` or `type;symbol`
`type` in second and thrid example is unnecessary since `=` always copys a type. Fourth example assigns type without value. In this case the symbol value would be zero value of that type. That's the only case when compiler cannot decide the type. When `value` is not a literal or symbol, it's type would be the same as `any`, which is no-type. In fact the definiton of `any` is `any\n` (without giving any type or value). Also note these expressions all returns `symbol` itself. People might wonder why use `;`. That's because `;` is the second easiest key to press on qwerty keyboard (the first is space but appearently it can't be used).

Value: In example above, `value` is a symbol and `symbol` will **refer** to `value`. To duplicate, use `symbol=cp value` where `cp` is some STL. This still works when `value` contains no value, like alises of a symbol. Note the actual compiled result might pass value (if it's faster than passing pointer) as an optimzation, but that will only happen there's no code trying to modify that value.

Type: `type` above is a symbol or literal. Only type information will be extracted from `type` and that becomes type of `symbol`. Some symbol/literal when used as type have special meaning:
* `NumberLiteral`: `NumberLiteral` can only be number literal. It is the not-reachable upper-limit of `symbol` where 0 is the lower-limit. If it's integer, `symbol` is integer-type. If it's float or complex, it works the same way. This decides not only the actual size/type used in assembly, but also a debug-time check mecanism.
* `[Number Number]`: `Number` can either be literal or symbol value. This is "lower-limit equals second number" version of above. In this case the mechine would actually record down value-lowerLimit and everything will be transparent to user. Note there's no pointer in Cept, everything is reference or value.
* `[Number Number Number]`: "Step-size equals third number" version of above. In this case `symbol` would be limited to discrete values. This only makes sense in debug-time check.
* The previous three things are called "range". They're named because they'll be used again later.
* `[enum symbol1 symbol2 symbol3 ...]`: Like enum in C.

Cast: Valued symbol in expression will be replaced by their value. They can't be used as pure symbol anymore (in this scope). 

## Literal
Like most programming languages, value can be literally written for convenience. In cept, there are number literals and string literals.

Number Literal Notation: `-?[0-9]*([0-9].[0-9]*)?`
Negative is treated as number literal rather than expression. Expression won't allow two-sided operator have only one side, so `-123` is legal while `+123` isn't.

String Literal Notation: `'your string content'`
Char and string all uses this form. Compiler will auto-decide is it a char or a string but counting words inside and would it be used as a string later somewhere.

## Function Call
In most programming languages, function call looks like `func(arg1,arg2,arg3...)`. This includes (for qwerty keyboard) `func shift 9 arg1 , arg2 , arg2 shift 0`. Function calls tend to appear on most of the lines in a program, usually more than once, and these commas and brackets are not logically necessary. So Cept uses a different form:

Prefix Notation: `func arg1 arg2 arg3`
Yes that's like shell arguments in strongly typed compiled language. This is **not** shell-ish since symbol(variables) can't be access with `$variable`. Since Cept grammar forces white space, there won't be confusion like is that `arg1++` or `++arg2` so it's totally riguous.

Backfix Notation: `arg2 \ func arg1 fix arg3`
This is the backfix notation of function call. It translates to `func arg1 arg2 arg3` in Cept. `fix` is like a keywoard but not actually forbidden to use. Cept is more tend to create a no-keywoard language so I'm still thinking about that. If no `fix` is given, it would be "added" to the end of this function. Note when LHS has multiple parts, `arg2 arg3 \ func arg1 fix arg4` actually translates to `func arg1 [arg2 arg3] arg4`, obviously, since this is not script language.

Infix Notation: `LHS+RHS`
Yes. Addition, subtraction, mutiplication, and division are, still, STL, with some compiler magic supporting them so that we don't need to write `1 + 2`, just `1+2`. `+-*/=` can't be used as symbol **except** when defining function. People can override them as they want.

Bracket: Bracket is too short for one section so it's moved here. To express function call in function call like `A B C D` (is that `A(B(C,D))`, or `A(B(C),D)` suppose A and B can all take multiple arguments), use bracket `[]`. For same reason above, Cept uses the only no-shift bracket `[]`. Bracktly is used **everywhere** in Cept. It's such a convenient concept that we can use it to do a lot of things. To express `A(B(C),D)`, use `A [B C] D`. It's more like math bracket.

Try-To-Parse: Brackets are **not** grammar, they are just used to explain things clearly so that compiler won't detect multiple possiblities and throw error. But if there is only one possibility, you don't need to do that. For example `string;a`. It can also be written as `vec char;a` which can be either `[vec char];a` or `vec [char;a]` and are both legal (the second one looks wield but it means `char a;vector<decltype(a)> nothing`). That's the reason why we have `string`. But if we have function `print` taking `string`s while function `itoa` taking `int`s, `print iota 3+5 '=' itoa 3 '+' itoa 5` would have only one reasonable explaination so no bracket is needed (note, overdoing this is a bad practice since code would not be human-readable) (note2, print actually takes any printable type, function pointer is not printable).

## Expression
Expression is symbol and it's type/value assigning, literal, function call, bracket, `+-*/=` and etc. combining together (or only one of them). Cept program is build with expressions.

Notation:
```
Expression = Expression +|-|*|/ Expression
           | [ Expression ]
           | anything
```
Like normal C expression but with much less symbols. Bitwise operatiors are replaced by functions in STL (they aren't frequently used anyway except `&&` and `||` which can be replaced by `*` and `\` if the operand is `bool` type. 

## Span
Programs are loops. There are always a lot of loops in a program so it would be nice if we have a good syntax for that. Old C++ uses `for(auto it=xxx.begin();it!=xxx.end(),++it)` and that's tiring. C++11 introduces `for(auto& it: xxx)` but that's still not convenient enough. Thanks to scope syntax in Cept, we have the following syntax

Notation:
```
xxx
    LoopCode1
    LoopCode2
```
This is sort of extreme, but it makes a lot of sense. What does it mean when we tab some code under another? We we write `for`/`while`/`if`, that's exactly what's happening here. Since we are iterating/spanning through `xxx`, that's `for`/`while`, and thanks to the "range" concept above, we can also do `if` with this: When expression evaluates to `true`, that's 1 and becomes `for(i=0;i<1;++i)` while `false` is 0.

Index: **content of iterators** (reference to each element) are auto named `CurrentIndent + 'h'`. This might seen wierd. The original design was to make it `i`,`j`,`k` one following another, but we find counting how many tabs is much easier than memorizing how many loop variable was use. Also thanks to the scope syntax in Cept, the two things here are usually the same (except in class/function function code). At the same time, the loop variable counting then index are called `I`, `J`, `K` etc.

## Function Definition
Function definition in programming languages are alway# Cept Programming Language Specification
    
## Symbol
Symbol is like variable in most programming languages but not necessarily exist in an executable or even mean anything. It is an abstract notation that will be resolved at compile time. Symbol live in it's and all it's child scopes. Child scope symbol auto overrides parent scope symbol with same name.

Notation:**anything**
Yes, unlike most programming languages with `[A-Za-z_][A-Za-z0-9_]*`, to allow more tricks to be used, symbol can have any form. Every grammar in Cept will be overriden if a symbol with exactly that notation is presented, but do note that defining unnecessary wierd symbol is a really bad practice.

Predefined symbols:
`int`,`int1`,`int2`,`int4`,`int8`,`int9`: First one auto converts to everything else this **and** next line. Second to fifth ones are `char`,`int`,`long`,`long long` in C. Last one is high definition integer.
`unt`,`unt1`,`unt2`,`unt4`,`unt8`,`unt9`: Unsigned version of above. First one auto converts to everything else this line.
`float`,`float4`,`float8`,`float9`: Float version of above. 
`ufloat`,`uloat4`,`uloat8`,`ufloat9`: Unsigned float version of above.
`comp`,`comp4`,`comp8`,`comp9`: Complex version of above.
`uomp`,`uomp4`,`uomp8`,`uomp9`: Unsigned complex version of above.
`char`: Alias of `unt1`.
`string`: Alias of `vec char` where `vec` is some STL.
`any`: Symbol that accepts **anything**, more like #define in C. Note this is not compiler magic!
`bool`: Alias of `enum false true` where `enum` is some STL working like `enum` in C. The order is important since `false` should be 0. 
STL: The tyes above are actually all defined in STL. There's no comipler type as there's no compiler keyword. STL contains a lot more than that, not listing here.

## Scope
Scope is like most programming languages but with different notation.

Notation: Tab child-scope code after parent-scope code to make a scope. Like python.
```
ParentScopeCode
    ChildScopeCode1
    ChildScopeCode2
```

## Symbol type/value

Symbol can (but don't have to) contain a value. When symbol contains a value, a type will also be auto-decided and binded to it. When compiler can't decide the type, a compiling error will be thrown and type must be explicitly written. Types once deducted **can't** be changed inside a scope.

Notation:`symbol=value`(auto decide) or `symbol=type;value` or `type;symbol=value` or `type;symbol`
`type` in second and thrid example is unnecessary since `=` always copys a type. Fourth example assigns type without value. In this case the symbol value would be zero value of that type. That's the only case when compiler cannot decide the type. When `value` is not a literal or symbol, it's type would be the same as `any`, which is no-type. In fact the definiton of `any` is `any\n` (without giving any type or value). Also note these expressions all returns `symbol` itself. People might wonder why use `;`. That's because `;` is the second easiest key to press on qwerty keyboard (the first is space but appearently it can't be used).

Value: In Cept, symbol is **always** passed by reference. To pass by value, use `symbol=cp value` where `cp` is STL. `symbol=value` still works when `value` contains no value, like alises of symbol. Note that the actual compiled result might pass value (if it's faster than passing pointer) as an optimzation, but that's transparent.

Type: `type` above is a symbol or literal. Only type information will be extracted from `type` and that becomes type of `symbol`. Some symbol/literal when used as type have special meaning:
* `NumberLiteral`: Range from 0 to itself. It's how STL implements `unt*`,`uloat*` and etc. Cept implements almost everything in STL rather than compiler. This is a good example.
* `set Number`: Same as above except `Number` can be const symbol. `set` is STL. How `set` works will be mentioned later.
* `set Number Number`: Range from RHS to LHS. It's how STL implements `int*`,`float*` and etc. Actual stored value subtracts it's lowerlimit if `set` finds this can result in a smaller type. `set` would actually return a function to implement this.
* `enum symbol1 symbol2...`: Like `emun` in C. `enum` is STL.

## Literal
Like most programming languages, value can be literally written for convenience. In cept, there are number literals and string literals.

Number Literal Notation: `-?[0-9]*\.?[0-9]*`
Negative is treated as number literal rather than expression. Expression won't allow two-sided operator have only one side, so `-123` is legal while `+123` isn't.

String Literal Notation: `'Your String Content`'
Char and string all uses this form. Compiler will auto-decide is it a char or a string but counting words inside and would it be used as a string later somewhere.

## Function Call
In most programming languages, function call looks like `func(arg1,arg2,arg3...)`. This includes (for qwerty keyboard) `func shift 9 arg1 , arg2 , arg2 shift 0`. Function calls tend to appear on most of the lines in a program, usually more than once, and these commas and brackets are not logically necessary. So Cept uses a different form:

Prefix Notation: `func arg1 arg2 arg3`
Yes that's like shell arguments in strongly typed compiled language. This is **not** shell-ish since symbol(variables) can't be access with `$variable`. Since Cept grammar forces white space, there won't be confusion like is that `arg1++` or `++arg2` so it's totally riguous.

Backfix Notation: `arg2 \ func arg1 fix arg3`
This is the backfix notation of function call. It translates to `func arg1 arg2 arg3` in Cept. `fix` is like a keywoard but not actually forbidden to use. Cept is more tend to create a no-keywoard language so I'm still thinking about that. If no `fix` is given, it would be "added" to the end of this function. Note when LHS has multiple parts, `arg2 arg3 \ func arg1 fix arg4` actually translates to `func arg1 [arg2 arg3] arg4`, obviously, since this is not script language.

Infix Notation: `LHS+RHS`
Yes. Addition, subtraction, mutiplication, and division are, still, STL, with some compiler magic supporting them so that we don't need to write `1 + 2`, just `1+2`. `+-*/=` can't be used as symbol **except** when defining function. People can override them as they want.

Bracket: Bracket is too short for one section so it's moved here. To express function call in function call like `A B C D` (is that `A(B(C,D))`, or `A(B(C),D)` suppose A and B can all take multiple arguments), use bracket `[]`. For same reason above, Cept uses the only no-shift bracket `[]`. Bracktly is used **everywhere** in Cept. It's such a convenient concept that we can use it to do a lot of things. To express `A(B(C),D)`, use `A [B C] D`. It's more like math bracket.

Try-To-Parse: Brackets are **not** grammar, they are just used to explain things clearly so that compiler won't detect multiple possiblities and throw error. But if there is only one possibility, you don't need to do that. For example `string;a`. It can also be written as `vec char;a` which can be either `[vec char];a` or `vec [char;a]` and are both legal (the second one looks wield but it means `char a;vector<decltype(a)> nothing`). That's the reason why we have `string`. But if we have function `print` taking `string`s while function `itoa` taking `int`s, `print iota 3+5 '=' itoa 3 '+' itoa 5` would have only one reasonable explaination so no bracket is needed (note, overdoing this is a bad practice since code would not be human-readable) (note2, print actually takes any printable type, function pointer is not printable).

## Expression
Expression is symbol and it's type/value assigning, literal, function call, bracket, `+-*/=` and etc. combining together (or only one of them). Cept program is build with expressions.

Notation:
```
Expression = Expression +|-|*|/ Expression
           | [ Expression ]
           | anything
```
Like normal C expression but with much less symbols. Bitwise operatiors are replaced by functions in STL (they aren't frequently used anyway except `&&` and `||` which can be replaced by `*` and `\` if the operand is `bool` type. 

## Span
Programs are loops. There are always a lot of loops in a program so it would be nice if we have a good syntax for that. Old C++ uses `for(auto it=xxx.begin();it!=xxx.end(),++it)` and that's tiring. C++11 introduces `for(auto& it: xxx)` but that's still not convenient enough. Thanks to scope syntax in Cept, we have the following syntax

Notation:
```
xxx
    LoopCode1
    LoopCode2
```
This is sort of extreme, but it makes a lot of sense. What does it mean when we tab some code under another? We we write `for`/`while`/`if`, that's exactly what's happening here. Since we are iterating/spanning through `xxx`, that's `for`/`while`, and thanks to the "range" concept above, we can also do `if` with this: When expression evaluates to `true`, that's 1 and becomes `for(i=0;i<1;++i)` while `false` is 0.

Index: **content of iterators** (reference to each element) are auto named `CurrentIndent + 'h'`. This might seen wierd. The original design was to make it `i`,`j`,`k` one following another, but we find counting how many tabs is much easier than memorizing how many loop variable was use. Also thanks to the scope syntax in Cept, the two things here are usually the same (except in class/function function code). At the same time, the loop variable counting then index are called `I`, `J`, `K` etc.

## Function Definition
Function in most programming languages are always tricky since they are a huge exception of "definning" syntax. If function overloading is allowed, the exception would be even bigger. Function in Cept is block of `any`, defined with:
```
functionName = [
    code1
    code2
]
```
We'll add a small while conveinent syntax sugar: When RHS of `=` is empty, `[]` will automatically be added to the child scope. Note that this is **not** span.This would be span but it's meaningless: 
```
funcName = [
    yourFunctionCode
]   yourSpanCode
```

Parameter of functions will be auto-named arg0, arg1, arg2 while return value is named the same as function name. Define symbol yourself to use different name (this would not cost any extra resource due to symbol resoving in compiler):
```
stringIndex =
    str = arg0
    idx = arg1
    ...code...
    stringIndex = foo // equals to return foo, but stringIndex is actually the receiver of return value, which means you can read from the recevier or use it as temporary space
```
There's no explicit function overloading in Cept since that don't make sense. Overload **is** supported like `auto`-based overloading in C++11. With the same rule, function overriding is not allowed in same scope. To force argument type, use
```
    type arg0 != string
        0=0 // arg0 must be a string
```
`type` is STL which removes value of symbol. 0=0 is obviously not allowed, so this will work.

STL provides a more usual way to write function:


## Class
Cept is non strict object-oriented programming language which supports class of course. The point of OOP is to make things clear and easy, and that's what Cept is trying to optimize.

Notation:
```
class;myClassName=
    type;myDataMenber
    type;myAnotherDataMenber
```

Minimal Notation:
```
myClassName=
s tricky since they are a huge exception of "definning" syntax. If function overloading is allowed, the exception would be even bigger. Cept is trying to be totally logical, so we will add two rules:
* Type definition `type;symbol` makes function when `symbol` is a list of symbols
* `=` equates the whole span (and adds `[]` to it) if it's at end of line (this is just a syntax sugar so that we don't need to write `[]`)

Notation:
```
returnType arg1Type arg2Type ;funcName arg1Name arg2Name =
    funcCode
    funcCode
```
The correct parsing should be `[returnType arg1Type arg2Type];[funcName arg1Name arg2Name]=`. No other parsing will success as long as all names are not defined. You can choose not to write the types, but it still at least have to be `;funcName arg1Name arg2Name=`. In this case the types will be auto-decided.

Type auto-deciding:
General function overloading makes no sense, while auto-based overloading do, and that's what Cept is trying to do. In cept, `any` can accept any type, which is also true for function. For the minimal example above, the types are actually `[any any any]` and the `any`s would be replaced by deducted result.

## Class
Cept is non strict object-oriented programming language which supports class of course. The point of OOP is to make things clear and easy, and that's what Cept is trying to optimize.

Notation:
```
class;myClassName=
    type;myDataMenber
    type;myAnotherDataMenber
```

Minimal Notation:
```
myClassName=
