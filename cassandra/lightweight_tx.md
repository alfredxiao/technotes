CAS

* Essentially ACID at partition level

INSERT INTO ....
IF NOT EXISTS;

UPDATE ...
SET ...
WHERE ...
IF col=123

* lightweight transactions are also sometimes referred to as compare-and-set operations.
* Each lightweight transaction is atomic and always works on a single partition.
