Tuple
* A tuple is a fixed-length list, where values can be of different types.
* Unlike UDTs, individual tuple components cannot be updated without updating the whole tuple.
  * Meaning it is always updated as a whole, hence it is FROZEN

```
ALTER TABLE users ADD full_name TUPLE<TEXT,TEXT,TEXT>;

UPDATE users
SET full_name = ('Joe', 'The', 'Great')
WHERE id = 7902a572-e7dc-4428-b056-0571af415df3;

SELECT name, full_name FROM users;
```

UDT
* More flexible than TUPLE, as UDT is not FROZEN
* You can update a single field of a UDT
* You can select a single field of a UDT

```
CREATE TYPE ADDRESS (
    street TEXT,
    city TEXT,
    state TEXT,
    postal_code TEXT
);
UPDATE users
SET address = { street: '1100 Congress Ave',
                city: 'Austin',
                state: 'Texas',
                postal_code: '78701' }
WHERE id = 7902a572-e7dc-4428-b056-0571af415df3;
SELECT name, address FROM users
WHERE id = 7902a572-e7dc-4428-b056-0571af415df3;

UPDATE users
SET address.state = 'TX'
WHERE id = 7902a572-e7dc-4428-b056-0571af415df3;
SELECT name,
       address.street      AS street,
       address.city        AS city,
       address.state       AS state,
       address.postal_code AS zip
FROM users
WHERE id = 7902a572-e7dc-4428-b056-0571af415df3;
```
