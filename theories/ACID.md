# Isolation


# Rollback

Transaction1                              | Transaction2
------------------------------------------------------------
BEGIN;                                    |
SELECT age FROM users WHERE id = 1;       |
-- retrieves 20                           |
                                          |
                                          |

# Commit of Update

# References
- https://en.wikipedia.org/wiki/Isolation_(database_systems)
