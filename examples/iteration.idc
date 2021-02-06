declare fibIter as function(x)
    {
    declare previous as 1.
    declare current as 1.
    declare next as 1.
    declare i as 3.
    while i is < x or i is = x then
        {
        declare next as (current + previous).
        declare previous as current.
        declare current as next.
        declare i as (i + 1).
        }.
    next.
    }.
displayln "calculating fib of 10".
displayln evaluate fibIter(10).
