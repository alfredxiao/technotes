SimpleSnitch
* single data center
* need to use SimpleStrategy as well
* Use this snitch only for development deployments.
* This snitch does not recognize datacenter or rack information.

Dynamic Snitch
* Enabled by default as another layer on top of existing snitch
* Is a wrapper on top of existing snitch, like GossipingPropertyFileSnitch
* Its purpose is to provide more info about a node, especially its load, so as to aid in better routing. E.g. route a request to a node with lower load, with better response time, etc.

PropertyFileSnitch
- cassandra-topology.properties

GossipingPropertyFileSnitch
- cassandra-rackdc.properties

RackInferringSnitch
- Infers from IP address
  * A.B.C.D
    * B = dc
    * C = rack
    * D = node
