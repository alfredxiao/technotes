This is for a live node. If decommission is used, the data will stream from the decommissioned node. If removenode is used, the data will stream from the remaining replicas.

Sync/stream data to other nodes and then leave the cluster.

Procedures
* Check node status using `nodetool status`
* If the node is up, run `nodetool decommission`
  * This assigns the ranges this node was responsible for to other nodes and replicates the data appropriately
  * Run `nodetool netstats` to monitor progress
  * Note that `decommission` does NOT shutdown the node
* If the node is down, run `nodetool removenode`
* If `removenode` fails, run `nodetool assasinate`
