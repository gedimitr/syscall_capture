# File Format

## Information Elements

Most information content is provided within Information Elements of the form Tag - Length - Value (TLV). 

There are two forms for TLVs, which differ regarding the length of the first two fields, Tag and Length. The first bit in the tag shows whether the short form (0) or the long form (1) is used.

Regarding endianness:
* That tag / length fields are written in big endian form.
* The value is always written in native endianness

Regardless of which form is used, if the number of values bytes are not a multiple of 4, padding bytes are added so that the Information Element uses up a number of bytes that are a multiple of 4.

Note: The figure in the length field excludes the padding bytes.

### Short Form

```
            1          2          3 
 01234567 89012345 67890123 45678901
+--------+--------+--------+--------+
|0      Tag       |     Length      |
+-----------------------------------+
|            Value bytes            |
+-----------------------------------+
```

### Long Form

```
            1          2          3 
 01234567 89012345 67890123 45678901
+--------+--------+--------+--------+
|1               Tag                |
+-----------------------------------+
|               Length              |
+-----------------------------------+
|            Value bytes            |
+-----------------------------------+
```

## Header

The file header appears once, at the beginning of the file.

```
            1          2          3 
 01234567 89012345 67890123 45678901
+--------+--------+--------+--------+
|           Magic Number            |
+-----------------------------------+
|            Header Tag             |
+-----------------------------------+
|           Header Length           | Covers all Header IEs below
+-----------------------------------+
|                                   |
:            Header IE#1            :
|                                   |
+-----------------------------------+
|                                   |
:            Header IE#2            :
|                                   |
+-----------------------------------+

/                                   /

+-----------------------------------+
|                                   |
|            Header IE#n            |
|                                   |
+-----------------------------------+

```