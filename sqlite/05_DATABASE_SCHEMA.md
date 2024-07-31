# Where
- In `page 1`

# Structure
- The structure of the `sqlite_schema` table is as if it had been created using the following SQL:
```
CREATE TABLE sqlite_schema(
    type text,
    name text,
    tbl_name text,
    rootpage integer,
    sql text
);
```

# Data
- The `sqlite_schema` table contains **one row** for each `table`, `index`, `view`, and `trigger` (collectively "objects") in the database schema, except there is no entry for the sqlite_schema table itself.
- The `sqlite_schema` table contains entries for internal schema objects in addition to application- and programmer-defined objects.
- The `tbl_name` column holds the name of a table or view that the object is associated with.
  - For a table or view, the `tbl_name` column is a copy of the name column. For an index, the tbl_name is the name of the table that is indexed.
  - For a trigger, the `tbl_name` column stores the name of the table or view that causes the trigger to fire.
- The `rootpage` column stores the page number of the **root** b-tree page for tables and indexes.
