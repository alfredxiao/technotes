# Basics
- The b-tree algorithm provides key/data storage with **unique** and **ordered** keys on page-oriented storage devices.
- Two variants of b-trees are used by SQLite.
  - **Table b-trees** use a 64-bit signed integer key and store all data in the leaves. This is `B+ Tree`
  - **Index b-trees** use arbitrary keys and store no data at all. This is `B- Tree`.

# B Tree Pages
- A b-tree page is either an **interior page** or a **leaf page**.
- A `leaf page` contains keys and in the case of a table b-tree each key has associated data.
- An `interior page` contains `K` keys together with `K+1` **pointers** to child b-tree pages.
  - A `pointer` in an interior b-tree page is just the 32-bit unsigned integer page number of the child page.
- Every b-tree page has at most one **parent b-tree page**.
- A b-tree page without a parent is called a **root page**.
- A root b-tree page together with the closure of its children form a complete b-tree.

# Number of Keys
- The number of keys on an interior b-tree page, `K`, is almost always at least 2 and is usually much more than 2.
- If page 1 is an interior b-tree page, it can end up holding just a single key. In all other cases, `K` is 2 or more.
- The upper bound on `K` is as many keys as will fit on the page.
  - Large keys (on index b-trees) may be split into overflow pages
  - The integer keys of table b-trees are never large enough to require overflow

# B for Balanced
- In a **well-formed database**, all children of an interior b-tree have the same depth.
- It is maintained so that it does **self-balancing**

# Logical Organisation of Keys
- In an interior b-tree page, the pointers and keys *logically* alternate with a pointer on both ends.
  - It looks something like `P|K|P|K|P|K|P|K|P|K|P` for an keys and pointers in an interior page
  - `K` for key, `P` for pointer (a page number)

- Two important properties
1. For any key `X`, pointers to the **left** of a `X` refer to b-tree pages on which all keys are **less than or equal to `X`**.
2. Pointers to the **right** of `X` refer to pages where all keys are **greater than `X`**.

- All keys within the same page are unique and are logically organized in ascending order from left to right.
  - his ordering is *logical*, **not physical**.
  - The actual location of keys within the page is arbitrary.

# Cell
- Within an interior b-tree page, each key and the pointer to its immediate left are combined into a structure called a **cell**. The right-most pointer is held separately.
  - E.g. `P|K|P|K|P|K|P|K|P|K|P` is organised as cells
    - `[P|K] [P|K] [P|K] [P|K] [P|K] P`
- A leaf b-tree page has no pointers, but it still uses the cell structure to hold keys for index b-trees or keys and content for table b-trees. Data is also contained in the cell.

# Tree Identity (Handle)
It is possible (and in fact rather common) to have a complete b-tree that consists of a single page that is both a leaf and the root.
Because there are pointers from parents to children, every page of a complete b-tree can be located if only the root page is known. Hence, b-trees are identified by their root page number.

# Table or Index B-Tree
- A b-tree page is either a table b-tree page or an index b-tree page.
- All pages within each complete b-tree are of the same type: either table or index.

# Relationships with Tables
- There is one table b-trees in the database file for each **rowid table** in the database schema, including system tables such as `sqlite_schema`.
- There is one index b-tree in the database file for each index in the schema, including implied indexes created by uniqueness constraints.
- `WITHOUT ROWID` tables use index b-trees rather than a table b-trees, so there is one index b-tree in the database file for each `WITHOUT ROWID` table.

# Master Schema
- The b-tree corresponding to the `sqlite_schema` table is always a table b-tree and always has a root page of `1`.
  - The `sqlite_schema` table contains the root page number for every other table and index in the database file.

# Key and Value/Data
- Each entry in a `table b-tree` consists of a `64`-bit signed integer key and up to 2147483647 bytes of arbitrary data.
  - The key here corresponds to the `rowid`
- Interior table b-trees hold only keys and pointers to children. All data is contained in the table b-tree leaves.
- Each entry in an `index b-tree` consists of an arbitrary key of up to 2147483647 bytes in length and no data.
