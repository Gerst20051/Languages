:mod:`zlib` --- Compression compatible with :program:`gzip`
===========================================================

.. module:: zlib
   :synopsis: Low-level interface to compression and decompression routines
              compatible with gzip.


For applications that require data compression, the functions in this module
allow compression and decompression, using the zlib library. The zlib library
has its own home page at http://www.zlib.net.   There are known
incompatibilities between the Python module and versions of the zlib library
earlier than 1.1.3; 1.1.3 has a security vulnerability, so we recommend using
1.1.4 or later.

zlib's functions have many options and often need to be used in a particular
order.  This documentation doesn't attempt to cover all of the permutations;
consult the zlib manual at http://www.zlib.net/manual.html for authoritative
information.

For reading and writing ``.gz`` files see the :mod:`gzip` module. For
other archive formats, see the :mod:`bz2`, :mod:`zipfile`, and
:mod:`tarfile` modules.

The available exception and functions in this module are:


.. exception:: error

   Exception raised on compression and decompression errors.


.. function:: adler32(data[, value])

   Computes a Adler-32 checksum of *data*.  (An Adler-32 checksum is almost as
   reliable as a CRC32 but can be computed much more quickly.)  If *value* is
   present, it is used as the starting value of the checksum; otherwise, a fixed
   default value is used.  This allows computing a running checksum over the
   concatenation of several inputs.  The algorithm is not cryptographically
   strong, and should not be used for authentication or digital signatures.  Since
   the algorithm is designed for use as a checksum algorithm, it is not suitable
   for use as a general hash algorithm.

   Always returns an unsigned 32-bit integer.

.. note::
   To generate the same numeric value across all Python versions and
   platforms use adler32(data) & 0xffffffff.  If you are only using
   the checksum in packed binary format this is not necessary as the
   return value is the correct 32bit binary representation
   regardless of sign.


.. function:: compress(data[, level])

   Compresses the bytes in *data*, returning a bytes object containing compressed data.
   *level* is an integer from ``1`` to ``9`` controlling the level of compression;
   ``1`` is fastest and produces the least compression, ``9`` is slowest and
   produces the most.  The default value is ``6``.  Raises the :exc:`error`
   exception if any error occurs.


.. function:: compressobj([level])

   Returns a compression object, to be used for compressing data streams that won't
   fit into memory at once.  *level* is an integer from ``1`` to ``9`` controlling
   the level of compression; ``1`` is fastest and produces the least compression,
   ``9`` is slowest and produces the most.  The default value is ``6``.


.. function:: crc32(data[, value])

   .. index::
      single: Cyclic Redundancy Check
      single: checksum; Cyclic Redundancy Check

   Computes a CRC (Cyclic Redundancy Check)  checksum of *data*. If *value* is
   present, it is used as the starting value of the checksum; otherwise, a fixed
   default value is used.  This allows computing a running checksum over the
   concatenation of several inputs.  The algorithm is not cryptographically
   strong, and should not be used for authentication or digital signatures.  Since
   the algorithm is designed for use as a checksum algorithm, it is not suitable
   for use as a general hash algorithm.

   Always returns an unsigned 32-bit integer.

.. note::
   To generate the same numeric value across all Python versions and
   platforms use crc32(data) & 0xffffffff.  If you are only using
   the checksum in packed binary format this is not necessary as the
   return value is the correct 32bit binary representation
   regardless of sign.


.. function:: decompress(data[, wbits[, bufsize]])

   Decompresses the bytes in *data*, returning a bytes object containing the
   uncompressed data.  The *wbits* parameter controls the size of the window
   buffer, and is discussed further below.
   If *bufsize* is given, it is used as the initial size of the output
   buffer.  Raises the :exc:`error` exception if any error occurs.

   The absolute value of *wbits* is the base two logarithm of the size of the
   history buffer (the "window size") used when compressing data.  Its absolute
   value should be between 8 and 15 for the most recent versions of the zlib
   library, larger values resulting in better compression at the expense of greater
   memory usage.  When decompressing a stream, *wbits* must not be smaller
   than the size originally used to compress the stream; using a too-small
   value will result in an exception. The default value is therefore the
   highest value, 15.  When *wbits* is negative, the standard
   :program:`gzip` header is suppressed.

   *bufsize* is the initial size of the buffer used to hold decompressed data.  If
   more space is required, the buffer size will be increased as needed, so you
   don't have to get this value exactly right; tuning it will only save a few calls
   to :c:func:`malloc`.  The default size is 16384.


