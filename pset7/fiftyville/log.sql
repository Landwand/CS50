-- Keep a log of any SQL queries you execute as you solve the mystery.

-- read crime scene reports
SELECT * FROM crime_scene_reports
WHERE year = 2021
    AND month = 07
    AND day = 28
    AND street LIKE "%Humphrey%"
    AND description LIKE "%duck%"
    ;
--- theft at 10:15 am.  3 witnesses, transcript mentions bakery




SELECT * FROM bakery_security_logs
    WHERE
    year = 2021
    AND month = 07
    AND day = 28
    AND hour = 10
    AND minute BETWEEN 05 AND 25
    ;
-- shows entrance/exit of license plates at barkery along with day, and time

-- +-----+------+-------+-----+------+--------+----------+---------------+
-- | id  | year | month | day | hour | minute | activity | license_plate |
-- +-----+------+-------+-----+------+--------+----------+---------------+
-- | 258 | 2021 | 7     | 28  | 10   | 8      | entrance | R3G7486       |
-- | 259 | 2021 | 7     | 28  | 10   | 14     | entrance | 13FNH73       |


SELECT * FROM interviews
    WHERE
    year = 2021
    AND month = 07
    AND day = 28
    AND
        (
            transcript LIKE "%bakery%"
            OR
            transcript LIKE "%theft%"
            OR
            transcript LIKE "%duck%"
        )
    ;

-- Ruth: look for cars leaving within 10 min of theft
-- Eugene: recognized, but doesn't know thief. Saw thief w/d $ from ATM
-- Raymond: as thief was leaving, they called someone and talked for < 1 min. Thief said: planning to take 1st flight out of Fiftyville tomorrow (29). Asked their collab. to buy a ticket.



-- use WITHDRAWAL from ATM on that date and correlate that back to bank-account and then bring that back to the PEOPLE table.
SELECT
    b.person_id, ppl.name, ppl.phone_number, ppl.passport_number
    FROM atm_transactions a
    JOIN bank_accounts b
        ON a.account_number = b.account_number
    JOIN people ppl
        ON ppl.id = b.person_id
    JOIN bakery_security_logs bsl
        ON ppl.license_plate = bsl.license_plate -- witnesses seen car drive away within 10 min of theft

    WHERE
        a.year = 2021
    AND a.month = 07
    AND
        a.day = 28
        -- (a.day = 28 or a.day = 29)
    AND a.atm_location LIKE "%Leggett%"
    AND a.transaction_type = "withdraw"  -- witnesses saw thief wd money on day-of
    AND bsl.year = 2021
    AND bsl.month = 07
    AND bsl.day = 28
    AND bsl.activity = "exit"
    AND bsl.hour = 10
    AND bsl.minute BETWEEN 05 AND 25
    ;

-- +-----------+--------+----------------+-----------------+
-- | person_id |  name  |  phone_number  | passport_number |
-- +-----------+--------+----------------+-----------------+
-- | 686048    | Bruce  | (367) 555-5533 | 5773159633      |
-- | 514354    | Diana  | (770) 555-1861 | 3592750733      |
-- | 396669    | Iman   | (829) 555-5269 | 7049073643      |
-- | 467400    | Luca   | (389) 555-5198 | 8496433585      |
-- | 449774    | Taylor | (286) 555-6063 | 1988161715      |



-- all passengers on next-day flight
SELECT *
    FROM people ppl
    JOIN flights f
    ON f.id = pass.flight_id
    JOIN passengers pass
    ON
        ppl.passport_number = pass.passport_number
    WHERE
        f.year = 2021
        AND f.month = 07
        AND f.day = 29
         -- witness said they heard thief call someone, asking them to book flight ticket for NEXT day (29) @ earliest time
        --AND ppl.passport_number = 4001449165
    ORDER BY f.hour, f.minute ASC
    ;



-- find earliest flight out of Fiftyville the day after theft
SELECT
*
FROM
flights f
JOIN passengers pass
ON f.id = pass.flight_id
JOIN people ppl
ON pass.passport_number = ppl.passport_number
WHERE f.origin_airport_id IN
(
    SELECT id
    FROM airports air
    WHERE air.city LIKE "%fiftyville%"
)
AND f.year = 2021
AND f.month = 07
AND f.day = 29 -- day after theft
ORDER BY
    f.hour, f.minute ASC
;


