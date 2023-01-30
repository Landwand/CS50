
SELECT p.name FROM people p
WHERE p.id IN

    (SELECT s.person_id FROM stars s
    WHERE s.movie_id IN

        (SELECT s.movie_id FROM stars s
        WHERE s.person_id IN
            (SELECT p.id
            FROM people p
            WHERE name = "Kevin Bacon"
            AND
            birth = 1958
            ))
    )
AND
NOT
p.name = "Kevin Bacon"
;
