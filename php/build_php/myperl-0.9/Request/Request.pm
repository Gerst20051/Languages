package MyPerl::Request;

use 5.008;
use strict;
use warnings;

our $VERSION = '1.00';

my %Cache;
sub handler {
	my $code = shift; #We know the first bit will be the code that we are looking to run
	unless($Cache{$code}) {
		$Cache{$code} = eval "sub { $code }";
	}

	if ($@) { # Just return on error;
		print STDERR "my_perl error: $@\n";
		return;
	}

	return $Cache{$code}->(@_);
}


1;
__END__

=head1 NAME

MyPerl::Request - Perl extension for MySQL

=head1 SYNOPSIS

  use MyPerl::Request;

=head1 ABSTRACT

MyPerl::Request does all of the heavy lifting for the embedded perl interpretor in 
MySQL. It handles each request, purging itself at the end of the request, but making sure
that through a single request that each chunk of perl code is cached. 

=head1 DESCRIPTION

This package handles each request to the embedded perl interpretor in MySQL. It caches each compiled script until the end of the request.

=head2 EXPORT

None

=head1 SEE ALSO

More information can be found at http://software.tangent.org/

=head1 AUTHOR

Brian Aker, E<lt>brian@E<gt>

=head1 COPYRIGHT AND LICENSE

Copyright 2003 by Brian Aker

This library is free software; you can redistribute it and/or modify
it under the same terms as Perl itself. 

=cut
