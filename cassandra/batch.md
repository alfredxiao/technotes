Individual DML
* In Cassandra, individual insert, update and delete operations are atomic and isolated, meaning that either an operation occurs or nothing occurs (atomicity) and any partially updated data is not visible (isolated) to other operations.
* To achieve atomicity for a set of operations, Cassandra provides atomic batches.

Single Partition
* A single-partition batch is an atomic batch where all operations work on the same partition and that, under the hood, can be executed as a single write operation
* As a result, single-partition batches guarantee both all-or-nothing atomicity and isolation. The main use case for single-partition batches is updating related data that may become corrupt unless atomicity is enforced.

Multi-Partitions
* A multi-partition batch is an atomic batch where operations work on different partitions that belong to the same table or different tables.
* Multi-partition batches only guarantee atomicity.
* Their main use case is updating the same data duplicated across multiple partitions due to denormalization.
* Atomicity ensures that all duplicates are consistent.

single partition vs multiple partition
* Can achieve atomicity and isolation when targeting a single partition
  * Single partition batch operations are atomic automatically,
* or only atomicity when targeting multiple partitions.
  * while multiple partition batch operations require the use of a batchlog to ensure atomicity.

Counter
* NOT idempotent
* Counter batches cannot include non-counter columns in the DML statements, just as a non-counter batch cannot include counter columns.
* Counter batch statements cannot provide custom timestamps.
```
BEGIN COUNTER BATCH
```

Performance
* BATCH does NOT increase performance (or rarely)
* Single-partition batches are quite efficient and can performance better than individual statements because batches save on client-coordinator and coordinator-replicas communication. However, sending a large batch with hundreds of statements to one coordinator node can also negatively affect workload balancing.
* Multi-partition batches are substantially more expensive as they require maintaining a batchlog in a separate Cassandra table. Therefore, even with respect to the main use case of updating the same data duplicated across multiple partitions due to denormalization, use multi-partition batches only when atomicity is truly important for your application. 

Ordering
* There is NO ordering for operations in a batch
  * They are executed with the same timestamp
* Your batch queries should NOT rely on order of statements

logged/unlogged
* Unlogged batches break the atomicity guarantee and counter batches are not safe to replay automatically as counter updates are not idempotent.

WITH TIMESTAMP
* Same timestamp for all operations
  * BEGIN BATCH USING TIMESTAMP 1481124356754405
* Apply a timestamp to an individual operation DML
  ```
  BEGIN BATCH
    DML_statement1 ;
    DML_statement2 USING TIMESTAMP [ epoch_microseconds ] ;
    DML_statement3 ;
  APPLY BATCH ;
```

lightweight Transaction
* Single-partition batches can even contain lightweight transactions, but multi-partition batches cannot.
* If one statement in a batch is a conditional update, the conditional logic must return true, or the entire batch won't be applied.
* If the batch contains two or more conditional updates, all the conditions must return true, or the entire batch won't be applied.
```
BEGIN BATCH

  INSERT INTO cycling.cyclist_expenses (
    cyclist_name, expense_id
  ) VALUES (
    'Joe WALLS', 9
  ) IF NOT EXISTS;

  INSERT INTO cycling.cyclist_expenses (
    cyclist_name, expense_id, amount, description, paid
  ) VALUES (
    'Joe WALLS', 10, 13, 'burrito', false
  );

APPLY BATCH;
```
