Memtable -> Bloom Filter -> Key Cache -> Partition Summary -> Partition Index (DISK) -> Compression Offset -> Data (DISK)
                                      -> Compression Offset
Row Cache
* Enabled using CQL:
  * `CREATE TABLE… ‘WITH CACHING = {‘keys’: NONE, ‘rows_per_partition’: '120'}`
    * ALL
    * n - cache first n rows for a partition key
    * NONE - default
* `row_cache_size_in_mb` – max row cache size, set to 0 to disable (0 by default)

Bloom Filter
* Each SSTable has a bloom filter on disk; used from off-heap memory
* FALSE POSITIVE is possible
* FALSE NEGATIVE is not
* trade off between using more memory or better accuracy
  * 0.0: no false positive, big memory use
  * 0.1: max recommended setting; should not set higher than this, or return diminishes
  * 1.0 disable bloom filter

Key Cache
* enable using cQL:
  * `CREATE TALBE… ‘WITH CACHING = {‘keys’: ‘ALL’, ‘rows_per_partition’: ‘NONE’}`
  * key_cache_size_in_mb