.. function:: decompressobj([wbits])

   Returns a decompression object, to be used for decompressing data streams that
   won't fit into memory at once.  The *wbits* parameter controls the size of the
   window buffer.

Compression objects support the following methods:


.. method:: Compress.compress(data)

   Compress *data*, returning a bytes object containing compressed data for at least
   part of the data in *data*.  This data should be concatenated to the output
   produced by any preceding calls to the :meth:`compress` method.  Some input may
   be kept in internal buffers for later processing.


.. method:: Compress.flush([mode])

   All pending input is processed, and a bytes object containing the remaining compressed
   output is returned.  *mode* can be selected from the constants
   :const:`Z_SYNC_FLUSH`,  :const:`Z_FULL_FLUSH`,  or  :const:`Z_FINISH`,
   defaulting to :const:`Z_FINISH`.  :const:`Z_SYNC_FLUSH` and
   :const:`Z_FULL_FLUSH` allow compressing further bytestrings of data, while
   :const:`Z_FINISH` finishes the compressed stream and  prevents compressing any
   more data.  After calling :meth:`flush` with *mode* set to :const:`Z_FINISH`,
   the :meth:`compress` method cannot be called again; the only realistic action is
   to delete the object.


.. method:: Compress.copy()

   Returns a copy of the compression object.  This can be used to efficiently
   compress a set of data that share a common initial prefix.


Decompression objects support the following methods, and two attributes:


.. attribute:: Decompress.unused_data

   A bytes object which contains any bytes past the end of the compressed data. That is,
   this remains ``""`` until the last byte that contains compression data is
   available.  If the whole bytestring turned out to contain compressed data, this is
   ``b""``, an empty bytes object.

   The only way to determine where a bytestring of compressed data ends is by actually
   decompressing it.  This means that when compressed data is contained part of a
   larger file, you can only find the end of it by reading data and feeding it
   followed by some non-empty bytestring into a decompression object's
   :meth:`decompress` method until the :attr:`unused_data` attribute is no longer
   empty.


.. attribute:: Decompress.unconsumed_tail

   A bytes object that contains any data that was not consumed by the last
   :meth:`decompress` call because it exceeded the limit for the uncompressed data
   buffer.  This data has not yet been seen by the zlib machinery, so you must feed
   it (possibly with further data concatenated to it) back to a subsequent
   :meth:`decompress` method call in order to get correct output.


.. method:: Decompress.decompress(data[, max_length])

   Decompress *data*, returning a bytes object containing the uncompressed data
   corresponding to at least part of the data in *string*.  This data should be
   concatenated to the output produced by any preceding calls to the
   :meth:`decompress` method.  Some of the input data may be preserved in internal
   buffers for later processing.

   If the optional parameter *max_length* is supplied then the return value will be
   no longer than *max_length*. This may mean that not all of the compressed input
   can be processed; and unconsumed data will be stored in the attribute
   :attr:`unconsumed_tail`. This bytestring must be passed to a subsequent call to
   :meth:`decompress` if decompression is to continue.  If *max_length* is not
   supplied then the whole input is decompressed, and :attr:`unconsumed_tail` is
   empty.


.. method:: Decompress.flush([length])

   All pending input is processed, and a bytes object containing the remaining
   uncompressed output is returned.  After calling :meth:`flush`, the
   :meth:`decompress` method cannot be called again; the only realistic action is
   to delete the object.

   The optional parameter *length* sets the initial size of the output buffer.


.. method:: Decompress.copy()

   Returns a copy of the decompression object.  This can be used to save the state
   of the decompressor midway through the data stream in order to speed up random
   seeks into the stream at a future point.


.. seealso::

   Module :mod:`gzip`
      Reading and writing :program:`gzip`\ -format files.

   http://www.zlib.net
      The zlib library home page.

   http://www.zlib.net/manual.html
      The zlib manual explains  the semantics and usage of the library's many
      functions.

