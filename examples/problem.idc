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

declare car as function(x)
    {
    evaluate x("car").
    }.

declare cdr as function(x)
    {
    evaluate x("cdr").
    }.

declare integerStream as function(x)
    {
    declare nextValue as function()
        {
        evaluate integerStream((x + 1)).
        }.
    evaluate cons(x, nextValue).
    }.

declare scar as function(x)
    {
    evaluate car(x).
    }.

declare scdr as function(x)
    {
    declare next as evaluate cdr(x).
    evaluate next().
    }.

--Sieve

declare streamRemove as function(predicate, stream)
    {
    if evaluate predicate(evaluate scar(stream)) is = 1 then
        {
        declare delFunc as function()  
            {
            evaluate streamRemove(predicate, evaluate scdr(stream)).
            }.
        evaluate cons(evaluate scar(stream), delFunc).
        }
    else then
        {
        evaluate streamRemove(predicate, evaluate scdr(stream)).
        }.
    }.

declare integer? as function(op1, op2)
    {
    if (op1 % op2) is = 0 then {0.}
    else then{1.}.
    }.


declare seive as function(s)
    {
    evaluate 
    cons(evaluate scar(s), 
         function(){
            evaluate seive(evaluate streamRemove(function(x){evaluate integer?(x, evaluate scar(s)).}, 
                                                    evaluate scdr(s))).
                }).
    }.
declare iStream as evaluate integerStream(2).
declare primes as evaluate seive(iStream).
displayln "seivin' 20 times".
declare count as 0.
declare next as primes.
while count is < 20 then
    {
    if count is > 0 then
        {
        display ", ".
        }.
    display evaluate scar(next).
    declare next as evaluate scdr(next).
    declare count as (count + 1).
    }.
display "\n".
