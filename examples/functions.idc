declare makeAccount as function(init, name)
    {
    declare balance as array of_size 1.
    set index 0 of balance as init.
    function(f)
        {
        if f is = "deposit" then
            {
            function(d)
                {
                display "Depositing ".
                display d.
                display " dollars into ".
                displayln name.
                set index 0 of balance as (index 0 of balance + d).
                }.
            }
        else then
            {
            if f is = "withdrawl" then
                {
                function(d)
                    {
                    display "Withdrawing ".
                    display d.
                    display " dollars from ".
                    displayln name.
                    set index 0 of balance as (index 0 of balance - d).
                    }.
                }
            else then
                {
                if f is = "balance" then
                    {
                    display "Current balance of ".
                    display name.
                    display " is ".
                    displayln index 0 of balance.
                    index 0 of balance.
                    }
                else then
                    {
                    if f is = "transform" then
                        {
                        function(t)
                            {
                            display "Transforming ".
                            display name.
                            display "balance to new balance of ".
                            displayln evaluate t(index 0 of balance).
                            set index 0 of balance as evaluate t(index 0 of balance).
                            }.
                        }
                    else then
                        {
                        display f.
                        displayln " is not a valid method on a bankAccount".
                        }.
                    }.
                }.
            }.
        }.
    }.

--Initializing account to 50
declare myAccount1 as evaluate makeAccount(50, "myAccount1").
declare myAccount2 as evaluate makeAccount(50, "myAccount2").
displayln evaluate myAccount1("balance").
declare deposit1 as evaluate myAccount1("deposit").
declare withdrawl1 as evaluate myAccount1("withdrawl").
declare deposit2 as evaluate myAccount2("deposit").
declare withdrawl2 as evaluate myAccount2("withdrawl").
--Making a deposit of 10 into account 1
evaluate deposit1(10).
evaluate myAccount1("balance").
--Making a deposit of 15 into account 2
evaluate deposit2(15).
evaluate myAccount2("balance").
--Making a withdrawl of 30
evaluate withdrawl1(30).
evaluate myAccount1("balance").
declare transform as evaluate myAccount1("transform").
--Making a withdrawl from account 2
evaluate withdrawl2(40).
evaluate myAccount2("balance").
--Making a function that returns x * 4 + 3
declare multBy4AndAdd3 as function(x)
    {
    ((x * 4) + 3).
    }.
--Transforming balance of account
evaluate transform(multBy4AndAdd3).
evaluate myAccount1("balance").
