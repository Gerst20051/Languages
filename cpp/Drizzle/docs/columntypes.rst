Built in Column Types
=====================

---------------------
VARCHAR and VARBINARY
---------------------

A VARCHAR or VARBINARY type is used to store variable length data. Indexes
on these types are by default the full length of the data stored.
The only difference between the two types is the COLLATION which is
used. VARBINARY uses a binary collation for all index usage. VARCHAR can only
contain valid UTF-8 characters.

-------------
TEXT and BLOB
-------------

A TEXT or BLOB type is used to store data which is over XXX in size. Indexes
on these types must specificy the number of character or bytes which should
be used. The only difference between the two types is the COLLATION which is
used. A BLOB usees a binary collation for all index usage. A TEXT column
can only contain valid UTF-8 characters.

---------
NUMERICAL
---------

BIGINT and INTEGER exist as Drizzle's two integer numerical types. BIGINT is
a 64bit integer while INTEGER is a 32bit integer.

DOUBLE is the systems native double type.

DECIMAL is a fixed precision number.

--------
TEMPORAL
--------

DATETIME

TIMESTAMP

DATE

----
ENUM
----

Enum (enumerated) types are static lists of strings that are defineed on
table creation. They can be used to represent a collection of string types
that are sorted based on the order that they are created.

------
SERIAL
------

A SERIAL is a meta type that creates a column where a number is inserted in
increasing order as rows are inserted into the table. The actual type is a
BIGINT.
