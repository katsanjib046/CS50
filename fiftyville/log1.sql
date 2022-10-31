-- Keep a log of any SQL queries you execute as you solve the mystery.
-- First of all lets go to crime_scene_reports table and look into the particular day july 28, 2021

SELECT * FROM crime_scene_reports WHERE year = 2021 AND month = 7 AND day = 28;

-- Result shows that theft took place at Humphrey Street bakery at 10:15 am, 3 witnesses,
-- all of them mention bakery (id = 295)
-- Lets check the interview of the witnesses for this event

SELECT * FROM interviews WHERE year = 2021 AND month = 7 ANd day = 28;

-- Result shows following things:
-- 1. Ruth say the thief get into car in the bakery parking lot within 10 minutes, so look into the parking lot footage
-- 2. Eugene recognized the thief who they saw in the ATM on Leggett Street withdrawing some money
-- 3. Raymond said, the thief called someone and talked for less than a minute, The flight out was on the next day.
-- The other person (accomplice) on the phone was asked to purchase the ticket
-- Let's begin with the parking lot video to recognize the thief

SELECT * FROm bakery_security_logs WHERE year = 2021
AND month = 7 AND day = 28 AND hour = 10 AND 15 <= minute ANd minute <= 25
AND activity = 'exit';

-- From the result, 8 license_plate where observed. One of these should belong to the thief
-- Lets look at the atm now

SELECT * FROM atm_transactions WHERE year=2021 AND month= 7 AND day=28
AND transaction_type= 'withdraw' ANd atm_location = 'Leggett Street';

-- The result provides us all the account numbers in that location from which money was withdrawn
-- Lets try to find people whose license number and bank account number matches the results from above

SELECT * FROM bank_accounts WHERE account_number IN
(SELECT account_number FROM atm_transactions WHERE year=2021 AND month= 7 AND day=28
AND transaction_type= 'withdraw' ANd atm_location = 'Leggett Street');

-- We get people's id from this who withdrew money from Leggett Street that particular day
-- We can use those ids to check for whose license was also in the parking lot

SELECT name FROM people WHERE id IN
(
    SELECT person_id FROM bank_accounts WHERE account_number IN
    (
        SELECT account_number FROM atm_transactions WHERE year=2021 AND month= 7 AND day=28
        AND transaction_type= 'withdraw' ANd atm_location = 'Leggett Street'
    )
)
AND license_plate IN
(
    SELECT license_plate FROM bakery_security_logs WHERE year = 2021
    AND month = 7 AND day = 28 AND hour = 10 AND 15 <= minute ANd minute <= 25
    AND activity = 'exit'
);

-- WE got name of four people (Iman, Luca, Diana, Bruce) instead of one so this got complicated now.
-- Of these four people let's look at who made a short phone call around the time of theft

SELECT * FROM phone_calls WHERE year=2021 AND month=7 AND day=28 and duration <= 60;

-- So we have got both the names of callers and receivers who made a less-than-1-min call in the time frame of crime
-- Next we match the caller's name with the names we found earlier
SELECT name FROM people WHERE phone_number IN
(
    SELECT caller FROM phone_calls WHERE year=2021 AND month=7 AND day=28 and duration <= 60
)
AND name IN
(
    SELECT name FROM people WHERE id IN
    (
        SELECT person_id FROM bank_accounts WHERE account_number IN
        (
            SELECT account_number FROM atm_transactions WHERE year=2021 AND month= 7 AND day=28
            AND transaction_type= 'withdraw' ANd atm_location = 'Leggett Street'
        )
    )
    AND license_plate IN
    (
        SELECT license_plate FROM bakery_security_logs WHERE year = 2021
        AND month = 7 AND day = 28 AND hour = 10 AND 15 <= minute ANd minute <= 25
        AND activity = 'exit'
    )
    );
-- So far we have got two suspects, Diana and Bruce.
-- Next we look at who flew the next day out of the town.
-- Lets check the airports

SELECT * from airports WHERE city = 'Fiftyville';

-- abbreviation of the airport is CSF and id is 8.
-- Lets look at the flights from the airport.

SELECT * from flights WHERE year=2021 AND month=7 AND day=29
AND origin_airport_id =
(
    SELECT id from airports WHERE city = 'Fiftyville'
);


-- Lets find what place is that
SELECT city FROM airports WHERE id =
(
    SELECT destination_airport_id from flights WHERE year=2021 AND month=7 AND day=29
    ANd hour = 8
    AND origin_airport_id =
(
    SELECT id from airports WHERE city = 'Fiftyville'
)
);
-- The thief escaped to Dallas
-- Lets find who flew to Dallas now by looking into the passengers details
SELECT * FROM passengers WHERE flight_id =
(SELECT id from flights WHERE year=2021 AND month=7 AND day=29
ANd hour = 8
AND origin_airport_id =
(SELECT id from airports WHERE city = 'Fiftyville'));
-- Lets look into who these people are
SELECT * from people WHERE passport_number IN
(SELECT passport_number FROM passengers WHERE flight_id =
(SELECT id from flights WHERE year=2021 AND month=7 AND day=29
ANd hour = 8
AND origin_airport_id =
(SELECT id from airports WHERE city = 'Fiftyville')));
