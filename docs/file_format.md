# File Format

## Information Elements

Non fixed information content is provided within Information Elements of the form Tag - Length - Value (TLV). 

The tag and length fields are fixed in size, being 2 bytes long each. The number of bytes provided as value are of variable 
count, ranging from 0 to 65535. The figure held in the length field measures the number of value bytes, excluding the bytes in the
tag and length fields. 

Regarding endianness, all fields are written in native endianness.

If the total number of bytes consisting an Information Element (TLV) are not a multiple of 4, padding bytes are added so that the
footprint fo the Information Element is divisible by 4 (i.e. is a multiple of 32 bit words).

The figure in the length field excludes the padding bytes.

### TLV schematic

```
            1          2          3 
 01234567 89012345 67890123 45678901
+--------+--------+--------+--------+
|       Tag       |     Length      |
+--------+--------+--------+--------+
|                                   |
|            Value bytes            |
|                                   |
|        | (Possible padding bytes) |
+--------+--------+--------+--------+
```

## Header

The file header appears once, at the beginning of the file.

```
            1          2          3 
 01234567 89012345 67890123 45678901
+--------+--------+--------+--------+
|           Magic Number            |
+--------+--------+--------+--------+
| Version|  Flags |     Unused      |
+--------+--------+--------+--------+
|            Process ID             |
+-----------------------------------+
|             Timestamp             |
|       (Seconds since Epoch)       |
+-----------------------------------+
|             Timestamp             |
|       (Fast clock reference)      |
+-----------------------------------+                                
|      Variable Header Part Tag     |
+--------+--------+--------+--------+
|    Variable Header Part Length    | Covers all Header IEs below
+--------+--------+--------+--------+
|                                   |
:        Variable Header IE#1       : Learn More 
|                                   |
+-----------------------------------+
|                                   |
:        Variable Header IE#2       :
|                                   |
+-----------------------------------+
:                                   :
/                                   /
:                                   :
+-----------------------------------+
|                                   |
:        Variable Header IE#n       :
|                                   |
+-----------------------------------+
```

### Magic Number

Literal byte sequence that identifies this particular file format, equal to `0x780617a5`

### Version

This single byte field holds the version used to describe the grammar of the file. Equal to 1 for this specification

### Flags

The flags field is one byte long and holds a number of flags that describe different aspects of the current capture file.
The bits in this field are used from least significant to most significant order, being numbered B0 (LSB) to B7 (MSB).
At the moment, only one such flag is defined:

* Endianness (E) --- B0
  0 if little-endian, 1 if big-endian

## Captured System Call

Captured system calls have the following format

```
            1          2          3 
 01234567 89012345 67890123 45678901
+--------+--------+--------+--------+
|            Tag (0x0001)           |
+--------+--------+--------+--------+
|               Length              |
+--------+--------+--------+--------+
| System Call Num |  Flags | Unused |
+--------+--------+--------+--------+
|           Return value            |
|                                   |
+--------+--------+--------+--------+
|         Process / Thread ID       |
+--------+--------+--------+--------+
|            System Call            |
|          Entry Timestamp          | Nanoseconds from fast clock (reference provided in header)
+--------+--------+--------+--------+
|   System Call In Kernel Duration  | Nanoseconds if MSB = 0 / Milliseconds if MSB = 1
+--------+--------+--------+--------+
|            Error Number           |
+--------+--------+--------+--------+
|                                   |
:             Arg IE#1              :
|                                   |
+-----------------------------------+
|                                   |
:             Arg IE#2              :
|                                   |
+-----------------------------------+
:                                   :
/                                   /
:                                   :
+-----------------------------------+
|                                   |
:             Arg IE#n              :
|                                   |
+-----------------------------------+
```

### Flags

The flags field is 1 byte long and is numbered as in the header case from B0 (LSB) to B7 (MSB). The bits currently used are:

 * Thread ID (Th) --- B0

   Shows whether the Thread ID field is present. If non-present, the system call is implicitly considered to be invoked by the main Thread, whose ID has been provided in the header.
 
 * Timestamp (Ts) --- B1

   Shows whether the System Call Entry Timestamp field is present.

 * Duration (D) --- B2

   Shows whether the System Call In Kernel duration field is present.

 * Error number (E) --- B3

   Shows whether the Error Number field is present.