
`gcc helloworld.c -o helloworld -lsqlite3 -L.`


To compile using modified library, copy the library file to current folder as e.g. `libmysqlite3.dylib`, then

`gcc helloworld.c -o helloworld -lmysqlite3 -L.`
