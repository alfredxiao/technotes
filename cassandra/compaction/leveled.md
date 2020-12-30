Groups sstables into levels. Each level has a fixed size limit which is 10 times larger than the previous level.

*  Good for read heavy use-cases.

* Note that level 0 is the landing place, everything there will get promoted
* default sstable max size is 160M
  * `sstable_size_in_mb`

* L1's max size is a - by default - 10x sstable max size, i.e. defaults to 1.6G

* Each partition resides in only one Sstable per level
* Generally reads handled in just a few sstables
  * 90% of data resides in the lowest level (due to 10x rule)

Disk usage
* In general, an sstable overlaps with 10(ish) sstables in the level below
* As a result, compaction requires 11x sstable_size_in_mb to compact
  * 1 for sstable in higher level
  * 10 for the overlapped sstagbles in the next level
* So, leveled compaction wastes less disk space
  * because updated records merge with older records due to grouping

IO
* Very intensive
* compacts more frequently than size tiered
