# Kaleidoscope
Kaleidoscope is a functioal toy language following the [LLVM tutorial](http://llvm.org/docs/tutorial/index.html). 



### Build

##### Requirements

- Environment: OS X, Linux
- Compiler: g++, clang
- Libraries: LLVM 3.7 or later


##### LLVM Install

Recomend dowload the [pre-built binaries LLVM](http://llvm.org/releases/download.html#3.8.0), rather than compiling it manually with source code. Untarred it into arbitrary directory ( e.g. `$HOME/workspace/LLVM380` ) after downloading.

##### Make

A consice `Makefile` is given to compile Kaleidoscope immediately. Just before, you should modify the `LLVM_SRC_PATH` variable to point to the correct direcoty LLVM lies in last step.

After that, just make it fly:

```shell
$ make
clang++ -fno-rtti -O3 -g -std=c++11 -I ./include `$HOME/Workspace/LLVM380/bin/llvm-config --cxxflags` src/kaljit.cc `$HOME/Workspace/LLVM380/bin/llvm-config --ldflags --system-libs --libs core mcjit native` -o kal
```



### Sample

```shell
$ ./kal
ready> def fib(x) if (x < 3) then 1 else fib(x-1)+fib(x-2);
Read function definition:
define double @fib(double %x) {
...

ready> fib(10);
ready> Evaluated to 55.000000

$ ./kal < test/mandel.ks
ready> Read function definition:
define double @"unary!"(double %v) {
...
```



### Language Spec

##### Tokens

- EOF
- id
- number (float)
- op
- 'if'
- 'then'
- 'else'
- 'for'
- 'var'
- 'in'
- 'unary'
- 'binary'

##### CFG

```
kal ::= 'def' prototype expression
		| 'extern' prototype
		| expression | ';'
		
prototype ::= id '(' [id [id ',']* ]* )'
			| 'binary' op number? '(' id ',' id ')'
			| 'unary' op '(' id ')'

expression ::= unary binary

binary ::= [op unary]*

unary ::= primary | op unary

primary ::= idexpr | numexpr | parenexpr | ifexpr | forexpr | varexpr

idexpr ::= id | callexpr

callexpr ::= id '(' expression* ')'

numexpr ::= number

parenexpr ::= '(' expression ')'

ifexpr ::= 'if' expression 'then' expression 'else' expression

forexpr ::= 'for' id '=' expression ',' expression [',' expression]? 'in' expression

varexpr ::= 'var' id ['=' expression]? [',' id ['=' expression]?]* 'in' expression
```

