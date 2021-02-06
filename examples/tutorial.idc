--1. Single-line comments are deliniated by two hyphens:

--displayln "you can't see me".
displayln "you can see me".

-- ->you can see me

-- 2. All statements end with a period, like English (good thing only integers are implemented!). 

-- 3. Print to the console with display and displayln:

display "hello world\n".
declare something as "hello again, world".
displayln something.

-- ->hello world
-- ->hello again, world

-- 4. Performing calculations with functions and math expressions:

--Function calls are preceded by the keyword "evaluate". 
--Math expressions are enclosed by parens, and are right-associative.
--As in other functional languages like Scam, the last statement evaluated in a function is its return value.

declare mystery as function(x, y)
    {
    declare step1 as ((x + 10) / y).
    declare step2 as (step1 * 5).
    declare step3 as (2 - step2).
    (step3 + (step3 / 2)).
    }.
displayln evaluate mystery(25, 10).

-- ->-19

-- 5. You can execute only the code you feel like executing with conditionals:

--N.B.: values of different types can't be compared with '>' and '<',
--      and values of different types compared with '=' always returns false.
declare choosey as function(x)
    {
    if x is = 9000 then
        {
        displayln "you're right at, but not quite over, 9000".
        }.
    if x is > 0 and x is < 8999 or x is = 9000 then
        {
        displayln "you are not over 9000".
        }
        else then
        {
        displayln "you are over 9000, or out of bounds".
        }.
    }.
evaluate choosey(2).
evaluate choosey(-10321).
evaluate choosey(5).
evaluate choosey(9000).

-- ->you are not over 9000
-- ->you are over 9000, or out of bounds
-- ->you are not over 9000
-- ->you're right at, but not quite over, 9000
-- ->you are not over 9000

-- 6. Importantly, functions are most certainly first class objects:

declare cons as function(x,y)
    {
    function(z)
        {
        if z is = "car" then
            {
            x.
            }
        else then
            {
            if z is = "cdr" then
                {
                y.
                }
            else then
                {
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

declare displayer as function(f)
    {
    displayln f.
    displayln evaluate f().
    }.
evaluate displayer(function(){"beans".}).

-- ->5
-- ->five
-- ->must use car or cdr to access values
-- ->PRINTDONE
-- ->function<x, y>
-- ->function<z>
-- ->function<>
-- ->beans

-- 7. Arrays are a cinch:

declare anArray as array of_size 5.
displayln anArray.
set index 1 of anArray as "whaddup".
set index 3 of anArray as (15 - 12).
displayln anArray.

-- ->[nil, nil, nil, nil, nil]
-- ->[nil, "whaddup", nil, 3, nil]

--8. Iteration with a while loop is also possible:

declare i as 0.
while i is < 5 then
    {
    if i is > 0 then
        {
        display " ".
        }.
    display i.
    declare i as (i + 1).
    }.
display "\n".

--->0 1 2 3 4

--9. Assignment is dangerous, but available:

declare delectable as "potato pie".
declare bakeTooLong as function()
    {
    change delectable to_be "black ash".
    }.
displayln delectable.
evaluate bakeTooLong().
displayln delectable.

-- ->potato pie
-- ->black ash

--10. There's no nil value... but one can make do:

declare cell1 as evaluate cons(1, null).
declare cell2 as evaluate cons(3, cell1).
declare cell3 as evaluate cons(4, cell2).

declare displayList as function(list)
    {
    display evaluate list("car").
    if evaluate list("cdr") is = null then
        {
        0.
        }
    else then
        {
        display ", ".
        declare cdr as evaluate list("cdr").
        evaluate displayList(cdr).
        }.
    }.

display "displaying list: (".
evaluate displayList(cell3).
displayln ")".

-- ->displaying list: (4, 3, 1)
