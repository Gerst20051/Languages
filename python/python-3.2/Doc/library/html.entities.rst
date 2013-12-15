:mod:`html.entities` --- Definitions of HTML general entities
=============================================================

.. module:: html.entities
   :synopsis: Definitions of HTML general entities.
.. sectionauthor:: Fred L. Drake, Jr. <fdrake@acm.org>

**Source code:** :source:`Lib/html/entities.py`

--------------

This module defines three dictionaries, ``name2codepoint``, ``codepoint2name``,
and ``entitydefs``. ``entitydefs`` is used to provide the :attr:`entitydefs`
member of the :class:`html.parser.HTMLParser` class.  The definition provided
here contains all the entities defined by XHTML 1.0 that can be handled using
simple textual substitution in the Latin-1 character set (ISO-8859-1).


.. data:: entitydefs

   A dictionary mapping XHTML 1.0 entity definitions to their replacement text in
   ISO Latin-1.


.. data:: name2codepoint

   A dictionary that maps HTML entity names to the Unicode codepoints.


.. data:: codepoint2name

   A dictionary that maps Unicode codepoints to HTML entity names.
