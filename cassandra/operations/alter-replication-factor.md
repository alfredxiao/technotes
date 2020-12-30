Procedures
* `ALTER KEYSPACE cycling WITH REPLICATION = {'class' : 'NetworkTopologyStrategy', 'DC1' : 3};`
* Run `nodetool repair -full` on each node affected by the change
