QUORUM
* a quorum of replica nodes across all datacenters.

LOCAL_QUORUM
* a quorum of replica nodes in local datacenter

EACH_QUORUM
* a quorum of replica nodes in each datacenter
* applicable to WRITEs only

SERIAL/LOCAL_SERIAL
* applicable to READs only
* used to read the latest value of a column after a user has invoked a lightweight transaction to write to the column
