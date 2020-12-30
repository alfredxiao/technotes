Procedures
* in `cassandra.yaml` update
  * `rpc_address`, `listen_address`, `seeds`,
  * and any reference to the previous IP address.
  * Also, ensure that `auto_bootstrap` is set to `false` (by default the directive is omitted and the default value is true).
* Depending on your installation, also look in
  * `cassandra-topology.properties` and `cassandra-rackdc.properties`, `jvm.options`
  * wherever the IP address is used in configuration
* Stop the cassandra service
* Change the IP address of the box (i.e. ifconfig, add the new route, etc.)
* in `cassandra-env.sh` include the following directive with the previous IP address:
  * `JVM_OPTS="$JVM_OPTS -Dcassandra.replace_address=previous_ip_address"`
* Start the Cassandra service in the node.
  * Wait until nodetool status reports it as UN, you should notice that the new IP address is reported
* Cleanup the IP transition configuration
  * If you added the `auto_bootstrap` directive in `cassandra.yml`, remove it
  * remove the `cassandra.replace_address` directive from `cassandra-env.sh`
* repeat in a different node

Notes
* setting `auto_bootstrap` to `false` was to disable data streaming step of the bootstrap process
  * this flag is not about the entire bootstrapping process, but just the data streaming step.
* `cassandra.replace_address_first_boot` was introduced to be used in favor of `cassandra.replace_address`
  * such that you do not need to remove it from config file after it has been booted once.
