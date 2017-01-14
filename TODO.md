# Type
Change from `anySymbol;var` to `var=unvaluedSymbol` or `var=type anySymbol` where `type` is STL which removes value of symbol. Cancelling use of `;`. Assigning unvalued symbol is compile-time logicing.
# Value
Change from `var=anySymbol` to `var=valuedSymbol` or `var=lit literal` where `lit` is STL which converts symbol `literal` to what it means **and stores it inside stack**. Assigning valued symbol is always **referencing**.
# Backfix
Change from `A 1 \ B 2 fix 3 \ C 4 fix 5` (which means `C 4 [B 2 [A 1] 3] 5`) to `A 1 \ B 2 \ 3 \ C 4 \ 5`. It's not as clear as previous one in this case, use `fix=\` so that `fix` can be used. It would be clear in `A 1 \ B 2 \\ C 3 \` which means `C 3 [B 2 [A 1]]` (last `\` do not have to be written) and `A 1 \\ B 2 \\ C 3` which means `C [B [A 1] 2] 3` (`\` in front of function name makes so sense, so `\ B 2` is treated as `B \ 2`. Cept STLs are designed so that `A \\ B \\ C` style is frequently used.
# Infix
Change from `A?B` where `?` is `[+-*/=]` to `A anything B`. Operator precidence is **not** implemented. Always use bracket as it's good practice.
# Scope
Change meaning of `ParentScope 1 \n \t ChildScope 2 \n 3` to `ParentScope 1 [ ChildScope  2 ] 3 `. Tabbed area will be bracketd as parameter of function.
# Span
Change from `value \n \t spanCode` to `span value \n \t spanCode` where `span` is STL taking two parameters: `value` and following `[any]` which means function/code.
# Function definition
Change from `([any])name = ([])` to `name =` where things inside `()` are optional.
Change STL from not-designed to `fn name arg1=type1 arg2=type2 ...`. Change explict overloading from not allowed to allowed when using STL.
# Class definition
Change from not-designed to `class name = \n \t var=type \n \t ...` where `class` is STL.
# STL implementation
STL is "magical" in Cept, here's how it's implemented
## [iu]nt[1248] [fu]loat[48]
```
int1                // define symbol "int1" without type or value
fn + a=int1 b       // define function "+" for symbol "int1"
                    // in following context "+" will be the named return value
    + = cp a        // reference symbol "+" to copy of symbol "a", like 
                    // "returnValue=a" in C
    add1 + b        // compiler provided assembly
fn - a=int1 b
    ...
fn * a=int1 b
    ...
fn / a=int1 b
    ...
```
That's it. `int1` is a symbol which compiler never understands. What compiler does is simply symbol processing and assembly supporting.
## comp([iu]nt[1248]|[fu]loat[48])
Same except `compint1 = class [ re=int1 \n im=int1 ]`
## [iu]nt9 [fu]loat9 comp([iu]nt|[fu]loat)9
Same except `int9 = vec int8`
## vec
```
int1                // define symbol "int1" without type or value
fn vec T n=lit 0    // define function "vec" taking type "T" and size "n"
    
```
## type
## lit
## cp
## span
## fn