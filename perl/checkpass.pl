#!/usr/local/bin/perl 
# Copyright 1996 PHRI Mark Bergman
# Script to check passwords for strength. Does NOTHING with the password
# besides "checking" so it's "safe". Of course, in a multi-user
# environment, the security of this program is only as good as the 
# communications path used to access the script and the machine it runs
# on...
eval 'exec /usr/local/bin/perl -S $0 ${1+"$@"}'
    if $running_under_some_shell;

#use diagnostics;
use integer;
use vars qw($ohome $home $gid $shell $nmon $nmday);
require 'look.pl';
require "flush.pl"; # for flush()
$running_under_some_shell="";
$RCSversion='$Header: /home/bergman/Bin/RCS/checkpass,v 1.7 2001/05/30 19:17:04 bergman Exp bergman $';
############### Initializations #######################################
#
#
$EXPLICIT="NO WAY, MAN!";
$DEBUG = 0;
$DELAY=.25;	# Delay to insert between checks...useful if the program
			# is really being used to change passwords, and we want to
			# slow things down enough to prevent brute forcing, and to
			# give users enough time to read the diagnostics
$PASSMIN=7; # Minimum password length
$PASSMAX=8; # Maximum password length
@words=('/usr/share/dict/words','/usr/dict/words','./badwords');
#
#
####################   START OF STANDARD SUBROUTINES ###################
($NAME=$0)=~s#^.*/##;
sub version
{
# Display the version number and date of modification extracted from
# the RCS header.
    $RCSversion=~s/\S+\s\S+\s(\S+\s\S+).*/$1/;
    print $0 . ": " . $RCSversion . "\n";
    exit 0
}
sub limits
{
    print "$NAME: No internal limits beyond perl\n";
	print "\n";
	print "Requires:\n";
	print "\tflush.pl\tto flush() stderr\n";
	print "\tlook.pl\tfor dictionary lookups\n";
    exit 0;
    }
sub usage
{
	print "$NAME [-V|-L] [-x] [-M maximum_pass] [-m minimum_pass] [-d dict]\n";
	print "\n";
	print "-V\tversion information\n";
	print "-L\tlimitations\n";
	print "-x\tbe explicit about rejected passwords.\n\t** May expose your desired password in clear text. **\n";
	print "-M\tspecify maximum password length (default $PASSMAX)\n";
	print "-m\tspecify minimum password length, must be &gt; 1 (default $PASSMIN)\n";
	print "-d\tadditional dictionaries, defaults to:\n\t\t@words\n";
	print "\n";
	print "While the old password is prompted for, it's only used to ensure that\n";
	print "the new one differs significantly; anything can be entered.\n";
	print "\n";
	print "Nothing is stored on disk, and no external processes are called; the\n";
	print "program is reasonably secure. Passwords are vulnerable to being read\n";
	print "from memory while the program is running, so it should only be run on\n";
	print "a secure, single-user machine.\n";
	print "\t@_\n";
    exit 1;
}
sub debug
{
	# Handle debugging. The debug routine depends on the presence of
	# the variable "DEBUG", which should be set as follows:
	#	=\d			only print debug statements that exactly match
	#				the specified level
	#
	#	\d			print debug statements at or less than the level

    local($level,$statement)=@_;
	my $debug=$DEBUG;

	if ( $DEBUG !~ /=\d+/ )
	{
		if ( $debug &gt;= $level )
		{
			print $statement;
		}
	}
	else
	{
		$debug=~s/=//;
		if ( $debug == $level )
		{
			print $statement;
		}
	}
}
sub myGetopts
{
# Usage:
#      do Getopts('a:bc');  # -a takes arg. -b &amp; -c not. Sets opt_* as a
#                           #  side effect.
    local($argumentative) = @_;
    local(@args,$_,$first,$rest);
    local($errs) = 0;

    @args = split( / */, $argumentative );
    while(@ARGV &amp;&amp; ($_ = $ARGV[0]) =~ /^-(.)(.*)/) {
	($first,$rest) = ($1,$2);
	$pos = index($argumentative,$first);
	if($pos &gt;= 0) {
	    if($pos &lt; $#args &amp;&amp; $args[$pos+1] eq ':') {
		shift(@ARGV);
		if($rest eq '') {
		    ++$errs unless @ARGV;
		    $rest = shift(@ARGV);
		}
		${"opt_$first"} = $rest;
	    }
	    else {
		${"opt_$first"} = 1;
		if($rest eq '') {
		    shift(@ARGV);
		}
		else {
		    $ARGV[0] = "-$rest";
		}
	    }
	}
	else {
	    return(1);
	    ++$errs;
	    if($rest ne '') {
		$ARGV[0] = "-$rest";
	    }
	    else {
		shift(@ARGV);
	    }
	}
    }
	return(0);
}
sub parse
{
	if ( myGetopts("VLm:M:d:x") != 0 )
	{
		usage("Invalid option");
	}

	if ( defined($opt_V) &amp;&amp; $opt_V == 1 )
	{
		&amp;version();
	}
	if ( defined($opt_L) &amp;&amp; $opt_L == 1 )
	{
		&amp;limits();
	}
	if ( defined($opt_d) &amp;&amp; "$opt_d" ne "1" )
	{
		push(@words,$opt_d);	# Add the specified dictionary to the array
	}      
	if ( defined($opt_M) &amp;&amp; "$opt_M" ne "1" )
	{
		$PASSMAX=$opt_M;
	}      
	if ( defined($opt_m) &amp;&amp; "$opt_m" ne "1" )
	{
		$PASSMIN=$opt_m;
	}      
	if ( defined($opt_x) &amp;&amp; $opt_x == 1 )
	{
		$EXPLICIT="YES";
	}      
}

