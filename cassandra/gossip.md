Basic
* A peer-to-peer communication protocol to discover and share location and state information about the other nodes in a Cassandra cluster.
* Gossip information is persisted locally by each node to use immediately when a node restarts.


* To prevent problems in gossip communications, be sure to use the same list of seed nodes for all nodes in a cluster.
* Generally good number of seed nodes in a data centre is 3 (or less for very small ring)

Each node initiates a gossip round every second
picks one to three nodes to gossip with
Nodes can gossip with ANY other node in the cluster
Slightly favour seed nodes and downed nodes
Nodes do not remember which nodes they gossiped with before

when a nod joins a cluster, it gossips with seed nodes that are specified in cassandra.yaml
* assign the same seed node to each node in a data center
* if more than one DC, include seed node from each DC
