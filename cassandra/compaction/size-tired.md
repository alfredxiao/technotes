* is Default.

* Triggers when multiple sstables of a similar sire are present.

* Good for high writes.

* Compacting files with similar size
  * minimises write amplification (compacting a 1M file and a 1T file is not efficient)
* requires 50% hard drive space
* Partition data can possible be scattered amongst several sstables
* Stale records in larger sstables take unnecessary space
* as a result, a READ can be slower due to the need to access many sstable files

* cassandra compacts several tiers (buckets) concurrently
  * defined by `concurrent_compactors`
  * defaults to min(numofdisks, numofcors)
