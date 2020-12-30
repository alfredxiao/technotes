compaction threshold
* per-table
* This setting means the number of sstables in a queue to be compacted
* default min=4, max=32
* If too large, the next compaction will need to compact many files at once, hence consuming lots of resources
* If too small, compaction too often unnecessarily
* You can modify this by `nodetool getcompactionthreshold` or `setcompactionthreshold`

Compaction Stats
* `nodetool compactionstats`

Compaction Throughput
* `nodetool getcompactionthroughput` and `setcompactionthroughput`
* `compaction_throughput_mb_per_sec` in `cassandra.yaml` config file

Compaction Threads
* `concurrent_compactors` in `cassandra.yaml`
* or `CompactionManagerMBean`
* It defaults to the minimum of the number of disks and number of cores, with a minimum of 2 and a maximum of 8.

`nodetool garbagecollect`
* If your concern is specifically related to cleanup of deleted data, you may use nodetool garbagecollect as an alternative to a major compaction.

`nodetool compactionhistory`

`nodetool compact`
`nodetool disableautocompaction`
`nodetool enableautocompaction`
