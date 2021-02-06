--Basic key structure functions-----
declare cons as function(x, y)
    {
    declare x as x.
    declare y as y.
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
                function(c, v)
                    {
                    if c is = "carset" then
                        {
                        change x to_be v.
                        }
                    else then
                        {
                        change y to_be v.
                        }.
                    }.
                }.
            }.
        }.
    }.

declare car as function(c)
    {
    evaluate c("car").
    }.

declare cdr as function(c)
    {
    evaluate c("cdr").
    }.

--AVL tree--

declare newNode as function(key, left, right, height, data)
    {
    evaluate cons(evaluate cons(evaluate cons(key, data), height), evaluate cons(left, right)).
    }.

declare key as function(node)
    {
    evaluate car(evaluate car(evaluate car(node))).
    }.

declare data as function(node)
    {
    evaluate cdr(evaluate car(evaluate car(node))).
    }.

declare height as function(node)
    {
    if node is = null or node is = 0 then
        {
        0.
        }
    else then
        {
        evaluate cdr(evaluate car(node)).
        }.
    }.

declare left as function(node)
    {
    evaluate car(evaluate cdr(node)).
    }.

declare right as function(node)
    {
    evaluate cdr(evaluate cdr(node)).
    }.

declare left! as function(node, newLeft)
    {
    evaluate newNode(evaluate key(node), newLeft, evaluate right(node), evaluate height(node), evaluate data(node)).
    }.

declare right! as function(node, newRight)
    {
    evaluate newNode(evaluate key(node), evaluate left(node), newRight, evaluate height(node), evaluate data(node)).
    }.

declare height! as function(node, newHeight)
    {
    evaluate newNode(evaluate key(node), evaluate left(node), evaluate right(node), newHeight, evaluate data(node)).
    }.

declare keyAndValue! as function(node, newKey, newValue)
    {
    evaluate newNode(newKey, evaluate left(node), evaluate right(node), evaluate height(node), newValue).
    }.

declare find as function(e, t)
    {
    if t is = null then
        {
        null.
        }
    else then
        {
        if e is < evaluate key(t) then
            {
            evaluate find(e, evaluate left(t)).
            }
        else then
            {
            if e is > evaluate key(t) then
                {
                evaluate find(e, evaluate right(t)).
                }
            else then
                {
                evaluate data(t).
                }.
            }.
        }.
    }.

declare findMin as function(t)
    {
    if t is = null then
        {
        null.
        }
    else then
        {
        if evaluate left(t) is = null then
            {
            t.
            }
        else then
            {
            evaluate findMin(evaluate left(t)).
            }.
        }.
    }.

declare findMax as function(t)
    {
    if t is = null then
        {
        null.
        }
    else then
        {
        if evaluate right(t) is = null then
            {
            t.
            }
        else then
            {
            evaluate findMax(evaluate right(t)).
            }.
        }.
    }.

declare max as function(x, y)
    {
    if x is > y then
        {
        x.
        }
    else then
        {
        y.
        }.
    }.

declare singleRotateWithLeft as function(k2)
    {
    declare k1 as evaluate left(k2).
    change k2 to_be evaluate left!(k2, evaluate right(k1)).
    change k1 to_be evaluate right!(k1, k2).
    change k2 to_be evaluate height!(k2, (evaluate max(evaluate height(evaluate left(k2)), evaluate height(evaluate right(k2))) + 1)).
    change k1 to_be evaluate height!(k1, (evaluate max(evaluate height(evaluate left(k1)), evaluate height(k2)) + 1)).
    k1.
    }.

declare singleRotateWithRight as function(k1)
    {
    declare k2 as evaluate right(k1).
    change k1 to_be evaluate right!(k1, evaluate left(k2)).
    change k2 to_be evaluate left!(k2, k1).
    change k1 to_be evaluate height!(k1, (evaluate max(evaluate height(evaluate left(k1)), evaluate height(evaluate right(k1))) + 1)).
    change k2 to_be evaluate height!(k2, (evaluate max(evaluate height(evaluate right(k2)), evaluate height(k1)) + 1)).
    k2.
    }.

declare doubleRotateWithLeft as function(k3)
    {
    change k3 to_be evaluate left!(k3, evaluate singleRotateWithRight(evaluate left(k3))).
    evaluate singleRotateWithLeft(k3).
    }.

declare doubleRotateWithRight as function(k1)
    {
    change k1 to_be evaluate right!(k1, evaluate singleRotateWithLeft(evaluate right(k1))).
    evaluate singleRotateWithRight(k1).
    }.

declare insert as function(e, v, t)
    {
    if t is = null then
        {
        change t to_be evaluate newNode(e, null, null, 0, v).
        }
    else then
        {
        if e is < evaluate key(t) then
            {
            change t to_be evaluate left!(t, evaluate insert(e, v, evaluate left(t))).
            if (evaluate height(evaluate left(t)) - evaluate height(evaluate right(t))) is = 2 then
                {
                if e is < evaluate key(evaluate left(t)) then
                    {
                    change t to_be evaluate singleRotateWithLeft(t).
                    }
                else then
                    {
                    change t to_be evaluate doubleRotateWithLeft(t).
                    }.
                }.
            }
        else then
            {
            if e is > evaluate key(t) then
                {
                change t to_be evaluate right!(t, evaluate insert(e, v, evaluate right(t))).
                if (evaluate height(evaluate right(t)) - evaluate height(evaluate left(t))) is = 2 then
                    {
                    if e is > evaluate key(evaluate right(t)) then
                        {
                        change t to_be evaluate singleRotateWithRight(t).
                        }
                    else then
                        {
                        change t to_be evaluate doubleRotateWithRight(t).
                        }.
                    }.
                }.
            }.
        }.
    change t to_be evaluate height!(t, (evaluate max(evaluate height(evaluate left(t)), evaluate height(evaluate right(t))) + 1)).
    t.
    }.

