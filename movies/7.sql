SELECT title, rating
FROM movies INNER JOIN ratings
ON movies.id = ratings.movie_id
AND id IN (SELECT id FROM movies WHERE year = 2010) ORDER BY rating DESC, title ASC;