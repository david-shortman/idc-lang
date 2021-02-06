--Showing various basic manipulations
declare someArray as array of_size 5.
displayln someArray.
set index 3 of someArray as "no way".
set index 4 of someArray as function(x) { display 3.}.
set index 0 of someArray as 15.
displayln index 3 of someArray.
displayln index 4 of someArray.
declare displayer as function(x)
  {
  displayln x.
  }.
evaluate displayer(index 4 of someArray).
displayln someArray.

--Making some arbitrary array
declare anotherArray as array of_size 10.
declare i as 0.
while i is < 10 then
    {
    set index i of anotherArray as ((i - 1) * (5 - i)).
    if i is > 1 and (i % 2) is = 0 then
        {
        declare temp as index (i - 1) of anotherArray.
        set index (i - 1) of anotherArray as index i of anotherArray.
        set index i of anotherArray as temp.
        }.
    declare i as (i + 1).
    }.
displayln anotherArray.

--Selection sort on the arbitrary array
declare sort as function(a, size)
    {
    declare j as 0.
    while j is < (size - 1) then
        {
        declare min as j.
        declare i as (j + 1).
        while i is < size then
            {
            if index i of a is < index min of a then
                {
                declare min as i.
                }.
            declare i as (i + 1).
            }.
        if min is < j or min is > j then
            {
            declare temp as index j of a.
            set index j of a as index min of a.
            set index min of a as temp.
            }.
        declare j as (j + 1).
        }.
    }.

evaluate sort(anotherArray, 10).
displayln anotherArray.
