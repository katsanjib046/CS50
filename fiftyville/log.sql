-- We can find License plates of the suspects by checking logs from bakery

SELECT *
FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND 15 <= minute ANd minute <= 25
AND activity = 'exit';

-- Lets look at the name of these people

SELECT name
FROM people
WHERE license_plate IN
(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND 15 <= minute ANd minute <= 25
    AND activity = 'exit'
)
ORDER BY name;

-- lets check the atm where the suspect withdrew some money

SELECT account_number
FROM atm_transactions
WHERE year=2021 AND month= 7 AND day=28
AND transaction_type= 'withdraw'
AND atm_location = 'Leggett Street';

-- lets check the bank_account

SELECT *
FROM bank_accounts
WHERE account_number IN
(
    SELECT account_number
    FROM atm_transactions
    WHERE year=2021 AND month= 7 AND day=28
    AND transaction_type= 'withdraw'
    AND atm_location = 'Leggett Street'
);

-- lets find the names of these people
SELECT name
FROM people
WHERE id IN
(
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN
    (
        SELECT account_number
        FROM atm_transactions
        WHERE year=2021 AND month= 7 AND day=28
        AND transaction_type= 'withdraw'
        AND atm_location = 'Leggett Street'
    )

) ORDER BY name;

-- Lets find the people who where both at the atm and also in the bakery's parking lot
SELECT name
FROM people
WHERE id IN
(
    SELECT person_id
    FROM bank_accounts
    WHERE account_number IN
    (
        SELECT account_number
        FROM atm_transactions
        WHERE year=2021 AND month= 7 AND day=28
        AND transaction_type= 'withdraw'
        AND atm_location = 'Leggett Street'
    )

)
AND license_plate IN
(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND 15 <= minute ANd minute <= 25
    AND activity = 'exit'
) ORDER BY name;

-- ################### Call Report #####################

-- Lets look at the names of callers and receivers

SELECT name
FROM people
WHERE phone_number IN
(
    SELECT caller
    FROM phone_calls
    WHERE year = 2021 AND month = 7 AND day = 28
    AND duration <= 60
) ORDER BY name;

-- Lets combine the result obtained so far

SELECT name
FROM people
WHERE people.id IN
(
    SELECT person_id
    FROM bank_accounts
    WHERE bank_accounts.account_number IN
    (
        SELECT account_number
        FROM atm_transactions
        WHERE year=2021 AND month= 7 AND day=28
        AND transaction_type= 'withdraw'
        AND atm_location = 'Leggett Street'
    )

)
AND people.license_plate IN
(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND 15 <= minute ANd minute <= 25
    AND activity = 'exit'
)
AND people.phone_number IN
(
    SELECT caller
    FROM phone_calls
    WHERE year = 2021 AND month = 7 AND day = 28
    AND duration <= 60
) ORDER BY name;

-- ############### Adding Flight Information ##################
-- Note that flight is on next day so day = 29
-- we see that first flight is to destination_id = 4 at 8.20
SELECT city
FROM airports
WHERE id IN
(
    SELECT destination_airport_id
    FROM flights
    WHERE year = 2021 AND month=7 AND day = 29 AND hour = 8
    AND origin_airport_id =
    (
        SELECT id
        FROM airports
        WHERE city='Fiftyville'
    )
);

-- The destination city is New York City
-- ############# People in the Flight #################
SELECT name
FROM people
WHERE passport_number IN
(
    SELECT passport_number
FROM passengers
WHERE flight_id =
(
    SELECT id
    FROM flights
    WHERE year = 2021 AND month=7 AND day = 29 AND hour = 8
    AND origin_airport_id =
    (
        SELECT id
        FROM airports
        WHERE city='Fiftyville'
    )
)
);

-- ############## Who is common in both the list ###################
SELECT name
FROM people
WHERE passport_number IN
(
    SELECT passport_number
    FROM passengers
    WHERE flight_id =
    (
        SELECT id
        FROM flights
        WHERE year = 2021 AND month=7 AND day = 29 AND hour = 8
        AND origin_airport_id =
        (
            SELECT id
            FROM airports
            WHERE city='Fiftyville'
        )
    )
)
AND people.id IN
(
    SELECT person_id
    FROM bank_accounts
    WHERE bank_accounts.account_number IN
    (
        SELECT account_number
        FROM atm_transactions
        WHERE year=2021 AND month= 7 AND day=28
        AND transaction_type= 'withdraw'
        AND atm_location = 'Leggett Street'
    )

)
AND people.license_plate IN
(
    SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND 15 <= minute ANd minute <= 25
    AND activity = 'exit'
)
AND people.phone_number IN
(
    SELECT caller
    FROM phone_calls
    WHERE year = 2021 AND month = 7 AND day = 28
    AND duration <= 60
);
-- The thief is Bruce

-- ###### Accomplice #################
-- Check the corresponding phone number
SELECT name
FROM people
where phone_number IN
(
SELECT receiver
FROM phone_calls
WHERE phone_calls.caller IN
(
    SELECT phone_number
    FROM people
    WHERE passport_number IN
    (
        SELECT passport_number
        FROM passengers
        WHERE flight_id =
        (
            SELECT id
            FROM flights
            WHERE year = 2021 AND month=7 AND day = 29 AND hour = 8
            AND origin_airport_id =
            (
                SELECT id
                FROM airports
                WHERE city='Fiftyville'
            )
        )
    )
    AND people.id IN
    (
        SELECT person_id
        FROM bank_accounts
        WHERE bank_accounts.account_number IN
        (
            SELECT account_number
            FROM atm_transactions
            WHERE year=2021 AND month= 7 AND day=28
            AND transaction_type= 'withdraw'
            AND atm_location = 'Leggett Street'
        )

    )
    AND people.license_plate IN
    (
        SELECT license_plate
        FROM bakery_security_logs
        WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND 15 <= minute ANd minute <= 25
        AND activity = 'exit'
    )
    AND people.phone_number IN
    (
        SELECT caller
        FROM phone_calls
        WHERE year = 2021 AND month = 7 AND day = 28
        AND duration <= 60
    )
)
AND year = 2021 AND month = 7 AND day = 28
        AND duration <= 60
);
