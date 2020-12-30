To implement a counter column, create a table that only includes:
* The primary key (can be one or more columns)
* The counter column
* Counter columns cannot be primary key columns.

Cannot SET
* You can only make increment or decrement, cannot set its value
* You can
  * UPDATE table SET CTR=CTR+2 WHERE id=...
* You CANNOT
  * UPDATE table SET CTR=2 WHERE id=...

* Can have more than one counter column
