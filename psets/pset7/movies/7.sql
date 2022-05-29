SELECT title, rating FROM (movies INNER JOIN ratings ON id = movie_id) WHERE year = 2010 ORDER BY title;
