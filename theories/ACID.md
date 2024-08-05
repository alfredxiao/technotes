# Isolation


# Rollback

<table>
  <tr>
    <th>Transaction1</th><th>Transaction2</th>
  </tr>
  <tr>
    <td>
      <pre>
      BEGIN;
      SELECT age FROM users WHERE id = 1;
      -- retrieves 20
      </pre>
    </td>
    <td>
    </td>
  </tr>
</table>

# Commit of Update

# References
- https://en.wikipedia.org/wiki/Isolation_(database_systems)
