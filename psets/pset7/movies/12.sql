SELECT title AS "Movies Starring Johnny Depp & Helena Bonham Carter" FROM movies
    WHERE id IN 
    (
        SELECT movie_id FROM stars WHERE person_id = 
            (SELECT id FROM people WHERE name = "Johnny Depp")
        AND movie_id IN
        (SELECT movie_id FROM stars WHERE person_id = 
            (SELECT id FROM people WHERE name = "Helena Bonham Carter"))
    )                
GROUP BY title 
