use 5.008;
use ExtUtils::MakeMaker;
use ExtUtils::Embed;
my @clean = qw|
	perlxsi.c
	myperl.so
|;

my $EXTRA_CFLAGS = `mysql_config --cflags`;

xsinit();

WriteMakefile(
		'NAME'		=> 'myperl',
		'VERSION_FROM'	=> 'MyPerl.pm',
		($] >= 5.005 ?    ## Add these new keywords supported since 5.005
		 (ABSTRACT => 'Embedded perl interpreter for MySQL.',
		  AUTHOR     => 'Brian Aker <brian@tangent.org>') : ()),
		DEFINE => $EXTRA_CFLAGS,
		macro   => {
			CVSROOT => 'cvs.tangent.org:/cvs',
		},
		clean   => {
			FILES   => "@clean",
		},
	     );

sub MY::top_targets {
	my $self = shift;
	my $string;# = $self->MM::top_targets;
	$string .= <<'EOF';

SRC=src/myperl.c perlxsi.c
MY_PERL_INC = `$(PERL) -MExtUtils::Embed -e perl_inc` `$(PERL) -MExtUtils::Embed -e ccopts`
LIBPERL_PATH = `$(PERL) -MConfig -e 'print $$Config{archlib}'`
MY_CFLAGS=-shared -Wall -O $(DEFINE) $(MY_PERL_INC)
DISTNAME = myperl
DISTVNAME = $(DISTNAME)-$(VERSION)

LFLAGS=`$(PERL) -MExtUtils::Embed -e ldopts`


all :: pure_all manifypods
	@$(NOOP)

pure_all :: config pm_to_blib subdirs libperl_path myperl
	@$(NOOP)

subdirs :: $(MYEXTLIB)
	@$(NOOP)

config :: Makefile $(INST_LIBDIR)/.exists
	@$(NOOP)

config :: $(INST_ARCHAUTODIR)/.exists
	@$(NOOP)

config :: $(INST_AUTODIR)/.exists
	@$(NOOP)

$(INST_AUTODIR)/.exists :: 
	@$(MKPATH) $(INST_AUTODIR)

	-@$(CHMOD) $(PERM_RWX) $(INST_AUTODIR)

$(INST_LIBDIR)/.exists ::
	@$(MKPATH) $(INST_LIBDIR)

	-@$(CHMOD) $(PERM_RWX) $(INST_LIBDIR)

$(INST_ARCHAUTODIR)/.exists :: 
	@$(MKPATH) $(INST_ARCHAUTODIR)

	-@$(CHMOD) $(PERM_RWX) $(INST_ARCHAUTODIR)

myperl: 
	$(CC) $(MY_CFLAGS) $(SRC) \
                $(LFLAGS) -o myperl.so

shared_install: myperl
	cp myperl.so /usr/lib

version:
	echo $(VERSION) > VERSION
	echo "#define VERSION \"$(VERSION)\"" > src/version.h

libperl_path:
	echo "#define LIBPERL_PATH \"$(LIBPERL_PATH)\"" > src/libperl_path.h

load : myperl
	echo "DROP FUNCTION myperl" | mysql -f -u root
	echo "DROP FUNCTION myperlgroup" | mysql -f -u root
	cp myperl.so /usr/lib
	echo "CREATE FUNCTION myperl RETURNS STRING SONAME \"myperl.so\"" | mysql -f -u root
	echo "CREATE AGGREGATE FUNCTION myperlgroup RETURNS STRING SONAME \"myperl.so\"" | mysql -f -u root

drop :
	echo "DROP FUNCTION myperl" | mysql -f -u root
	echo "DROP FUNCTION myperlgroup" | mysql -f -u root

help:
        perldoc ExtUtils::MakeMaker

rpm: #all dist
	cp $(DISTVNAME).tar$(SUFFIX) /usr/src/redhat/SOURCES
	rpmbuild -ba MyPerl.spec
	cp /usr/src/redhat/RPMS/i386/$(DISTVNAME)*.i386.rpm .
	cp /usr/src/redhat/SRPMS/$(DISTVNAME)*.src.rpm .

EOF
	return $string;
}

sub MY::install {
	my $self = shift;
	my $string = $self->MM::install;
	my $add = "shared_install";
	$string =~ s/(pure_install\s+)(.*)/$1 $add $2/;

	return $string;
}
