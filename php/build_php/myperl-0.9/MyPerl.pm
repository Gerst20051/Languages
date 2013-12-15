package MyPerl::Request;

use 5.008;
use strict;
use warnings;

our $VERSION = '0.9';


1;
__END__

=head1 NAME

MyPerl - Perl extension for MySQL

=head1 SYNOPSIS

  CREATE FUNCTION myperl RETURNS STRING SONAME "my_perl.so";
  CREATE AGGREGATE FUNCTION myperlgroup RETURNS STRING SONAME "myperl.so";

=head1 DESCRIPTION

Basically this allows you to create functions
for MySQL using perl. You can store perl in a column (or
just pass it directly to the myperl function). 

 A sample call would look like:
 mysql> select myperl("$MYSQL_MYPERL_VALUE = 2*3");
 +-------------------------------------+
 | myperl("$MYSQL_MYPERL_VALUE = 2*3") |
 +-------------------------------------+
 | 6                                   |
 +-------------------------------------+
 1 row in set (0.01 sec)
 
 Another example:
 mysql> select myperl('return $ARGV[0]',item) from dog;
 +------------------------------------+
 | myperl('return $ARGV[0]',item)     |
 +------------------------------------+
 | eek                                |
 | eek                                |
 | eek                                |
 | eek                                |
 | eek                                |
 +------------------------------------+
 5 rows in set (0.01 sec)
 mysql> 
 
 It is also possible to do aggregate function calls.
 mysql> select myperlgroup(code),diff FROM perlcode GROUP BY diff;
 +-------------------+------+
 | myperlgroup(code) | diff |
 +-------------------+------+
 | 5                 |    1 |
 | 5                 |    2 |
 +-------------------+------+
 2 rows in set (0.03 sec)
 
 Where the data was:
 ql> select * FROM perlcode ;
 +----+------+---------------------------------------+
 | id | diff | code                                  |
 +----+------+---------------------------------------+
 |  1 |    1 | $x = $ARGV[0] ? $ARGV[0] :0; $x++; $x |
 |  2 |    1 | $x = $ARGV[0] ? $ARGV[0] :0; $x++; $x |
 |  3 |    1 | $x = $ARGV[0] ? $ARGV[0] :0; $x++; $x |
 |  4 |    1 | $x = $ARGV[0] ? $ARGV[0] :0; $x++; $x |
 |  5 |    1 | $x = $ARGV[0] ? $ARGV[0] :0; $x++; $x |
 |  6 |    2 | $x = $ARGV[0] ? $ARGV[0] :0; $x++; $x |
 |  7 |    2 | $x = $ARGV[0] ? $ARGV[0] :0; $x++; $x |
 |  8 |    2 | $x = $ARGV[0] ? $ARGV[0] :0; $x++; $x |
 |  9 |    2 | $x = $ARGV[0] ? $ARGV[0] :0; $x++; $x |
 | 10 |    2 | $x = $ARGV[0] ? $ARGV[0] :0; $x++; $x |
 +----+------+---------------------------------------+
 10 rows in set (0.00 sec)

For each sql call a perl interpreter is currently created; this interpreter persists for the connection of the sql call.

=head2 EXPORT

None

=head1 SEE ALSO

More information can be found at http://software.tangent.org/

=head1 AUTHOR

Brian Aker, E<lt>brian@tangent.orgE<gt>

=head1 COPYRIGHT AND LICENSE

Copyright 2003 by Brian Aker

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself. 

=cut
