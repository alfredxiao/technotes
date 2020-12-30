- ACID
  * Not valid when considering master slave, unless slave does NOT serve requests at all?

- Relational DB
  * Vertical scale is limiting
  * Normalised form of tables is BAD for queries, esp. complex queries  
  * Sharding
    * No more joins
    * No more aggregations
    * Querying secondary indexes requires hitting every shard

RDBMS Lessons Learned & Cassandra Design Principles
* Consistency is not practical
  * So we give it up
* Manual Sharding & Rebalancing is hard
  * So build it as part of the cluster
* ??
  * Simplify the architecture - no more master/slave
* Scaling up is expensive
  * We go for horizontal scaling
* Scatter/gather is bad
  * Goal is to always hit 1 machine
  * Denormalise everything

Cassandra Characteristics
* Distributed
  * No Master/Slave
  * No Leader election  
  * No Failover
* High Availability
* Linear Scalability
* Predictable Performance
* Multi-DC
  * RF is per keyspace per DC
  * DC can be physical or logical
* Commodity Hardware
* CAP Tradeoffs
  * When Network Partitioning happens, we choose Availability over Consistency
* Consistency Level
  * Per query basis

Hash Ring (Cluster)
* No Master/slave
  * All nodes are equal
* No config server, zookeeper
* Data is partitioned around the ring
  * Location of data on ring is determined by partition key
* Data is replicated to RF=N servers
* All nodes hold data and can answer queries (both reads & writes)

Once table is defined
* You cannot change primary key!

Query Clustering Columns
* Partition Key must be provided
* = or range queries (> <) can be used on clustering columns
* All equality comparisons must come before inequality columns

Storage
* DO NOT run Cassandra on SAN
  * https://www.datastax.com/blog/2017/01/impact-shared-storage-apache-cassandra

Snitch???
* Is the mechanism to let Cassandra know which node is on which DC and Rack.
* All nodes must use the same snitch
* Changing cluster topology requires restarting all nodes
  * Run sequential repair and cleanup on each node

Logging Level
* ./nodetool getlogginglevels
* ./nodetool setlogginglevel org.apache.cassandra TRACE
* /var/log/cassandra/system.log

Traces
* ./nodetool settraceprobability 0.1
* Saved traces can then be viewed in the system_traces keyspace

Adding a new Node
* The hash ring changes, will nodes move data to next nodes in the ring?
* Talking to seed nodes to understand structure

Node is Down?

Decommission a node?

Tomestone and Performance


Driver Choosing Coordinator
* on a per-query basis
* TokenAwarePolicy driver chooses nodes containing the data
* RoundRobinPolicy
* DCAwareRoundRobinPolicy -?

Which partition resides on which node?
* SELECT token(tag), tag FROM videos_by_tag;
* nodetool getendpoints killrvideo videos_by_tag 'cassandra'

VNode
* https://www.datastax.com/blog/2012/12/virtual-nodes-cassandra-12
* If you want to get started with vnodes on a fresh cluster, however, that is fairly straightforward. Just don't set the initial_token parameter in your conf/cassandra.yaml and instead enable the num_tokens parameter. A good default value for this is 256.


Hint Handoff
* Default to saved for 3 hours in coordinator node
* When CL=ANY, it means a WRITE is successfully as long as written to coordinator, even if NONE of the replicas nodes are running.

Read Path
*

Tabel stats
* /home/ubuntu/node/bin/nodetool cfstats keyspace1.standard1

Memory cache in Read Path
* key cache
* Bloom Filter
* Partition Summary

Stored on Disk in Read Path
* Partition Indexes
* SSTable

Avoid
* Hot partition
  * too active compared with other partitions
* Big Partition
  * up to 2 billion cells per partition
  * up to 100K rows
  * up to 100MB
* Big and constantly growing partition
* Query and write requests across multiple partitions

Batches
* LOGGED
* UNLOGGED

Seed nodes
* all nodes use same seed list? (probably not)
