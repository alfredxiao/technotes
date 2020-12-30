secondary index
* An index on a column that allows a table to be queried on a column that is usually prohibited
  * table structure not affected
* Can be created on any column including collections except counter and static columns

Storage
* secondary index data stored on nodes hosting relevant partitions
* A query requires scanning all nodes' local index data is NOT efficient
* A query with partition key scans only one node's local index data and IS efficient

How it works
* Coordinator sends request to ALL nodes - when partition key not known
  * Technically not ALL nodes are required due to replication
* Each node searches its local index - returns results to Coordinator

When to use secondary index
* Low cardinality columns
* For search on both a partition key and an indexed column in large partition

When NOT to use secondary index
* High cardinality columns
* Tables with COUNTER column
* Frequently updated or deleted columns

Two types of Secondary Index
* Regular
  * Uses hashtable to index data and supports `=` only
* SSTable-Attached Secondary Index (SASI)
  * Uses B+ tree, and supports `=` as well as range query, and `LIKE`

```
DROP KEYSPACE IF EXISTS myschema;

CREATE KEYSPACE myschema WITH replication = {'class': 'SimpleStrategy', 'replication_factor': 1};
CREATE TABLE myschema.users (
   userID uuid,
   fname text,
   lname text,
   email text,
   address text,
   zip int,
   state text,
   PRIMARY KEY (userID)
 );

CREATE INDEX user_state
   ON myschema.users (state);

CREATE INDEX ON myschema.users (zip);
```


materialised view
* a database object that stores query results
* Has a new primary key and new properties
* suited for high cardinality data

Primary key
* Source table's primary key MUST be part of materialised view's primary key
* AT MOST ONE new column can be added to the materialised view's primary key
* All view primary key columns must be restricted to not allow nulls.

Caveats
* Data can only be written to source table, not materialised view
* Updates from source table to materialised view is asynchronous and come with delay
* No base table static column can be included in a view;
* Cassandra does not provide a way to automatically detect and fix such inconsistencies other than dropping and recreating the materialized view, which is not an ideal solution in production:
