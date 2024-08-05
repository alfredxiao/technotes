# Isolation


# Rollback

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
# Commit of Update

# References
- https://en.wikipedia.org/wiki/Isolation_(database_systems)
