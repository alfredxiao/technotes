# Context
**Record** is used in table b-tree leaf and the key of an index b-tree page

# Format
- `[header][body]`

## `[header]`
- `[varint size][varint serial type]+`
  - `[varint size]`: is the length of entire header (including the [varint size] itself)
  - `[varint serial type]+`
    - For each varint, refer to `Serial Type Codes Of The Record Format` in https://www.sqlite.org/fileformat.html.
    - Some types do not require value to be present
      - `0` represents NULL
      - `8` represents `0`
      - `9` represents `1`
    - Two types of variable length
      - when `N≥12 and even`, it represents BLOB of length `(N-12)/2`
      - when `N≥13 and odd`, it represents VARCHAR of length `(N-13)/2`
      - Note that this is about 'record' - an instance or row of a table, **NOT** about the table definition, so the `N` is different for different rows

## `[body]`
- `[column value]+`
  - Some types does not require column value bytes to be present (in a record)
  - Some types require static number of bytes
  - For BLOB and VARCHAR, length is dynamic

# An Example Record (table leaf page)
`0319 0161 6c66 7265 6419` is a record
- `0319 01` is the header
  - because `03` determines the total number of bytes in the header, including the `03` itself, and two more bytes
  - `19`: `0x19` -> `25`, `25>=13` and is odd so it is VARCHAR of length `(25-13)/2` -> `6`
  - `01`: meaning value is 8-bit integer
- rest is the body
  - `61 6c66 7265 64` string `alfred` (of length 6)
  - `19` `0x19` -> `25` (represents table column of integer, e.g. table `Person`'s column `age`)

# An Example Record (index leaf page)
`0x 03 1701 6272 6961 6e02` is a record
- `03`: meaning `03 1701` is header
- `17`: 0x17 -> 23, meaning VARCHAR(5)
- `01`: meaning 8-bit integer
- `6272 6961 6e`: represents `brian`
- `02`: meaning rowid is integer `2`
