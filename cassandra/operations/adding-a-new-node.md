What will happen
* new node's num_tokens determines how many token ranges it will be assigned for
* bootstraping will happen that assigns some token ranges to this new node
  * as well as streaming data from other nodes
* the joining node will pick current replicas of the token ranges it will become responsible for to stream data from. By default it will stream from the primary replica of each token range in order to guarantee data in the new node will be consistent with the current state.
  * In the case of any unavailable replica, the consistent bootstrap process will fail. To override this behavior and potentially miss data from an unavailable replica, set the JVM flag `-Dcassandra.consistent.rangemovement=false`

Procedures
* Install Cassandra software of the same version as other nodes
* Copy snitch properties file to this new node and update if needed
* Configure below properties in cassandra.yaml
  * auto_bootstrap = true
  * cluster_name
  * listen_address
  * endpoint_snitch
  * num_tokens
  * seed_provider
* Start the new node
* run `nodetool status` to verify it and all other nodes are in `UN` state .
* After new node running as `UN`, run `nodetool cleanup` on each of previously existing nodes to remove the keys that no longer belong to those nodes. Wait for cleanup to complete on one node before running cleanup on next node
