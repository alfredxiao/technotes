it restream data and repartition
  * so can load into a cluster of different topology (different number of nodes)

  usage: sstableloader [options] <dir_path>

Bulk load the sstables found in the directory <dir_path> to the configured
cluster.The parent directories of <dir_path> are used as the target
keyspace/table name. So for instance, to load an sstable named
Standard1-g-1-Data.db into Keyspace1/Standard1, you will need to have the
files Standard1-g-1-Data.db and Standard1-g-1-Index.db into a directory
/path/to/Keyspace1/Standard1/.


Requires `cassandra.yaml` configuration (as it talks to a target cluster, it needs to know some basic info about it)
  * at least one node in the cluster is configured as a seed
  * either run on an existing node of a cluster
  * or having a sort of copy of cassandra.yaml file ready
