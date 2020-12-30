Request Level Repair
* When CL=1, no repair is needed at all
* When CL=ALL, always go with BLOCKING repair if needed
* When CL>1 but CL<ALL
  * Let's say CL=n
  * When data from n nodes do not match
    * repair in BLOCKING manner (meaning, return to client after repair is finished)
    * this can be configured to be turned off (v4.0?)
  * When data from n nodes match
    * And when `read_repair_chance` or `dclocal_read_repair_chance` says yes
    * verify consistency ALL OTHER nodes not involved in the read request
    * In BACKGROUND manner
    * DSE uses a single random value for both of the read repair tests and global read repair, `read_repair_chance`, is evaluated first.
    * If `read_repair_chance` is greater than or equal to `dclocal_read_repair_chance` for a given table, a local DC read repair never occurs.

Rapid Read Protection
* speculative_retry
  * For a read request, coordinator contacts CL number of nodes for data
  * If one of these nodes are down or slow (compared with threshold), coordinator fails the read request
  * However, if speculative_retry is configured, coordinator will try other replica nodes instead
    * So as to increase the chance of satisfying the read request
