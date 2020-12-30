Understand ELF file format, using gcc on Linux 64bit
====================================================

* Reference http://www.linuxjournal.com/article/1059 The ELF Object File Format: Introduction. Apr 01, 1995	 By Eric Youngdale


* Sample code
```
// hello.c
#include <stdio.h>

int main() {
        printf("Hello World\n");
}
```

* Compile
  * `gcc -c hello.c`
  * this generates hello.o (as object file)
  * `-c` means compile and assembly but do not link
  * Generated `hello.o` is much larger than `hello.c`
* ELF Header
  * `readelf -h hello.o`
  * this displays ELF header information of hello.o, where you can see things like TYPE, and Number of section headers.
```
readelf -h hello.o
ELF Header:
  Magic:   7f 45 4c 46 02 01 01 00 00 00 00 00 00 00 00 00
  Class:                             ELF64
  Data:                              2's complement, little endian
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              REL (Relocatable file)
  Machine:                           Advanced Micro Devices X86-64
  Version:                           0x1
  Entry point address:               0x0
  Start of program headers:          0 (bytes into file)
  Start of section headers:          696 (bytes into file)
  Flags:                             0x0
  Size of this header:               64 (bytes)
  Size of program headers:           0 (bytes)
  Number of program headers:         0
  Size of section headers:           64 (bytes)
  Number of section headers:         13
  Section header string table index: 12
```

* Types
  * Relocatable file: holds code and data suitable for linking with other object files to create an executable or a shared object file.
  * Executable file: holds a program suitable for execution
  * Shared Object file: holds code and data suitable for linking in two contexts
    * link the shared object file with other relocatable and shared object files to create another object file
    * the dynamic linker combines it with an executable file and other shared objects to create a process image.
    * a.out does NOT support this concept
  * Files of these types actually have similar internal structure

* Section
  * Section is very fundamental concept in ELF file, or even a.out files
  * Each section represents a portion of the file. For example
    * executable code is always placed in a section known as .text;
    * all data variables initialized by the user are placed in a section known as .data;
    * and uninitialized data is placed in a section known as .bss
    * Typically each a.out or ELF file also includes a symbol table. This contains a list of all of the symbols (program entry points, addresses of variables, etc.) that are defined or referenced within the file, the address associated with the symbol, and some kind of tag indicating the type of the symbol.

* Libraries
  * Static Library
    * Code is copied into executable
  * Shared Library
    * Code is NOT copied into executable
    * When you link your program against the shared library, the linker merely makes note of the fact that you are calling a function in a shared library, so it does not extract any executable code from the shared library. Instead, the linker adds instructions to the executable which tell the startup code in your executable that some shared libraries are also required, so when you run your program, the kernel starts by inserting the executable into your address space, but once your program starts up, all of these shared libraries are also added to your address space.

* Relocation
  * your program uses `printf` function, the location of which is not known when this object file (your program that uses printf) is generated, but generated in linking.

* Section Headers
```
readelf -S hello.o
There are 13 section headers, starting at offset 0x2b8:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .text             PROGBITS         0000000000000000  00000040
       0000000000000017  0000000000000000  AX       0     0     1
  [ 2] .rela.text        RELA             0000000000000000  00000208
       0000000000000030  0000000000000018   I      10     1     8
  [ 3] .data             PROGBITS         0000000000000000  00000057
       0000000000000000  0000000000000000  WA       0     0     1
  [ 4] .bss              NOBITS           0000000000000000  00000057
       0000000000000000  0000000000000000  WA       0     0     1
  [ 5] .rodata           PROGBITS         0000000000000000  00000057
       000000000000000c  0000000000000000   A       0     0     1
  [ 6] .comment          PROGBITS         0000000000000000  00000063
       000000000000001d  0000000000000001  MS       0     0     1
  [ 7] .note.GNU-stack   PROGBITS         0000000000000000  00000080
       0000000000000000  0000000000000000           0     0     1
  [ 8] .eh_frame         PROGBITS         0000000000000000  00000080
       0000000000000038  0000000000000000   A       0     0     8
  [ 9] .rela.eh_frame    RELA             0000000000000000  00000238
       0000000000000018  0000000000000018   I      10     8     8
  [10] .symtab           SYMTAB           0000000000000000  000000b8
       0000000000000120  0000000000000018          11     9     8
  [11] .strtab           STRTAB           0000000000000000  000001d8
       0000000000000029  0000000000000000           0     0     1
  [12] .shstrtab         STRTAB           0000000000000000  00000250
       0000000000000061  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  l (large), p (processor specific)
```
  * `readelf -S hello.o`
  * this displays section table information
   .shstrtab means a string table which is used for section headers
   'Address' column of this table means virtual memory address that a section is to be loaded into. However, a object file is not meant to be loaded into memory, so it would be 00...0. For a 64 bit file, you will see this address has 8 bytes.
   'offset' column meaning the offset within the file of the section.
   'size' is size of the section.
   'flags' indicates whether the section is read-only, writable, and/or executable.
4. readelf -r hello.o
   this displays relocation information about the code section
   you should see 'printf' here?
5. gcc hello.c -o hello
   to make executable file
6. readelf -h hello
   now 30 sections!
7. readelf -S hello; readelf -l hello
   .interp section contains ASCII string that is the name of a dynamic loader, in my case '/lib64/ld-linux-x86-64.so.2'
   .hash section is just a hash table that is used so that we can quickly locate a given symbol in the .dynsym section, thereby avoiding a linear search of the symbol table.
   .dynsym, and .dynstr are a minimal symbol table used by the dynamic linker when performing relocations. You will notice that these sections are mapped into virtual memory because the virtual address field is non-zero.
   .symtab and .strtab are the regular symbol and string tables, and these are not mapped into virtual memory by the loader.
   .plt contains the jump table that is used when we call functions in the shared library. By default the .plt entries are all initialized by the linker not to point to the correct target functions, but instead to point to the dynamic loader itself. Thus, the first time you call any given function, the dynamic loader looks up the function and fixes the target of the .plt so that the next time this .plt slot is used we call the correct function. After making this change, the dynamic loader calls the function itself.
   This feature is known as lazy symbol binding. The addresses used for the target of the jump are actually stored in the .got section. The .got also contains a set of pointers for all of the global variables that are used within a program that come from a shared library.
   .dynamic section is essentially just a distilled version of the section header table that contains just what is needed for the dynamic loader to do its job. This is introduced because the section header table itself is not loaded into memory for dynamic loader to utilise. .A list of required shared libraries is stored in the .dynamic section.
   This can be seen by running 'readelf -l hello'

Concepts
- .text segment/section, also known as CODE SEGMENT
  In computing, a code segment, also known simply as text, is a portion of an object file or the corresponding section of the program's virtual address space that contains executable instructions. The term "segment" comes from the memory segment, which is a historical approach to memory management now known as paging.