-- use Flight 36 and find destination city.

SELECT f.destination_airport_id
FROM
flights f
JOIN passengers pass
ON f.id = pass.flight_id
JOIN people ppl
ON pass.passport_number = ppl.passport_number

WHERE f.origin_airport_id IN
(
    SELECT id
    FROM airports air
    WHERE air.city LIKE "%fiftyville%"
)
AND f.year = 2021
AND f.month = 07
AND f.day = 29 -- day after theft
AND f.id = 36
;

-- earlist flight is #36 ; plug this in.


SELECT ppl.name
FROM
flights f
JOIN passengers pass
ON f.id = pass.flight_id
JOIN people ppl
ON pass.passport_number = ppl.passport_number

WHERE f.origin_airport_id IN
(
    SELECT id
    FROM airports air
    WHERE air.city LIKE "%fiftyville%"
)
AND f.year = 2021
AND f.month = 07
AND f.day = 29 -- day after theft
AND f.id = 36

-- +--------+
-- |  name  |
-- +--------+
-- | Doris  |
-- | Sofia  |
-- | Bruce  |
-- | Edward |
-- | Kelsey |
-- | Taylor |
-- | Kenny  |
-- | Luca   |

INTERSECT -- link withdrawal to bakery license plate, to people

    SELECT
    --b.person_id, ppl.name, ppl.phone_number, ppl.passport_number
    ppl.name
    FROM atm_transactions a
    JOIN bank_accounts b
        ON a.account_number = b.account_number
    JOIN people ppl
        ON ppl.id = b.person_id
    JOIN bakery_security_logs bsl
        ON ppl.license_plate = bsl.license_plate -- witnesses seen car drive away within 10 min of theft
    WHERE
        a.year = 2021
    AND a.month = 07
    AND
        a.day = 28
        -- (a.day = 28 or a.day = 29)
    AND a.atm_location LIKE "%Leggett%"
    AND a.transaction_type = "withdraw"  -- witnesses saw thief wd money on day-of
    AND bsl.year = 2021
    AND bsl.month = 07
    AND bsl.day = 28
    AND bsl.hour = 10
    AND bsl.minute BETWEEN 05 AND 25

INTERSECT -- add limit of <1 min call

    SELECT ppl.name
    FROM phone_calls ph
    JOIN people ppl
    ON (ph.caller = ppl.phone_number)
    WHERE
        ph.year = 2021
        AND
        ph.month = 07
        AND
        ph.day = 28
        AND
        ph.duration < 60
    ;

-- +-------+
-- | name  |
-- +-------+
-- | Bruce |


======

-- find his accomplice.

SELECT
*
FROM phone_calls pc
JOIN people ppl
ON ppl.phone_number = pc.receiver

WHERE pc.caller IN

(
    SELECT
    ppl.phone_number
    FROM
    people ppl
    WHERE ppl.name = "Bruce"
)

AND pc.year = 2021
AND pc.day = 28
AND pc.month = 07
AND pc.duration < 60

;

-- +-----+----------------+----------------+------+-------+-----+----------+--------+-------+----------------+-----------------+---------------+
-- | id  |     caller     |    receiver    | year | month | day | duration |   id   | name  |  phone_number  | passport_number | license_plate |
-- +-----+----------------+----------------+------+-------+-----+----------+--------+-------+----------------+-----------------+---------------+
-- | 233 | (367) 555-5533 | (375) 555-8161 | 2021 | 7     | 28  | 45       | 864400 | Robin | (375) 555-8161 |                 | 4V16VO0       |
-- +-----+----------------+----------------+------+-------+-----+----------+--------+-------+----------------+-----------------+---------------+



--- Find destination

SELECT city
FROM airports
WHERE id =
(
    SELECT destination_airport_id
    FROM flights
    WHERE id =
        (
        SELECT id FROM flights
        WHERE year = 2021
        AND month = 07
        AND day = 29
        AND origin_airport_id =
            (
            SELECT id
            FROM airports
            WHERE city = "Fiftyville"
            )
        ORDER BY hour, minute
        LIMIT 1
        )
)
    ;

-- +----+--------------+-------------------+---------------+
-- | id | abbreviation |     full_name     |     city      |
-- +----+--------------+-------------------+---------------+
-- | 4  | LGA          | LaGuardia Airport | New York City |
-- +----+--------------+-------------------+---------------+