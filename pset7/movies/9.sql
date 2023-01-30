SELECT DISTINCT p.name
FROM people p
WHERE p.id IN

(SELECT person_id from stars s
WHERE s.movie_id IN

(SELECT m.id FROM movies m
WHERE year = 2004))

ORDER BY p.birth;