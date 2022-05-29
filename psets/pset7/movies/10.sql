SELECT DISTINCT name AS "Directors with a >= 9.0 rating" FROM people
WHERE id IN (SELECT person_id FROM directors 
    WHERE movie_id IN (SELECT movie_id FROM ratings WHERE rating >= 9.0));
