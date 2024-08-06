# Isolation

The execution of concurrent SQL-transactions at isolation level `SERIALIZABLE` is guaranteed to be serializable. A serializable execution is defined to be an execution of the operations of **concurrently executing** SQL-transactions that produces **the same effect as some serial execution** of those same SQL-transactions. A serial execution is one in which each SQL-transaction executes to completion before the next SQL-transaction begins.

## Baseline
```
users
id 	name 	  age
1 	Alice 	20
2 	Bob 	  25
```

## Dirty Read

1. Transaction1
```SQL
BEGIN;
SELECT age FROM users WHERE id = 1;
-- retrieves 20
```
2. Transaction2
```SQL
    BEGIN;
    UPDATE users SET age = 21 WHERE id = 1;
    -- no commit here
```
3. Transaction1
```SQL
SELECT age FROM users WHERE id = 1;
-- READ UNCOMMITTED retrieves 21 (dirty read)
-- READ COMMITTED retrieves 20 (dirty read has been avoided)
-- REPEATABLE READ retrieves 20 (dirty read has been avoided)
-- SERIALIZABLE retrieves 20 (dirty read has been avoided)
COMMIT;
```
4. Transaction2
```SQL
    ROLLBACK;
```

## Non-Repeatable Reads

1. Transaction1
```SQL
BEGIN;
SELECT age FROM users WHERE id = 1;
-- retrieves 20
```
2. Transaction2
```SQL
    BEGIN;
    UPDATE users SET age = 21 WHERE id = 1;
    COMMIT;
```
3. Transaction1
```SQL
SELECT age FROM users WHERE id = 1;
-- READ UNCOMMITTED retrieves 21 (non-repeatable read)
-- READ COMMITTED retrieves 21 (non-repeatable read)
-- REPEATABLE READ retrieves 20 (non-repeatable read has been avoided)
-- SERIALIZABLE retrieves 20 (non-repeatable read has been avoided)
COMMIT;
```

## Phantom reads

1. Transaction1
```SQL
BEGIN;
SELECT name FROM users WHERE age > 17;
-- retrieves Alice and Bob
```
2. Transaction2
```SQL
    BEGIN;
    INSERT INTO users VALUES (3, 'Carol', 26);
    COMMIT;
```
3. Transaction1
```SQL
SELECT name FROM users WHERE age > 17;
-- READ UNCOMMITTED retrieves Alice, Bob and Carol (phantom read)
-- READ COMMITTED retrieves Alice, Bob and Carol (phantom read)
-- REPEATABLE READ retrieves Alice, Bob and Carol (phantom read)
-- SERIALIZABLE retrieves Alice and Bob (phantom read has been avoided)
COMMIT;
```



# References
- https://en.wikipedia.org/wiki/Isolation_(database_systems)
