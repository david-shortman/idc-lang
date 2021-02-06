declare fib as function(x)
    {
    if x is = 0 or x is < 0 then
        {
        0.
        }
    else then
        {
        if x is = 1 or x is = 2 then
            {
            1.
            }
        else then
            {
            (evaluate fib((x - 1)) + evaluate fib((x - 2))).
            }.
        }.
    }.

displayln "calculating fib of 10...".
displayln evaluate fib(40).
