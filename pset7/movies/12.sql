SELECT m.title
FROM movies m
WHERE m.id IN

    (SELECT s.movie_id FROM stars s
    WHERE s.person_id IN
        (SELECT p.id FROM people p
        WHERE name = "Johnny Depp")
    )

AND m.id IN

    (SELECT s.movie_id FROM stars s
    WHERE s.person_id IN
        (SELECT p.id FROM people p
        WHERE name = "Helena Bonham Carter")
    )
;