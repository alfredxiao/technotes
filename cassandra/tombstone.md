Generation of Tombstones
* Using a CQL DELETE statement
* Expiring data with time-to-live (TTL)
* Using internal operations, such as Using materialized views??
* INSERT or UPDATE operations with a null value
* UPDATE operations with a collection column

Having an excessive number of tombstones in a table can negatively impact application performance. Many tombstones often indicate potential issues with either the data model or in the application.
