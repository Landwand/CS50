SELECT DISTINCT p.name
FROM people p
WHERE p.id IN

    (SELECT d.person_id from directors d
    WHERE movie_id IN

        (SELECT r.movie_id FROM ratings r
        WHERE r.rating >=9.0))
;