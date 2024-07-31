# Foundational Rules
## Pages
- The main database file consists of one or more pages.
- Common page size is 1k or 4k (depending on OS page size?)
- All pages within the same database are the same size.
- Pages are numbered beginning with 1.

# Questions
- How are table definition stored?
- How are table rows stored?
- How is index stored?
- How does index work?
- How does table join work?
- How does aggregation work?
- How I/O is controlled w.r.t. pages?
- What is a cursor?
- How does b-tree pages get reorganised when rebalancing (reposition nodes to make tree balanced) occurs?
- How does transaction work? commit and rollback
- How does a query works with arbitrary criteria?
- How is SQL parsed?
- How does the VM work that carries out queries?
- What happens over time, i.e. what are long term operations?
  - rebalancing the b-tree nodes
  - defragmentation
  - vaccum
- How are concurrent clients coordinated? (like one update, one query)
- When is temp file used?
- how is subquery implemented?