####################   END OF STANDARD SUBROUTINES

parse();
debug(2,"\$EXPLICIT=\"$EXPLICIT\"\n");
select (STDOUT) ; $| = 1;   # Un-buffer ouput.
select (STDERR) ; $| = 1;   # Un-buffer ouput.
$;="\000";          # Set delim for arrays to be NUL.



# Security blankets.
$path="";
$ENV{'IFS'} = '' if $ENV{'IFS'};
$ENV{'PATH'} = '/bin:/usr/bin:/usr/local/bin:/usr/ucb:/usr/lib/nis';
$path=$ENV{'PATH'};
$ENV{'SHELL'}='/bin/sh';
umask (022);
chop($host =`/bin/hostname`);

# Make a list of dictionaries to search with &amp;look
$fh = 'dictaa';
foreach $dict (@words)
{
    open($fh,$dict) &amp;&amp; push(@dicts, eval "*$fh");
    $fh++;
}

# Get passwd entry and remember all logins
$login = '';
$me = getlogin || getpwuid($&lt;) unless $me; 
open(PASSWD, "&lt;/etc/passwd") || die "Can't open /etc/passwd";
while (&lt;PASSWD&gt;)
{
    /^([^:]+)/;
    if ($1 eq $me)
    {
        ($ologin,$opasswd,$ouid,$ogid,$ogcos,$ohome,$oshell) = split(/:/);
        $login=$ologin;
        $uid=$ouid;
        $gid=$ogid;
        $home=$ohome;
        $shell=$oshell;

        die "You aren't you! ($&lt; $uid $me $login)\n"
            if $&lt; &amp;&amp; $&lt; !=  $uid;       # just being paranoid...
        $salt = substr($opasswd,0,2);

        # Canonicalize  name.

        $ogcos =~ s/,.*//;
        $mynames = $ogcos;
        $mynames =~ s/&amp;/$login/g;
        $mynames =~ s/\W+/ /;
        $mynames =~ s/^ //;
        $mynames =~ s/ $//;
        $mynames =~ s/ . / /g;
        $mynames =~ s/ . / /g;
        $mynames =~ s/^. //;
        $mynames =~ s/ .$//;
        $mynames =~ s/ /|/;
        $mynames = '^$' if $mynames eq '' ;
    }
    ++$isalogin{$1} if length ($1) &gt;= 6;
}
close(PASSWD);
die "$me isn't in the passwd file.\n" unless $login;


# Finally we can begin.

# Turn off echo
system 'stty', '-echo';
if ($&lt;)
{
    print "Old password: " ;
    chop($pass0 = &lt;STDIN&gt;);
system 'stty', 'echo';
    print "\n";

    # Note: We shouldn't use die while echo is off.

    myexit(1) unless $pass0;
}


