import sqlite3

c = sqlite3.connect("./words_pk_by_word.db")
with open('/usr/share/dict/words') as f:
    c.execute("create table words (word varchar(120) not null primary key, dup varchar(120));")
    for i, word in enumerate(f):
        word = word.strip()
        c.execute("INSERT INTO words VALUES (?, ?);", (word, word))
c.commit()
c.close()
