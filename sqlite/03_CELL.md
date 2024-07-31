# Context
- **Cell** is used in table b-tree leaf and the key of an index b-tree page
- **Cell** contains **Record**

# Cell Format
- Slightly different among cell types
  - leaf table cell
  - leaf index cell
  - interior table cell
  - interior index cell
- Regions/Units (to be assembled)
  - `varint payloadSize`
  - `int32 leftPage`: pointing to left page
  - `varint rowid` (a.k.a. integer key)
  - `byte[] firstPayload`
  - `int32 firstOverflowPage`: pointing to first overflow page
- Index cells require
  - `[leftPage]?[payloadSize][firstPayload][firstOverflowPage]?`
  - `[leftPage]` only required for interior index cell
- Interior Table Cell requires
  - `[leftPage][rowid]`
- Leaf Table Cell requires
  - `[payloadSize][rowid][firstPayload][firstOverflowPage]?`

## Format Observations
- Only interior cells require a `leftPage` number
- Only table interior cells missing `payloadSize`, `firstPayload`, as well as `firstOverflowPage`
  - its only payload technically is the `rowid`
  - others have payload that holds a **record**
- `rowid` exist in table cells (interior and leaf)
  - appear at cell level
  - exists in leaf index cells at record level
  - does not exist in interior index cell or record

# An Example Table Leaf Cell
`0x 0a01 0319 0161 6c66 7265 6419` is a table leaf cell

- `0a`: A varint which is the total number of bytes of payload, including any overflow
  - `payload` means the record (*excluding* `this` varint, and the `rowid`)
- `01`: A varint which is the integer key, a.k.a. `rowid`
- rest: The initial portion of the payload that does not spill to overflow pages.
  - `0x 0319 0161 6c66 7265 6419` is a record
    - `03`: this *varint* which determines the total number of bytes in the header. The varint value is the size of the header in bytes including the size varint itself, meaning `0319 01` is header.
    - `19`: this *varint* 0x19 -> 25, 25>=13 and is odd so it is varchar
      - (25-13)/2 -> 6, which is string length
    - `01`: this *varint* means second column is a 8-bit twos-complement integer
    - `61 6c66 7265 64`: value for first column: string `alfred` of length 6
    - `19`: value for second column: 8-bit integer, which is `25` (`0x19`)

# An Example Index Leaf Cell
`0x 0903 1909 616c 6672 6564`
- `09`: rest total 9 bytes
- `0x 03 1909 616c 6672 6564` is a record
  - `03`: meaning `03 1909` is header
  - `19`: meaning column value type is varchar(6)
  - `09`: meaning value `1`

# An Example Table Interior Cell
`0x 00 0003 2f83 d90c`
- `00 0003 2f`: `32`-bit integer `815` (page number)
- `83 d90c`: varint, which is `60556` (rowid)
`0x 0000 0330 87b1 12`
- `0000 0330`: `32`-bit integer `816`
- `87b1 12`: varint, which is `120978`

# An Example Index Interior Cell
`0x 00 0008 3011 0323 0374 656c 6c75 7265 7468 796c 030b d5`
- `00 0008 30` left child page number -> `2096`
- `11` 17 bytes as payload (excluding `11` itself)
- `03` record header is `0323 03`
- `23` meaning 0x23=35, (35-13)/2=11, VARCHAR(11)
- `03` meaning 24bit integer
- `74 656c 6c75 7265 7468 796c` `tellurethyl`
- `030b d5` -> 199637 (rowid)
