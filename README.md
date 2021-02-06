# idc-lang

An example of a (very) basic interpreted language.

### Overview

idc executes `.idc` files by

- **Lexing** the source code into identified lexemes like `INTEGER`, `STRING`, `OPAREN`, `NULL`, `IF`, `NEXT`, `PRINTDONE`, etc

- **Parsing** the list of lexemes into a parse tree which organizes units of meaning like functions, expressions, and conditionals

- **Evaluating** the parse tree

Evaluation occurs in environments definied similarly to Scheme-like languages.

## Usage

<small>Examples are available in the [examples directory](./examples)</small>

### Compile
- Run the command `chmod +x idc`
- Run `make`
- Now you can interpret source code with `./idc filename.idc`

### Usage

<small>All code seen below is in tutorial.idc.</small>

#### Comments

Single-line comments are deliniated by two hyphens.

```
--displayln "you can't see me".
displayln "you can see me".
```

output
```
you can see me
```

#### Logging

Print to the console with `display` and `displayln`.

```
display "hello world\n".
declare something as "hello again, world".
displayln something.
```

output
```
hello world
hello again, world
```

### Expressions

Perform calculations with functions and math expressions.

- Function calls are preceded by the keyword `evaluate`. 
- Math expressions are enclosed by parentheses, and are right-associative.
- As in other functional languages like Scheme, the last statement evaluated in a function is its return value.

```
declare mystery as function(x, y) {
    declare step1 as ((x + 10) / y).
    declare step2 as (step1 * 5).
    declare step3 as (2 - step2).
    (step3 + (step3 / 2)).
}.
displayln evaluate mystery(25, 10).
```

output
```
-19
```

#### Conditionals

You can execute only the code you feel like executing with conditionals.

- Values of different types can't be compared with `>` and `<`
- Comparing values of different types with `=` always returns false

```
declare choosey as function(x) {
    if x is = 9000 then {
        displayln "you're right at, but not quite over, 9000".
    }.
    if x is > 0 and x is < 8999 or x is = 9000 then {
        displayln "you are not over 9000".
    } else then {
        displayln "you are over 9000, or out of bounds".
    }.
}.
evaluate choosey(2).
evaluate choosey(-10321).
evaluate choosey(5).
evaluate choosey(9000).
```

output
```
you are not over 9000
you are over 9000, or out of bounds
you are not over 9000
you're right at, but not quite over, 9000
you are not over 9000
```

#### Functions

Functions are first class objects.

The return value of a function is the value of the last statement in the function.

```
declare cons as function(x,y) {
    function(z) {
        if z is = "car" then {
            -- the value of x is returned
            x.
        }
        else then {
            if z is = "cdr" then {
                y.
            }
            else then {
                displayln "must use car or cdr to access values".
            }.
        }.
    }.
}.

declare cell as evaluate cons(5, "five").
displayln evaluate cell("car").
displayln evaluate cell("cdr").
displayln evaluate cell("garbage").
displayln cons.
displayln cell.

declare displayer as function(f){
    displayln f.
    displayln evaluate f().
}.
evaluate displayer(function(){"beans".}).
```

output
```
5
five
must use car or cdr to access values
PRINTDONE
function<x, y>
function<z>
function<>
beans
```

#### Arrays

Arrays are very rudimentary.

- They are declared with a set size and no initial values
- The array is mutated at a given indicie with `set`

```
declare anArray as array of_size 5.
displayln anArray.
set index 1 of anArray as "whaddup".
set index 3 of anArray as (15 - 12).
displayln anArray.
```

output
```
[null, null, null, null, null]
[null, "whaddup", null, 3, null]
```

### Iteration

Only `while` loops are provided.

```
declare i as 0.
while i is < 5 then {
    if i is > 0 then {
        display " ".
    }.
    display i.
    declare i as (i + 1).
}.
display "\n".
```

output
```
0 1 2 3 4
```

#### Assignment

Assignment is bad practice for functional programmers*, but available.

```
declare delectable as "potato pie".
declare bakeTooLong as function() {
    change delectable to_be "black ash".
}.
displayln delectable.
evaluate bakeTooLong().
displayln delectable.
```

output
```
potato pie
black ash
```

*Using this language to develop anything is also bad practice.

## Debug

Internal debugging output can be enabled by calling the `idcInterpreter` directly with the `--debug` flag.

The parse tree will be printed before it is evaluated.

A simple `PROGRAM`:

```
declare a as 1.
displayln a.
```

will be parsed as:

```
PROGRAM
1 left: 
  DECLARE
  2 left: 
    VARIABLE: a
    3 left: 
    3 right: 
  2 right: 
    INTEGER: 1
    3 left: 
    3 right: 
1 right: 
  NEXT
  2 left: 
    DISPLAYLN
    3 left: 
      VARIABLE: a
      4 left: 
      4 right: 
    3 right: 
  2 right: 
    NEXT
    3 left: 
    3 right:
```

<small>(actually you can pass literally anything second argument to `idcInterpreter` and it will product debug output)</small>