# Terminology
C for Consistency
A for Availability
P for Partition-tolerance.

# Consistent
## Database
It means integrity constraints on the data
Consistency in ACID relates to data integrity such as:
- data types,
- relations,
- field is not null
- FK reference

The database will not let you commit a transaction that breaks a constraint. That’s the consistency-in-ACID contract. This consistency definition has no equivalent in CAP.

## CAP
Consistency means that the nodes will have the **same copies of a replicated data item** across replicated distributed systems are visible for various transactions. A guarantee that every node in a distributed cluster returns **the same, most recent** and a successful write.

Consistency refers to every client having the same view of the data.
Every read receives the most recent write or an error.

There are various types of consistency models. Consistency in CAP refers to **sequential consistency**, a very strong form of consistency.

## Available
**All** non-failing nodes continue to serve requests if the system is partitioned. Many distributed systems will consider themselves as available if, when there is a partition, some non-failing nodes continue to serve requests. These systems are not available-in-CAP.

## Partition-Tolerance
The system continues to operate despite an arbitrary number of messages being dropped (or delayed) by the network between nodes.

# Summary
No distributed system is safe from network failures, thus network partitioning generally has to be tolerated. In the presence of a partition, one is then left with two options: consistency or availability. When choosing consistency over availability, the system will return an error or a time out if particular information cannot be guaranteed to be up to date due to network partitioning. When choosing availability over consistency, the system will always process the query and try to return the most recent available version of the information, even if it cannot guarantee it is up to date due to network partitioning.

In the absence of a partition, both availability and consistency can be satisfied.

# References
- http://blog.thislongrun.com/2015/03/the-confusing-cap-and-acid-wording.html
- https://api.repository.cam.ac.uk/server/api/core/bitstreams/a37941b0-4868-4323-87c9-538f36222676/content
