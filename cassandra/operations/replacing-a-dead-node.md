Replacing vs. Removing & Adding
* If you remove a node and then add a new node, you introduce two rounds of token range movements and data streaming .
* Or if you add a new node and then remove an old node, same as above

Benefits
* Not need to move data twice
* Backups on the node being replaced still works
* less load on the cluster

Prerequisites
* a node is in DN (Down/Normal) state, you want to replace it, starting a new instance on the same IP or different IP, throwing away the dead node's data
  * Run `nodetool status` to verify that the node is dead (`DN`)
  * Throwing away data means risk if there is CL=ONE used for some data
* New node will be running the same version of Cassandra

Procedure
* If the dead node was a seed node
  * update all nodes' `cassandra.yml`'s `seed-provider` section, remove the dead node's IP address
  * If you want the new node to be a seed, add new node's IP there
* Set configuration for the replacing node
  * `cluster_name`
  * `endpoint_snitch`
  * `cassandra-rackdc.properties`/ `cassandra-topology.properties` (update if necessary)
  * `listen_address` and other addresses (if new node IP is different from dead node)
  * If the new node is a seed node, make sure it is NOT listed in its own - seeds list.
  * Other non-default settings
  * Make sure `auto_bootstrap` is true (or not present)
* in `jvm.options` include the following directive with the previous IP address:
  * `-Dcassandra.replace_address=previous_ip_address"`
* start the node
* Run `nodetool status` to verify its status
* After the node bootstraps, remove the replace-address parameter from cassandra-env.sh.
* Restart the node.
