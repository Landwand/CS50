SELECT AVG(rating) from ratings
WHERE movie_id IN

(
    SELECT id FROM movies
    where year = 2012
);