# Pick a password
for (;;)
{
    $goodenough = 0;
    until ($goodenough)
    {
        $permcheck="no";
        $delay=$DELAY;
        system 'stty', '-echo';
        print "New password: ";
        chop($pass1 = &lt;STDIN&gt;);
        system 'stty', 'echo';
        print "\n";
        myexit(1) unless $pass1;
        #print "\tChecking for poor passwords.\n";
        $goodenough = &amp;goodenough($pass1);
        if ( !  $goodenough  )
        {
            print "";
            select(undef,undef,undef,0.5);
            print "";
        }
        print "\n";

        # If longer than $PASSMAX chars, check first $PASSMAX chars alone.
    
        if ($goodenough &amp;&amp; length($pass1) &gt; $PASSMAX)
        {
            $passtruncated = substr($pass1,0,$PASSMAX);
            print "\tRechecking first $PASSMAX ...\n";
            unless ($goodenough = &amp;goodenough($passtruncated))
            {
                print "\n(Note that only the first $PASSMAX  count.)\n";
                print "";
                select(undef,undef,undef,0.5);
                print "";
            }
            #print "\n";
        }

        if ( $goodenough &amp;&amp; (  grep(/[0-9@!]/,$pass1) gt 0  ) )
        {
            # There are numbers in the password...
            # Permute the numbers into letters, re-check
            # each permutation.
            $permcheck="yes";
            $delay="0";
			$noprefix="";
			$nosuffix="";
			if ( $pass1 =~ /^[^A-Za-z]/ )
			{
				($noprefix = $pass1) =~ s/^[^A-Za-z]*//;
				if ( (length($noprefix) &lt;= 2 ))
				{
					$noprefix="";
				}
			}
			if ( $pass1 =~ /[^A-Za-z]$/ )
			{
				($nosuffix = $pass1) =~ s/[^A-Za-z]*$//;
				if ( (length($nosuffix) &lt;= 2 ))
				{
					$nosuffix="";
				}
			}
			if ( $noprefix =~ /[^A-Za-z]$/ )
			{
				($noprenosuffix = $noprefix) =~ s/[^A-Za-z]*$//;
				if ( (length($nosuffix) &lt;= 2 ))
				{
					$noprenosuffix="";
				}
			}

            &amp;permute("$pass1 $noprefix $nosuffix $noprenosuffix");
            print "\tChecking $#tumbled permutations";
			$PASSMIN=2;	# Reset the minimum length to deal with the
				# tumbled passwords that are stripped on non-alphabetic
				# prefixes and suffixes.
				
            foreach $word (@tumbled ) 
            {
                print ".";
                if ( $word =~ /^\S+$/ )
                {
					debug(5,"Checking \"$word\" returned from permuting numbers into letters.\n");
                    $goodenough = &amp;goodenough($word);
                    if (! $goodenough )
                    {
						if ( $EXPLICIT =~  /^YES$/ )
						{
                        	print "\nA permutation of numbers into letters results in a weak password ($word).\n";
						}
						else
						{
                        	print "\nA permutation of numbers into letters results in a weak password.\n";
						}
                        print "";
                        select(undef,undef,undef,0.5);
                        print "";
                        exit 3;
                    }
                }
            }
            print "!\n";
        }
        if (  $goodenough )
        {
            print "\tNot a weak password after permutating numbers into letters...good.\n";
            # print "\n";
        }
    };


    print "\tYour password was well-chosen.\n";
    system 'stty', '-echo';
    print "Confirm new passwd: ";
    chop($pass2 = &lt;STDIN&gt;);
    system 'stty', 'echo';
    print "\n";
    if ($pass1 eq $pass2)
    {
        last;
    }
    else
    {
        print "Password mismatch-try again.\n";
        exit 3;
    }
}

# Turn the echo back on
system 'stty', 'echo';

###################################################
# This subroutine is the whole reason for this program. It  #
# checks for many different kinds of bad password. We   #
# don't tell people what kind of pattern they MUST have,    #
# because that would reduce the search space unnecessarily. #
#
# goodenough( ) returns 1 if password passes muster, else 0.    #
###################################################