declare getBalance as function(t)
    {
    if t is = null then
        {
        0.
        }
    else then
        {
        (evaluate height(evaluate left(t)) - evaluate height(evaluate right(t))).
        }.
    }.

declare delete as function(t, k)
    {
    if t is = null then
        {
        t.
        }
    else then
        {
        if k is < evaluate key(t) then
            {
            change t to_be evaluate left!(t, evaluate delete(evaluate left(t), k)).
            }
        else then
            {
            if k is > evaluate key(t) then
                {
                change t to_be evaluate right!(t, evaluate delete(evaluate right(t), k)).
                }
            else then
                {
                if evaluate left(t) is = null or evaluate right(t) is = null then
                    {
                    declare temp as null.
                    if evaluate left(t) is = null then
                        {
                        change temp to_be evaluate right(t).
                        }
                    else then
                        {
                        change temp to_be evaluate left(t).
                        }.
                    if temp is = null then
                        {
                        change temp to_be t.
                        change t to_be null.
                        }
                    else then
                        {
                        change t to_be temp.
                        }.
                    }
                else then
                    {
                    declare temp as evaluate findMin(evaluate right(t)).
                    change t to_be evaluate keyAndValue!(t, evaluate key(temp), evaluate data(temp)).
                    change t to_be evaluate right!(t, evaluate delete(evaluate right(t), evaluate key(temp))).
                    }.
                }.
            }.
        if t is = null then
            {
            t.
            }
        else then
            {
            change t to_be evaluate height!(t, (evaluate max(evaluate height(evaluate left(t)), evaluate height(evaluate right(t))) + 1)).
            declare balance as evaluate getBalance(t).
            if evaluate getBalance(evaluate left(t)) is > 0 or evaluate getBalance(evaluate left(t)) is = 0 and balance is > 1 then
                {
                evaluate singleRotateWithLeft(t).
                }
            else then
                {
                if balance is > 1 and evaluate getBalance(evaluate left(t)) is < 0 then
                    {
                    evaluate doubleRotateWithLeft(t).
                    }
                else then
                    {
                    if evaluate getBalance(evaluate right(t)) is < 0 or evaluate getBalance(evaluate right(t)) is = 0 and balance is < -1 then
                        {
                        evaluate singleRotateWithRight(t).
                        }
                    else then
                        {
                        if balance is < -1 and evaluate getBalance(evaluate right(t)) is > 0 then
                            {
                            evaluate doubleRotateWithRight(t).
                            }
                        else then
                            {
                            t.
                            }.
                        }.
                    }.
                }.
            }.
        }.
    }.

declare displayAVL as function(t)
    {
    if t is = null then
        {
        0.
        }
    else then
        {
        display "Val: ".
        display evaluate data(t).
        if evaluate left(t) is = null then
            {
            0.
            }
        else then
            {
            display " Left: ".
            display evaluate data(evaluate left(t)).
            }.
        if evaluate right(t) is = null then
            {
            0.
            }
        else then
            {
            display " Right: ".
            display evaluate data(evaluate right(t)).
            }.
        display "\n".
        evaluate displayAVL(evaluate left(t)).
        evaluate displayAVL(evaluate right(t)).
        }.
    }.

--Basic inserts
declare someTree as evaluate insert(3, "c", null).
displayln "****tree after insert k: 3, v: \"c\":".
evaluate displayAVL(someTree).
change someTree to_be evaluate insert(5, "e", someTree).
displayln "****tree after insert k: 5, v: \"e\":".
evaluate displayAVL(someTree).
change someTree to_be evaluate insert(1, "a", someTree).
displayln "****tree after insert k: 1, v: \"a\":".
evaluate displayAVL(someTree).
change someTree to_be evaluate insert(22, "w", someTree).
displayln "****tree after insert k: 22, v: \"w\":".
evaluate displayAVL(someTree).
change someTree to_be evaluate insert(19, "t", someTree).
displayln "****tree after insert k: 19, v: \"t\":".
evaluate displayAVL(someTree).
change someTree to_be evaluate insert(2, "b", someTree).
displayln "****tree after insert k: 2, v: \"b\":".
evaluate displayAVL(someTree).
change someTree to_be evaluate insert(81, 2352, someTree).
displayln "****tree after insert k: 2, v: \"b\":".
evaluate displayAVL(someTree).
change someTree to_be evaluate insert(201, 11, someTree).
displayln "****tree after insert k: 2, v: \"b\":".
evaluate displayAVL(someTree).
change someTree to_be evaluate insert(235, -31, someTree).
displayln "****tree after insert k: 2, v: \"b\":".
evaluate displayAVL(someTree).
--Showing some finds
displayln evaluate find(2, someTree).
displayln evaluate find(19, someTree).
displayln evaluate find(235, someTree).
--Doing some deletes
change someTree to_be evaluate delete(someTree, 3).
displayln "**** tree after an arbitrary delete".
evaluate displayAVL(someTree).
change someTree to_be evaluate delete(someTree, 2).
displayln "**** tree after an arbitrary delete".
evaluate displayAVL(someTree).
change someTree to_be evaluate delete(someTree, 5).
displayln "**** tree after an arbitrary delete".
evaluate displayAVL(someTree).
change someTree to_be evaluate delete(someTree, 1).
displayln "**** tree after an arbitrary delete".
evaluate displayAVL(someTree).
change someTree to_be evaluate delete(someTree, 19).
displayln "**** tree after an arbitrary delete".
evaluate displayAVL(someTree).
change someTree to_be evaluate delete(someTree, 22).
displayln "**** tree after an arbitrary delete".
evaluate displayAVL(someTree).
