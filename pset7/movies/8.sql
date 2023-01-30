SELECT p.name FROM people p
WHERE p.id IN
(SELECT person_id FROM stars s
WHERE s.movie_id IN

(SELECT m.id FROM movies m
WHERE m.title = "Toy Story"))
;