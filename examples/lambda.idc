displayln "stream magic incoming...".

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

declare getNextFromStream as function(x)
    {
    declare next as evaluate cdr(x).
    next.
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

declare iStream as evaluate integerStream(0).
displayln evaluate scar(iStream).
displayln evaluate scar(evaluate scdr(iStream)).
displayln evaluate scar(evaluate scdr(evaluate scdr(iStream))).

displayln "a freeroaming lambda makes its way into some parameters".
declare displayer as function(f)
    {
    displayln f.
    displayln evaluate f().
    }.
evaluate displayer(function(){"beans".}).
