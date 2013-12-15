.. highlightlang:: c

.. _longobjects:

Integer Objects
---------------

.. index:: object: long integer
           object: integer

All integers are implemented as "long" integer objects of arbitrary size.

.. c:type:: PyLongObject

   This subtype of :c:type:`PyObject` represents a Python integer object.


.. c:var:: PyTypeObject PyLong_Type

   This instance of :c:type:`PyTypeObject` represents the Python integer type.
   This is the same object as :class:`int` in the Python layer.


.. c:function:: int PyLong_Check(PyObject *p)

   Return true if its argument is a :c:type:`PyLongObject` or a subtype of
   :c:type:`PyLongObject`.


.. c:function:: int PyLong_CheckExact(PyObject *p)

   Return true if its argument is a :c:type:`PyLongObject`, but not a subtype of
   :c:type:`PyLongObject`.


.. c:function:: PyObject* PyLong_FromLong(long v)

   Return a new :c:type:`PyLongObject` object from *v*, or *NULL* on failure.

   The current implementation keeps an array of integer objects for all integers
   between ``-5`` and ``256``, when you create an int in that range you actually
   just get back a reference to the existing object. So it should be possible to
   change the value of ``1``.  I suspect the behaviour of Python in this case is
   undefined. :-)


.. c:function:: PyObject* PyLong_FromUnsignedLong(unsigned long v)

   Return a new :c:type:`PyLongObject` object from a C :c:type:`unsigned long`, or
   *NULL* on failure.


.. c:function:: PyObject* PyLong_FromSsize_t(Py_ssize_t v)

   Return a new :c:type:`PyLongObject` object from a C :c:type:`Py_ssize_t`, or
   *NULL* on failure.


.. c:function:: PyObject* PyLong_FromSize_t(size_t v)

   Return a new :c:type:`PyLongObject` object from a C :c:type:`size_t`, or
   *NULL* on failure.


.. c:function:: PyObject* PyLong_FromLongLong(PY_LONG_LONG v)

   Return a new :c:type:`PyLongObject` object from a C :c:type:`long long`, or *NULL*
   on failure.


.. c:function:: PyObject* PyLong_FromUnsignedLongLong(unsigned PY_LONG_LONG v)

   Return a new :c:type:`PyLongObject` object from a C :c:type:`unsigned long long`,
   or *NULL* on failure.


.. c:function:: PyObject* PyLong_FromDouble(double v)

   Return a new :c:type:`PyLongObject` object from the integer part of *v*, or
   *NULL* on failure.


.. c:function:: PyObject* PyLong_FromString(char *str, char **pend, int base)

   Return a new :c:type:`PyLongObject` based on the string value in *str*, which
   is interpreted according to the radix in *base*.  If *pend* is non-*NULL*,
   *\*pend* will point to the first character in *str* which follows the
   representation of the number.  If *base* is ``0``, the radix will be
   determined based on the leading characters of *str*: if *str* starts with
   ``'0x'`` or ``'0X'``, radix 16 will be used; if *str* starts with ``'0o'`` or
   ``'0O'``, radix 8 will be used; if *str* starts with ``'0b'`` or ``'0B'``,
   radix 2 will be used; otherwise radix 10 will be used.  If *base* is not
   ``0``, it must be between ``2`` and ``36``, inclusive.  Leading spaces are
   ignored.  If there are no digits, :exc:`ValueError` will be raised.


.. c:function:: PyObject* PyLong_FromUnicode(Py_UNICODE *u, Py_ssize_t length, int base)

   Convert a sequence of Unicode digits to a Python integer value.  The Unicode
   string is first encoded to a byte string using :c:func:`PyUnicode_EncodeDecimal`
   and then converted using :c:func:`PyLong_FromString`.


.. c:function:: PyObject* PyLong_FromVoidPtr(void *p)

   Create a Python integer from the pointer *p*. The pointer value can be
   retrieved from the resulting value using :c:func:`PyLong_AsVoidPtr`.


.. XXX alias PyLong_AS_LONG (for now)
.. c:function:: long PyLong_AsLong(PyObject *pylong)

   .. index::
      single: LONG_MAX
      single: OverflowError (built-in exception)

   Return a C :c:type:`long` representation of the contents of *pylong*.  If
   *pylong* is greater than :const:`LONG_MAX`, raise an :exc:`OverflowError`,
   and return -1. Convert non-long objects automatically to long first,
   and return -1 if that raises exceptions.