sub goodenough
{
    $pass = shift (@_);
    $mono = $pass !~/^.+([A-Z].*[a-z]|[a-z].*[A-Z])/;
    $mono = 0 if $pass =~ /[^a-zA-Z0-9 ]/;

    $now = time;
    $nsec="";
    $nmin="";
    $nhour="";
    ($nsec,$nmin,$nhour,$nmday,$nmon,$nyear) = localtime($now);

    if ( $permcheck ne "yes" )
    {
        if ( $pass =~ /^[A-Za-z]+$/ )
        {
            print "\nYour password must contain at least one character that's not a letter.\n";
            return 0;
        }
        print "\tNot just letters...good.\n";
        select(undef,undef,undef,$delay);
    }
        
    # Embedded null can spoof crypt routine.
    if ($pass =~ /\0/)
    {
        print "\nPlease don't use the null character in your password.\n";
        return 0;
    }
    if ( $permcheck ne "yes" ) { print "\tNo NULL characters...good.\n"; }
    select(undef,undef,undef,$delay);

    # Same password they just had?

    if (crypt($pass,$salt) eq $opasswd)
    {
        print "\nPlease use a different password than you just had.\n";
        return 0;
    }
    if ( $permcheck ne "yes" ) { print "\tNot your previous password...good.\n"; }
    select(undef,undef,undef,$delay);
    # Too much like the old password?

    if ($pass0 &amp;&amp; length($pass0) == length($pass))
    {
        $diff = 0;
        for ($i = length($pass)-1; $i &gt;= 0; --$i)
        {
            ++$diff if substr($pass,$i,1) ne substr($pass0,$i,1);
        }
        if ($diff &lt;= 2)
        {
            print "\nThat's too close to your old password.\n";
            return  0;
        }
    }
    if ( $permcheck ne "yes" ) {  print "\tNot too similar to your previous password...good.\n";}
    select(undef,undef,undef,$delay);
    
    # Too short? Get progressively nastier.
    
    if (length($pass) &lt; $PASSMIN)
    {
        printf "Too short.\n";
        exit 4;
    }
    if ( $permcheck ne "yes" ) {  print "\tNot too short...good.\n";}
    select(undef,undef,undef,$delay);
    
    # Is it in one of the dictionaries?
    
    ($foo = $pass) =~ y/A-Z/a-z/;

    # Truncate common suffixes before searching dict.
    
    $shorte = '';
    $short = $pass;
    $even = ($short =~ s/\d+$//) ? " (even with a number) " : "";
    $short =~ s/s$//;
    $short =~ s/ed$// &amp;&amp; ($shorte = "${short}e");
    $short =~ s/er$// &amp;&amp; ($shorte = "${short}e");
    $short =~ s/ly$//;
    $short =~ s/ing$// &amp;&amp; ($shorte = "${short}e");
    ($cshort = $short) =~ y/A-Z/a-z/;
    
    # We'll iterate over several dictionaries.

    @tmp = @dicts;
    while ($dict = shift(@tmp))
    {
        local(*DICT) = $dict;
			debug(2,"Checking in \"$dict\" for \"$short\"\n");
    
        # Do the lookup (dictionary order, case folded)
    
        &amp;look($dict,$short,1,1);
			debug(2,"Checking in \"$dict\" for \"$short\"\n");
        while (&lt;DICT&gt;)
        {
            ($cline = $_) =~ y/A-Z/a-z/;
            last if substr($cline,0,length($short)) ne $cshort;
            chop;
            ($_,$response) = split(/\t+/);
            if ($pass eq $_ ||
                ($pass eq substr($_,0,$PASSMAX)) ||
                ($pass =~ /^$_$/i &amp;&amp; $mono) ||
                $shorte eq $_ ||
                ($shorte =~ /^$_$/i &amp;&amp; $mono)   ||
                $short eq $_ ||
                ($short =~ /^$_$/i &amp;&amp; $mono))
            {
                if ($response)
                {   # Has a snide remark.
                    if ( $permcheck ne "yes" ) { print $response, "\nPlease try again.\n"; }
                }
    
                elsif (/^[A-Z]/)
                {
                    if (/a$|ie$|yn$|een$|is$/)
                    {
                        if ( $permcheck ne "yes" ) { print "\nDon't you use HER name that way!\n"; }
                    }
                    else
                    {
                        $also = ' also';
                        if ( $permcheck ne "yes" ) { print "\nThat name is$also too popular.\n"; }
                    }
                }
                else
                {
                    if ( $permcheck ne "yes" ) { print "\nPlease avoid words in the dictionary$even.\n"; }
                }
                return 0;
            }
        }
    }
    if ( $permcheck ne "yes" ) {  print "\tNot in the dictionary...good.\n";}
    select(undef,undef,undef,$delay);

    $ssap=reverse $pass;
    ($foo = $ssap) =~ y/A-Z/a-z/;

    # Truncate common suffixes before searching dict.
    
    $shorte = '';
    $short = $ssap;
    $even = ($short =~ s/\d+$//) ? " (even with a number) " : "";
    $short =~ s/s$//;
    $short =~ s/ed$// &amp;&amp; ($shorte = "${short}e");
    $short =~ s/er$// &amp;&amp; ($shorte = "${short}e");
    $short =~ s/ly$//;
    $short =~ s/ing$// &amp;&amp; ($shorte = "${short}e");
    ($cshort = $short) =~ y/A-Z/a-z/;
    
    # We'll iterate over several dictionaries.

    @tmp = @dicts;
    while ($dict = shift(@tmp))
    {
        local(*DICT) = $dict;
    
        # Do the lookup (dictionary order, case folded)
    
        &amp;look($dict,$short,1,1);
			debug(2,"Checking in \"$dict\" for \"$short\"\n");
        while (&lt;DICT&gt;)
        {
            ($cline = $_) =~ y/A-Z/a-z/;
            last if substr($cline,0,length($short)) ne $cshort;
            chop;
            ($_,$response) = split(/\t+/);
            if ($ssap eq $_ ||
                ($ssap eq substr($_,0,$PASSMAX)) ||
                ($ssap =~ /^$_$/i &amp;&amp; $mono) ||
                $shorte eq $_ ||
                ($shorte =~ /^$_$/i &amp;&amp; $mono)   ||
                $short eq $_ ||
                ($short =~ /^$_$/i &amp;&amp; $mono))
            {
                if ($response)
                {   # Has a snide remark.
                    if ( $permcheck ne "yes" ) { print $response, " Please try again.\n"; }
                }
    
                elsif (/^[A-Z]/)
                {
                    if (/a$|ie$|yn$|een$|is$/)
                    {
                        if ( $permcheck ne "yes" ) { print "\nDon't you use HER name that way!\n"; }
                    }
                    else
                    {
                        $also = ' also';
                        if ( $permcheck ne "yes" ) { print "\nThat name is$also too popular.\n"; }
                    }
                }
                else
                {
                    print "\nPlease avoid reversed words in the dictionary$even.\n";
                }
                return 0;
            }
        }
    }
    if ( $permcheck ne "yes" ) {  print "\tNot in the dictionary reversed...good.\n";}
    select(undef,undef,undef,$delay);
    
    # Now check for two word-combinations.  This gets hairy.
    # We look up everything that starts with the same first
    # two letters as the password, and if the word matches the
    # head of the password, we save the rest of the password
    # in %others to be looked up later. Passwords which have
    # a single char before or after a word are special-cased.
    
    # We take pains to disallow things like "CamelAte",
    # "CameLate" and "CamElate" but allow things like
    # "CamelatE" or "CameLAte".
    
    # If the password is exactly $PASSMAX characters, we also have
    # to disallow passwords that consist of a word plus the
    # BEGINNING of another word, such as "CamelFle", which
    # will warn you about "camel" and "flea".
    
    %others = ( );
    ($cpass = $pass) =~ y/A-Z/a-z/;
    ($oneup) = $pass =~ /.[a-z]*([A-Z][a-z]*)$/;
    $cpass =~ s/ //g;
    if ($pass !~ /.+[A-Z].*[A-Z]/)
    {
        $others{substr($cpass,1,999)}++
        if $pass =~ /^..[a-z]+$/;
        @tmp = @dicts;
        while ($dict = shift(@tmp))
        {
            local(*DICT) = $dict;
            $two = substr($cpass,0,2);
            &amp;look($dict,$two,1,1);
			debug(2,"Checking in \"$dict\" for \"$two\"\n");
            $two++;
            word: while (&lt;DICT&gt;)
            {
                chop;
                s/\t.*//;
                y/A-Z/a-z/;
                last if $_ ge $two;
                if (index($cpass,$_) == 0)
                {
                    $key = substr($cpass,length($_),999);
                    next word if $key =~ /\W/;
                    $others{$key}++ unless $oneup
                    &amp;&amp; length($oneup) != length($key);
                }
            }
        }
        @tmp = @dicts;
        while ($dict = shift(@tmp))
        {
            local(*DICT) = $dict;
            foreach $key (keys(%others))
            {
                &amp;look($dict,$key,1,1);
			debug(2,"Checking in \"$dict\" for \"$key\"\n");
                $_ = &lt;DICT&gt;;
                chop;
                s/\t.*//;
                if ($_ eq $key || length($pass) == $PASSMAX &amp;&amp; /^$key/)
                {
                    $pre = substr($cpass,0,length($cpass) - length($key));
                    if (length($pre) == 1)
                    {
                        $pre = sprintf("^%c", ord($pre)^64) unless $pre =~ /[ -~]/;
                        print "\nOne char \"$pre\" plus a word like \"$_\" is too easy to guess.\n";
                        return 0;
                    }
					if ( $EXPLICIT eq "YES" )
					{
                    	print "\nPlease avoid two-word combinations like \"$pre\" and \"$_\".\n"; 
					}else
					{
                    	print "\nPlease avoid two-word combinations.\n"; 
					}
                    #if ( $permcheck ne "yes" ) { print "\nSuggestion: insert a random character in one of the words,\n"; }
                    #if ( $permcheck ne "yes" ) { print "\nor misspell one of them.\n"; }
                    return 0;
                }
                elsif (length($key) == 1 &amp;&amp; $pass =~ /^.[a-z]+.$/)
                {
                    chop($pre = $cpass);
                    $key = sprintf("^%c", ord($key)^64) unless $key =~ /[-~]/;
                    print "\nA word like \"$pre\" plus one char \"$key\" is too easy to guess.\n";
                    return 0;
                }
            }
        }
    }
    if ( $permcheck ne "yes" ) {  print "\tNot a dictionary word with a simple change...good.\n";}
    select(undef,undef,undef,$delay);
    
    #   Check for naughty words. :-)
    
    if ($pass =~ /(fuck|shit|piss|damn|hell|cunt|unix|dick|ibm|dec|sun|at&amp;t|nasa|mac)/i)
    {
		if ( $EXPLICIT eq "YES" )
		{
        	print "\nA common substring such as \"$1\" makes your password too easy to guess.\n";
		}
		else
		{
        	print "\nA common substring makes your password too easy to guess.\n";
		}
        return 0;
    }
    if ( $permcheck ne "yes" ) {  print "\tNot a common substring...good.\n";}
    select(undef,undef,undef,$delay);

    # Does it look like a date?
    
    if ($pass =~ m!^[-\d/]*$!)
    {
        if ($pass =~ m!^\d{3}-\d{2}-\d{4}$! || $pass =~ m!-\d\d\d\d\d\d\d\d\d$!)
        {
            print "\nPlease don't use a pattern similar to a Social Security Number!\n";
            return  0;
        }
	if ( $pass =~ /^\$[0-9]*\.[0-9]*[+-]*.*tax/i )
	{
		print "\nPlease don't use a pattern similar to a sticker price\n";
		return 0;
	}

        if ($pass =~ m!^\d*/\d*/\d*$! ||
            $pass =~ m!^\d*-\d*-\d*$! ||
            $pass =~ m!$nyear$!)
        {
            print  "Please don't use dates.\n";
            return  0;
        }
        if ($pass =~ m!^\d\d\d-?\d\d\d\d$!)
        {
            print "\nPlease don't use a phone number.\n";
            return  0;
        }
        if ($pass =~ m!^\d{6,9}$!)
        {
            if ( $permcheck ne "yes" ) { print "\nPlease don't use a sequence of numbers.\n"; }
            return  0;
        }
    }
    
    if ($mo = ($pass =~ /^[ \d]*([a-zA-Z]{3,5})[ \d]*$/) &amp;&amp;
        ($mo =~ /^(jan|feb|mar(ch)?|apr(il)?|may|june?)$/i ||
        $mo =~  /^(july?|aug|sept?|oct|nov|dec)$/i) )
    {
        print  "Please don't use dates.\n";
        return 0;
    }
    if ( $permcheck ne "yes" ) {  print "\tNot a pattern similar to a date/SSN/short number/phone number...good.\n";}
    select(undef,undef,undef,$delay);
    
    # Login id?
    
    if ($pass =~ /$me/i)
    {
        print "\nPlease don't use part your name.\n";
        return 0;
    }
    if ( $permcheck ne "yes" ) {  print "\tNot your login name...good.\n";}
    select(undef,undef,undef,$delay);
    
    # My own    name?
    
    if ($pass =~ /$mynames/i)
    {
        print "\nPlease don't use part of your name.\n";
        return 0,
    }
    if ( $permcheck ne "yes" ) {  print "\tNot part of your name...good.\n";}
    select(undef,undef,undef,$delay);

    # My host   name?
    
    if ($pass =~ /$host/i)
    {
        print "\nPlease don't use your host name.\n";
        return 0;
    }
    if ( $permcheck ne "yes" ) {  print "\tNot the name of the computer you are using...good.\n";}
    select(undef,undef,undef,$delay);

        # License plate number?
    
    if ($pass =~ /^\d?[a-zA-Z][a-zA-Z][a-zA-Z]\d\d\d$/  ||
        $pass =~ /^\d\d\d[a-zA-Z][a-zA-Z][a-zA-Z]$/ ||
        $pass =~ /^\d\d\d[a-zA-Z][a-zA-Z][a-zA-Z]$/ ||
        $pass =~ /^[a-zA-Z][a-zA-Z][a-zA-Z]\d\d\d$/ )
    {
        print "\nPlease don't use pattern similar to a license plate number.\n";
        return 0;
    }
    if ( $permcheck ne "yes" ) {  print "\tNot a license-plate pattern...good.\n";}
    select(undef,undef,undef,$delay);

	if ( $pass =~ /^\$[0-9]*\.[0-9]*[+-]*.*tax/i )
	{
		print "\nPlease don't use a pattern similar to a sticker price\n";
		return 0;
	}
    if ( $permcheck ne "yes" ) {  print "\tNot a sticker price pattern...good.\n";}
    select(undef,undef,undef,$delay);
    
    # A function key? (This pattern checks Sun-style fn keys.)
    
    if ($pass =~ /^\033\[\d+/)
    {
        print "\nPlease don't use a function key.\n";
        return 0;
    }
    if ( $permcheck ne "yes" ) {  print "\tDoes not contain SUN function key characters...good.\n";}
    select(undef,undef,undef,$delay);
    
    # A sequence of closely related ASCII characters?
    
    @ary = unpack('C*',$pass);
    $ok = 0;
    for ($i = 0; $i &lt; $#ary; ++$i)
    {
        $diff = $ary[$i+1] - $ary[$i];
        $ok = 1 if $diff &gt; 1 || $diff &lt; -1;
    }
    if (!$ok)
    {
        print "\nPlease don't use ASCII sequences.\n";
        return 0;
    }

    if ( $permcheck ne "yes" ) {  print "\tDoes not contain a sequence of ASCII characters...good.\n";}
    select(undef,undef,undef,$delay);
    
    # A sequence of keyboard keys?
    
    ($foo = $pass) =~ y/A-Z/a-z/;
    $foo =~ y/qwertyuiop[ ]asdfghjkl; 'zxcvbnm,.\//a-la-ka-j/;
    $foo =~ y/=-0987654321\][poiuytrewq';lkjhgfdsa\/.,mnbvcxz/a-la-ka-j/;
    $foo =~ y/~!@#\$%^&amp;*( )_+|-/abcdefghijklmn/;
    $foo =~ y/-1234567890=\\'/kabcdefghijlmn/;
    # $foo =~ y/\/.,mnbvc/;
    # $foo =~ y/';lkjhgf/;
    # $foo =~ y/\\][poiuy/;
    # $foo =~ y/|}{poiuytrewq":lkjhgfdsa?&gt;&lt;mnbvcxz/;
    # $foo =~ y/+_)(\*&amp;\^%\$#@!~/;
    @ary = unpack('C*',$foo);
    $ok = 0;
    for ($i = 0; $i &lt; $#ary; ++$i)
    {
        $diff = $ary[$i+1] - $ary[$i];
        $ok = 1 if $diff &gt; 1 || $diff &lt; -1;
    }
    if (!$ok)
    {
        print "\nPlease don't use consecutive keys.\n";
        return 0;
    }
    if ( $permcheck ne "yes" ) {  print "\tDoes not contain a keyboard sequence...good.\n";}
    select(undef,undef,undef,$delay);
    
    # Repeated patterns: ababab, abcabc, abcdabcd
    
    if ( $pass=~ /^(..)\1\1/
        || $pass=~ /^(...)\1/
        || $pass=~ /^(.)\1\1/
        || $pass =~ /^(....)\1/ )
    {
		if ( $EXPLICIT eq "YES" )
		{
        	print "\nPlease don't use repeated sequences of \"$1\".\n";
		}
		else
		{
        	print "\nPlease don't use repeated sequences.\n";
		}
        return  0;
    }
    if ( $permcheck ne "yes" ) {  print "\tDoes not contain a sequence of repeated characters...good.\n";}
    select(undef,undef,undef,$delay);
    
    # Reversed patterns: abccba abcddcba
    
    if  ( $pass =~ /^(.)(.)(.)\3\2\1/
        ||  $pass =~ /^(.)(.)(.)(.)\4\3\2\1/ )
    {
		if ( $EXPLICIT eq "YES" )
		{
    		print "\nPlease don't use palindromic sequences of \"$1$2$3$4\".\n";
		}else
		{
    		print "\nPlease don't use palindromic sequences.\n";
		}
        return 0;
    }
    if ( $permcheck ne "yes" ) {  print "\tDoes not contain a reversed sequence of repeated characters...good.\n";}
    select(undef,undef,undef,$delay);
    
    #   Some other login name?
    
    if ($isalogin{$pass})
    {
        print "\nPlease don't use somebody's login id.\n";
        return 0;
    }
    if ( $permcheck ne "yes" ) {  print "\tNot someone else's login name...good.\n";}
    select(undef,undef,undef,$delay);
    
    # A local host name?
    
    if (-f "/usr/hosts/$pass")
    {
        print "\nPlease don't use a local host name.\n";
        return 0;
    }
    if ( $permcheck ne "yes" ) {  print "\tNot the name of the local computer...good.\n";}
    select(undef,undef,undef,$delay);
    
    # Reversed login id?
    
    $reverse = reverse $me;
    if ($pass =~ /$reverse/i)
    {
        print "\nPlease don't use your login id spelled backwards.\n";
        return 0;
    }
    if ( $permcheck ne "yes" ) {  print "\tNot your login name reversed...good.\n";}
    select(undef,undef,undef,$delay);

    select(undef,undef,undef,$delay);
    1;
}
    
sub myexit
{
    system 'stty', 'echo';
    exit shift(@_);
}

sub permute
{
    # Needed: a function that accepts an array of words, substitutes the first
    # number in it for each possible match, and returns the resulting words.
    # Call this function recursively until all the returned words have no
    # numbers.  Pass the resulting array of words to &amp;goodenough();
    
    @AT=(); # keep -w happy
	@BANG=();
	@PLUS=();
    @1=('a','l','i');
    @2=('b','s');
    @3=('c','e');
    @4=('d','y');
    @5=('e','s');
    @6=('f','g');
    @7=('g','f');
    @8=('h','s');
    @9=('i','d','g');
    @0=('j','o','q','c','u');
    @AT=('a','o');
	@BANG=('i','l');
	@PLUS=('t');
    
	debug(3,"In &amp;permute with \"@_\"\n");
	@tumbled=@_;
    while ( grep(/[\d\@!\+]/,@tumbled) gt 0 )
    {
        # While there are non-letters in the @tumbled array...
        @tumbled2=&amp;tumble(@tumbled);
	debug(3, "\nIn permute(), \@tumbled2 is now \"@tumbled2\"\n");
		@tumbled=@tumbled2;
    }
		@tumbled=uniqarray(@tumbled);
	debug(3, "\nIn permute(), uniqued \@tumbled is now \"@tumbled\"\n");
}


sub tumble
{
    local($before)="";
    local($num)="";
    local($after)="";
    local(@newresult)="";
	local(@words)="";
	debug(3, "\&amp;tumble called with \"@_\"\n");
	local($words)="@_";
	$words=~s/^\s\s*//;
	$words=~s/\s*$//;
	debug(3, "In \&amp;tumble, \$words=\"$words\"\n");
	@words=split(/\s+/,$words);
    foreach $word ( @words )
    {
	debug(3, "Substituting in word: \"$word\"\n");
        if ( $word =~ /[0-9@\!\+]/ )
        {
            $_=$word;
            ($before,$num,$after)=/^([^0-9@\!\+]*)([0-9@\!\+])(.*)/;
            $num=~s/\@/AT/;
            $num=~s/!/BANG/;
            $num=~s/\+/PLUS/;

            foreach $sub ( @$num )
            {
                $newword=$before . $sub . $after;
                push(@newresult,$newword);
	debug(3, "In &amp;tumble, generated new word: \"$newword\"\n");
            }
        }
        else
        {
            if ( $word =~ /^\S+$/ )
            {
                push(@newresult,$word);
	debug(3, "In &amp;tumble, generated new word: \"$word\"\n");
            }
        }
    }
	debug(3, "In \&amp;tumble, returning \"@newresult\"\n");
	return(@newresult);
}   


sub uniqarray
{
	my @input=@_;
	my @uniqarray;
	my $item="";
	my $prev="";
	@input=sort(@input);
	foreach $item ( @input )
	{
        	if ( $item ne $prev )
        	{
                	push(@uniqarray,$item);
                	$prev=$item;
        	}
	}
	debug(3, "In uniqarray(), returning \"@uniqarray\"\n");
	return(@uniqarray);
}