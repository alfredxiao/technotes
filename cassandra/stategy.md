SimpleStrategy:
* Use only for a single datacenter and one rack. If you ever intend more than one datacenter, use the NetworkTopologyStrategy.
* SimpleStrategy places the first replica on a node determined by the partitioner. Additional replicas are placed on the next nodes clockwise in the ring without considering topology (rack or datacenter location).

NetworkTopologyStrategy:
* Highly recommended for most deployments because it is much easier to expand to multiple datacenters when required by future expansion.
