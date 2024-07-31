import sqlite3

c = sqlite3.connect("./words.db")
with open('/usr/share/dict/words') as f:
    c.execute("create table words (id int not null primary key, word varchar(120));")
    for i, word in enumerate(f):
        word = word.strip()
        c.execute("INSERT INTO words VALUES (?, ?);", (i, word))
c.commit()
c.close()