.. c:function:: long PyLong_AsLongAndOverflow(PyObject *pylong, int *overflow)

   Return a C :c:type:`long` representation of the contents of
   *pylong*.  If *pylong* is greater than :const:`LONG_MAX` or less
   than :const:`LONG_MIN`, set *\*overflow* to ``1`` or ``-1``,
   respectively, and return ``-1``; otherwise, set *\*overflow* to
   ``0``.  If any other exception occurs (for example a TypeError or
   MemoryError), then ``-1`` will be returned and *\*overflow* will
   be ``0``.


.. c:function:: PY_LONG_LONG PyLong_AsLongLongAndOverflow(PyObject *pylong, int *overflow)

   Return a C :c:type:`long long` representation of the contents of
   *pylong*.  If *pylong* is greater than :const:`PY_LLONG_MAX` or less
   than :const:`PY_LLONG_MIN`, set *\*overflow* to ``1`` or ``-1``,
   respectively, and return ``-1``; otherwise, set *\*overflow* to
   ``0``.  If any other exception occurs (for example a TypeError or
   MemoryError), then ``-1`` will be returned and *\*overflow* will
   be ``0``.

   .. versionadded:: 3.2


.. c:function:: Py_ssize_t PyLong_AsSsize_t(PyObject *pylong)

   .. index::
      single: PY_SSIZE_T_MAX
      single: OverflowError (built-in exception)

   Return a C :c:type:`Py_ssize_t` representation of the contents of *pylong*.
   If *pylong* is greater than :const:`PY_SSIZE_T_MAX`, an :exc:`OverflowError`
   is raised and ``-1`` will be returned.


.. c:function:: unsigned long PyLong_AsUnsignedLong(PyObject *pylong)

   .. index::
      single: ULONG_MAX
      single: OverflowError (built-in exception)

   Return a C :c:type:`unsigned long` representation of the contents of *pylong*.
   If *pylong* is greater than :const:`ULONG_MAX`, an :exc:`OverflowError` is
   raised.


.. c:function:: size_t PyLong_AsSize_t(PyObject *pylong)

   Return a :c:type:`size_t` representation of the contents of *pylong*.  If
   *pylong* is greater than the maximum value for a :c:type:`size_t`, an
   :exc:`OverflowError` is raised.


.. c:function:: PY_LONG_LONG PyLong_AsLongLong(PyObject *pylong)

   .. index::
      single: OverflowError (built-in exception)

   Return a C :c:type:`long long` from a Python integer.  If *pylong*
   cannot be represented as a :c:type:`long long`, an
   :exc:`OverflowError` is raised and ``-1`` is returned.


.. c:function:: unsigned PY_LONG_LONG PyLong_AsUnsignedLongLong(PyObject *pylong)

   .. index::
      single: OverflowError (built-in exception)

   Return a C :c:type:`unsigned long long` from a Python integer. If
   *pylong* cannot be represented as an :c:type:`unsigned long long`,
   an :exc:`OverflowError` is raised and ``(unsigned long long)-1`` is
   returned.

   .. versionchanged:: 3.1
      A negative *pylong* now raises :exc:`OverflowError`, not :exc:`TypeError`.


.. c:function:: unsigned long PyLong_AsUnsignedLongMask(PyObject *io)

   Return a C :c:type:`unsigned long` from a Python integer, without checking for
   overflow.


.. c:function:: unsigned PY_LONG_LONG PyLong_AsUnsignedLongLongMask(PyObject *io)

   Return a C :c:type:`unsigned long long` from a Python integer, without
   checking for overflow.


.. c:function:: double PyLong_AsDouble(PyObject *pylong)

   Return a C :c:type:`double` representation of the contents of *pylong*.  If
   *pylong* cannot be approximately represented as a :c:type:`double`, an
   :exc:`OverflowError` exception is raised and ``-1.0`` will be returned.


.. c:function:: void* PyLong_AsVoidPtr(PyObject *pylong)

   Convert a Python integer *pylong* to a C :c:type:`void` pointer.
   If *pylong* cannot be converted, an :exc:`OverflowError` will be raised.  This
   is only assured to produce a usable :c:type:`void` pointer for values created
   with :c:func:`PyLong_FromVoidPtr`.
