# Context
- `main database file` contains **pages**
- A **page** contains **Cells** (and others)

# Page Types
- A b-tree page
  - A table b-tree interior page
  - A table b-tree leaf page
  - An index b-tree interior page
  - An index b-tree leaf page
- A freelist page
  - A freelist trunk page
  - A freelist leaf page
- A payload overflow page
- A pointer map page
- The lock-byte page

# B-Tree Page
A b-tree page is divided into regions in the following order:

- The 100-byte `database file header` (found on page 1 only)
- The `8` or `12` byte b-tree page header
  - `12` byte is for interior b-tree pages only
- The cell pointer array
- Unallocated space
- The cell content area
- The reserved region.

## Page Header
- `[pageType][startOfFirstFreeBlock][numberOfCells][startOfCellContentArea][?][rightMostPointer]`
- `[pageType]`
  - `0x02` interior index b-tree cell
  - `0x05` interior table b-tree cell
  - `0x0a` leaf index b-tree cell
  - `0x0d` leaf table b-tree cell
- `[numberOfCells]`:
- `[startOfCellContentArea]`:

# Payload Overflow Pages
- When the payload of a b-tree cell is too large for the b-tree page, the surplus is spilled onto overflow pages.
- Overflow pages form a linked list.
- The first four bytes of each overflow page are a big-endian integer which is the page number of the next page in the chain, or zero for the final page in the chain.
- The fifth byte through the last usable byte are used to hold overflow content.

# Pointer Map Pages
- Pointer map or ptrmap pages are extra pages inserted into the database to make the operation of auto_vacuum and incremental_vacuum modes more efficient.
