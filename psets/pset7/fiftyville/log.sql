-- Keep a log of any SQL queries you execute as you solve the mystery.



-- First I looked for a crime scene report matching the date and location of the crime:
SELECT id, description AS "Crime Scene Report"  FROM crime_scene_reports
WHERE year = 2020 
AND month = 7 
AND day = 28 
AND street = "Chamberlin Street";

-- Theft took place at 10:15am, at Chamberlin Street courthouse. 3 witness testimonies were taken.
-- I then looked up the witness transcripts: 
SELECT id, name, transcript AS "interview" FROM interviews
WHERE year = 2020 
AND month = 7 
AND day = 28;
-- Witnesses each give a lead to follow: 

-- Ruth states the theif left sometime within 10 minutes of the theft (between 10:15 and 10:25) they got into a car and drove away. Thus, I can grab their license plate from the court house security footage table.

-- Eugene states they recognized the theif from an ATM on "Fifer Street earlier that morning where the theif was withdrawing money.

-- Raymond states as the theif left, they called someone for less than a minute (the accomplice). The theif said they will take the earliest flight out of the fiftyville the next day. They also asked for the accomplice to purchase the flight ticket.


-- The first lead I followed up was the security footage.
-- I looked at the court house security logs on the 28th from 10:15 AM to 10:25 AM for the license plates of exiting cars.
SELECT id, license_plate, hour, minute FROM courthouse_security_logs
WHERE year = 2020 
AND month = 7 
AND day = 28 
AND hour = 10 
AND minute > 15 
AND minute < 25 
AND activity = "exit";
-- This produced 9 license plates. Thus, on its own doesn't help, as no single plate can be linked to the theif. 
-- However, it immensly narrows down the number of possible license plates the theif may have. 
-- Which will make this selection possibly useful for cross examination later. 


-- I next looked at phone records.
-- Since the phone call to the accomplice was reported as being less than one minute, I looked for any calls on the day of the theft that were less than 60 seconds.
SELECT id, caller, receiver, duration FROM phone_calls
WHERE year = 2020 
AND month = 7 
AND day = 28 
AND duration < 60;
-- This also produced 9 results. Thus, again on it's own not able to pin point the theif or accomplice. 
-- Yet, it greatly narrows down the number of possible phone numbers for both the theif and accomplice, also creating a link between the two. 
-- Since, if I later confirm either the theif or accomplices phone number through other means. I can then look to this selection to find the corresponding number of the other criminal. 
-- As a result, like the license plates, this too may later be useful for cross examination.
-- It is important to note that the witness Raymond stated the theif was the one who "called". If this is accurate. All callers in this selection correspond to possible theifs and all recievers are possible accomplices.


-- Following the phone records I checked atm transactions.
-- I specifically checked atm withdrawls on Fifer Street on the 28th. As this corresponds with Eugenes testimony.
SELECT * FROM atm_transactions
WHERE year = 2020 
AND month = 7 
AND day = 28 
AND atm_location = "Fifer Street" 
AND transaction_type = "withdraw";
-- This produced 8 transactions. One of these supposedly are the theif, yet on it's own this cannot pin point them. However this selection of account numbers may be usful for cross examination.

-- I now wanted to find a list of the passengers on the earliest flight out of the city the day after the theft
-- As this is the flight the theif was planning to take according to Raymond's testimony.
-- Therefore, the theif would be a passenger on this plane.
SELECT * FROM passengers
WHERE flight_id = (SELECT id FROM flights
    WHERE year = 2020 
    AND month = 7 
    AND day = 29
    AND origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville")
    ORDER BY hour, minute LIMIT 1);
-- This prodcued 7 passengers. These passports can be used to help in cross examination.



--FINDING THEIF---------------------------------------------------------------------------------------------
-- I now culminated these factors in a query to try to single out a single person who correlated to all the selections of possible theifs.
SELECT name AS "Theif:" FROM people
-- Has a bank account that was used on Fifer Street:
WHERE id IN (SELECT person_id FROM bank_accounts 
    WHERE account_number IN (SELECT account_number FROM atm_transactions 
        WHERE year = 2020 
        AND month = 7 
        AND day = 28 
        AND atm_location = "Fifer Street" 
        AND transaction_type = "withdraw"))

-- Made a call less than a minute between 10:15 AM and 10:25 AM on the 28th:
AND phone_number IN (SELECT caller FROM phone_calls
    WHERE year = 2020 
    AND month = 7 
    AND day = 28 
    AND duration < 60)

-- Was on the earliest flight outbound on the 29th: 
AND passport_number IN (SELECT passport_number FROM passengers
    WHERE flight_id = 
        (SELECT id FROM flights
        WHERE year = 2020 
        AND month = 7 
        AND day = 29
        AND origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville")
        ORDER BY hour, minute LIMIT 1))

-- Same license plate as car leaving scene of crime:
AND license_plate IN (SELECT license_plate FROM courthouse_security_logs
WHERE year = 2020 AND month = 7 AND day = 28 AND hour = 10 AND minute > 15 AND minute < 25 AND activity = "exit");

--Ernest is the only suspect that returns from this deduction, concluding he is the theif.

--FINDING CITY THE THEIF ESCAPED TO----------------------------------------------------------------------
SELECT city AS "City thief escaped to:" FROM airports 
    WHERE id = (SELECT destination_airport_id FROM flights 
        WHERE id = 
            (SELECT id FROM flights
            WHERE year = 2020 
            AND month = 7 
            AND day = 29
            AND origin_airport_id = (SELECT id FROM airports WHERE city = "Fiftyville")
            ORDER BY hour, minute LIMIT 1));

-- The destination of the earliest flight on the 29th (i.e the flight the theif (Ernest) and his accomplice were on) is the city the theif escaped to.

-- This city is revealed to be: London.

--FINDING ACCOMPLICE-------------------------------------------------------------------------------------------
SELECT name AS "Accomplice:" FROM people 
-- Recieved phone call between 10:15 AM and 10:25 AM from the theif (a.k.a Ernest)
WHERE phone_number IN 
    (SELECT receiver FROM phone_calls
        WHERE year = 2020 
        AND month = 7 
        AND day = 28 
        AND duration < 60 
        AND caller = (SELECT phone_number FROM people WHERE name = "Ernest"));

-- Berthold is the only suspect that returns from this deduction, concluding they are the accomplice.
