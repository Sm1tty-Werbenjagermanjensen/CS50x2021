SELECT DISTINCT name AS "People who have starred in a movie with Kevin Bacon" FROM people WHERE id IN 
    (SELECT person_id FROM stars WHERE movie_id IN
        (SELECT movie_id FROM stars WHERE person_id =
            (SELECT id FROM people WHERE name = "Kevin Bacon" AND birth = 1958)))
AND id != (SELECT id FROM people WHERE name = "Kevin Bacon" AND birth = 1958);
