Two appraoches
* Decommission the node, then add a new node (in new rack),
  * follow procedures for decommission a node
  * then procedures for adding a new node
* Another way   
  * Bit risky procedures (risk in that before repair is complete, it handles requests for data it does not have blindly)
    * Update the node's topology
    * restart it
    * Once it is up, run a full repair
  * Better procedure
    * Start the node with `-Dcassandra.join_ring=false`
    * Repair
    * Join ring using JMX `org.apache.cassandra.db.StorageService.joinRing()`
    * After joining, repair again
