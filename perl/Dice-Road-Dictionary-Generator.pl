#!/usr/bin/perl #
# ----------------------------------------------------------------------
# Dice-Road-Dictionary-Generator.pl #
# ----------------------------------------------------------------------
# ----------------------------------------------------------------------
# The Dice-Road Dictionary(tm) Password Code Book : Your /dev/random home
# away from home.
# ----------------------------------------------------------------------
# The Dice-Road Dictionary(tm) Password Code Book  is inspired by Arnold
# Reinhold's Diceware(tm) Dictionary. Each Dice-Road(tm) Dictionary is
# generated from the GNU aspell Spell Checker Dictionary as well as
# the FIPS-181 compliant GNU apg program. Dice-Road(tm) Password
# Dictionaries are intended for use "on the road" when a reliable
# GNU-based system may not be available. It may also be useful for
# those who are required to work under untrustworthy systems or those
# where APG may not be available.
# 
# Each Dice-Road Dictionary(tm) Password Code Book entry is indexed
# off a dice value and offers the following:
# 
# * A Dictionary word from Aspell.
# * A Random word from /dev/urandom.
# * An APG-generated FIPS-181 word.
# 
# WARNING: A single dictionary word password equals NO Password. Diceware
# recommends 5 words for most users.  The Dice-Road dictionary offers the 
# ability to mix dictionary words with random and APG words.
# 
# Dice-Road Dictionaries can be generated for all languages supported
# by aspell-- Over 160 of them (see
# http://aspell.net/man-html/Supported.html).
# 
#
# ----------------------------------------------------------------------
# Copyright (c) 2011 by Tom Borawski [ChiefOperator (circle-A)
# GeneralTelegraph.com]
# ----------------------------------------------------------------------
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.  This program is distributed in
# the hope that it will be useful, but WITHOUT ANY WARRANTY; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.  You should have received a copy of the GNU General Public
# License along with this program.  If not, see
# <http://www.gnu.org/licenses/>.
# **********************************************************************
# pod documentation at the bottom.
use strict;
use IO::File; 
use Getopt::Std;

my %DiceRoadDict_options_description = ();
$DiceRoadDict_options_description{'l'} = "aspell language code";
$DiceRoadDict_options_description{'n'} = "number of dice for each directory entry";
$DiceRoadDict_options_description{'s'} = "dice geometry: number of sides for each dice";
$DiceRoadDict_options_description{'C'} = "maximum directory word character length";
$DiceRoadDict_options_description{'c'} = "minimum directory word character length";
$DiceRoadDict_options_description{'h'} = "Print this help.";
my %DiceRoadDict_options_defaults = ();
$DiceRoadDict_options_defaults{'l'} = "en";
$DiceRoadDict_options_defaults{'n'} = "5";
$DiceRoadDict_options_defaults{'s'} = "6";
$DiceRoadDict_options_defaults{'C'} = "9";
$DiceRoadDict_options_defaults{'c'} = "2";

my %DiceRoadDict_options_minimum = ();
$DiceRoadDict_options_minimum{'n'} = "3";
$DiceRoadDict_options_minimum{'s'} = "2";
$DiceRoadDict_options_minimum{'C'} = "4";
$DiceRoadDict_options_minimum{'c'} = "1";

my %DiceRoadDict_options_maximum = ();
$DiceRoadDict_options_maximum{'n'} = "20";
$DiceRoadDict_options_maximum{'s'} = "26";
$DiceRoadDict_options_maximum{'C'} = "18";
$DiceRoadDict_options_maximum{'c'} = "6";

my %DuplicateDetector = ();
my %aspell_language_code_description_hash = (
'aa' => 'Afar',
'af' => 'Afrikaans',
'ak' => 'Akan',
'am' => 'Amharic Ethiopic',
'ar' => 'Arabic Arabic',
'as' => 'Assamese Bengali',
'av' => 'Avar Cyrillic',
'ay' => 'Aymara',
'az' => 'Azerbaijani Cyrillic,',
'ba' => 'Bashkir Cyrillic',
'be' => 'Belarusian Cyrillic',
'bg' => 'Bulgarian Cyrillic',
'bh' => 'Bihari Devanagari',
'bm' => 'Bambara',
'bn' => 'Bengali Bengali',
'bo' => 'Tibetan Tibetan',
'br' => 'Breton',
'bs' => 'Bosnian',
'ca' => 'Catalan / Valencian',
'ce' => 'Chechen Cyrillic',
'co' => 'Corsican',
'cop' => 'Coptic Greek',
'cs' => 'Czech',
'csb' => 'Kashubian',
'cv' => 'Chuvash Cyrillic',
'cy' => 'Welsh',
'da' => 'Danish',
'de' => 'German',
'dyu' => 'Dyula',
'ee' => 'Ewe',
'el' => 'Greek Greek',
'en' => 'English',
'eo' => 'Esperanto',
'es' => 'Spanish',
'et' => 'Estonian',
'eu' => 'Basque',
'fa' => 'Persian Arabic',
'ff' => 'Fulah',
'fi' => 'Finnish',
'fj' => 'Fijian',
'fo' => 'Faroese',
'fr' => 'French',
'fur' => 'Friulian',
'fy' => 'Frisian',
'ga' => 'Irish',
'gd' => 'Scottish Gaelic',
'gl' => 'Gallegan',
'gn' => 'Guarani',
'gu' => 'Gujarati Gujarati',
'gv' => 'Manx Gaelic',
'ha' => 'Hausa',
'he' => 'Hebrew Hebrew',
'hi' => 'Hindi Devanagari',
'hil' => 'Hiligaynon',
'ho' => 'Hiri Motu',
'hr' => 'Croatian',
'hsb' => 'Upper Sorbian',
'ht' => 'Haitian Creole',
'hu' => 'Hungarian',
'hy' => 'Armenian Armenian',
'hz' => 'Herero',
'ia' => 'Interlingua (IALA)',
'id' => 'Indonesian Arabic,',
'ig' => 'Igbo',
'ii' => 'Sichuan Yi Yi',
'io' => 'Ido',
'is' => 'Icelandic',
'it' => 'Italian',
'jv' => 'Javanese Javanese,',
'ka' => 'Georgian Georgian',
'kg' => 'Kongo',
'ki' => 'Kikuyu / Gikuyu',
'kj' => 'Kwanyama',
'kk' => 'Kazakh Cyrillic',
'km' => 'Khmer Khmer',
'kn' => 'Kannada Kannada Planned',
'kr' => 'Kanuri',
'ks' => 'Kashmiri Arabic, Devanagari',
'ku' => 'Kurdish Arabic, Cyrillic,',
'kv' => 'Komi Cyrillic',
'ky' => 'Kirghiz Arabic, Cyrillic,',
'lb' => 'Luxembourgish',
'lg' => 'Ganda',
'li' => 'Limburgian',
'ln' => 'Lingala',
'lt' => 'Lithuanian',
'lu' => 'Luba-Katanga',
'lv' => 'Latvian',
'mg' => 'Malagasy',
'mi' => 'Maori',
'mk' => 'Macedonian Cyrillic',
'ml' => 'Malayalam , Malayalam',
'mn' => 'Mongolian Cyrillic, Mongolian',
'mo' => 'Moldavian Cyrillic',
'mos' => 'Mossi',
'mr' => 'Marathi Devanagari',
'ms' => 'Malay Arabic,',
'mt' => 'Maltese',
'my' => 'Burmese Myanmar',
'nb' => 'Norwegian Bokmal',
'nd' => 'North Ndebele',
'nds' => 'Low Saxon',
'ne' => 'Nepali Devanagari',
'ng' => 'Ndonga',
'nl' => 'Dutch',
'nn' => 'Norwegian Nynorsk',
'nr' => 'South Ndebele',
'nso' => 'Northern Sotho',
'nv' => 'Navajo',
'ny' => 'Nyanja',
'oc' => 'Occitan / Provencal',
'om' => 'Oromo Ethiopic,',
'or' => 'Oriya Oriya',
'os' => 'Ossetic Cyrillic',
'pa' => 'Punjabi Gurmukhi',
'pl' => 'Polish',
'ps' => 'Pushto Arabic',
'pt' => 'Portuguese',
'qu' => 'Quechua',
'rn' => 'Rundi',
'ro' => 'Romanian',
'ru' => 'Russian Cyrillic',
'rw' => 'Kinyarwanda',
'sc' => 'Sardinian',
'sd' => 'Sindhi Arabic',
'sg' => 'Sango',
'si' => 'Sinhalese Sinhala',
'sk' => 'Slovak',
'sl' => 'Slovenian',
'sm' => 'Samoan',
'sn' => 'Shona',
'so' => 'Somali',
'sq' => 'Albanian',
'sr' => 'Serbian Cyrillic,',
'ss' => 'Swati',
'st' => 'Southern Sotho',
'su' => 'Sundanese',
'sv' => 'Swedish',
'sw' => 'Swahili',
'ta' => 'Tamil Tamil',
'te' => 'Telugu Telugu',
'tet' => 'Tetum',
'tg' => 'Tajik Arabic, Cyrillic, Incomplete',
'ti' => 'Tigrinya Ethiopic',
'tk' => 'Turkmen Arabic, Cyrillic,',
'tl' => 'Tagalog , Tagalog',
'tn' => 'Tswana',
'to' => 'Tonga',
'tr' => 'Turkish Arabic,',
'ts' => 'Tsonga',
'tt' => 'Tatar Cyrillic',
'tw' => 'Twi',
'ty' => 'Tahitian',
'ug' => 'Uighur Arabic, Cyrillic,',
'uk' => 'Ukrainian Cyrillic',
'ur' => 'Urdu Arabic',
'uz' => 'Uzbek Cyrillic,',
've' => 'Venda',
'vi' => 'Vietnamese',
'wa' => 'Walloon',
'wo' => 'Wolof',
'xh' => 'Xhosa',
'yi' => 'Yiddish Hebrew',
'yo' => 'Yoruba',
'za' => 'Zhuang',
'zu' => 'Zulu'
);

my %ascii_to_groff_conversion_hash = (
'\\' => '\\[rs]',
']' => '\\[rB]',
'[' => '\\[lB]',
'{' => '\\[lC]',
'}' => '\\[rC]',
'\'' => '\\[aq]',
'"' => '\\[dq]',
'`' => '\\[oq]',
'´' => '\\[aa]',
'|' => '\\[ba]',
'`' => '\\[ga]',
'^' => '\\[ha]',
'~' => '\\[ti]',
'/' => '\\[sl]',
'-' => '\\[en]',
'#' => '\\[sh]',
'@' => '\\[at]',
'$' => '\\[Do]'

);



sub Ascii2Groff
{
  my $argGroffIn = shift @_;
  my $locRetStr = "";
  my @locInput = split(//,$argGroffIn);
  my $locTmp = "";

  foreach $locTmp (@locInput)
  {

    if(defined $ascii_to_groff_conversion_hash{$locTmp})
    {
      $locTmp = $ascii_to_groff_conversion_hash{$locTmp};
    }

#     $locTmp =~ s/[\\]/\\[rs]/g;
#     $locTmp =~ s/[\]]/\\[rB]/g;
#     $locTmp =~ s/[\[]/\\[lB]/g;
#     $locTmp =~ s/[\{]/\\[lC]/g;
#     $locTmp =~ s/[\}]/\\[rC]/g;
#     $locTmp =~ s/[\']/\\[aq]/g;
#     $locTmp =~ s/[\"]/\\[dq]/g;
#     $locTmp =~ s/[\`]/\\[oq]/g;
#     $locTmp =~ s/[\´]/\\[aa]/g;
#     $locTmp =~ s/[\|]/\\[ba]/g;
#     $locTmp =~ s/[\`]/\\[ga]/g;
#     $locTmp =~ s/[\^]/\\[ha]/g;
#     $locTmp =~ s/[\~]/\\[ti]/g;
#     #  $locTmp =~ s/[\=]/\[eq]/g;
#     $locTmp =~ s/[\/]/\\[sl]/g;
#     $locTmp =~ s/[\-]/\\[en]/g;
#     $locTmp =~ s/[\#]/\\[sh]/g;
#     $locTmp =~ s/[\@]/\\[at]/g;
#     $locTmp =~ s/[\$]/\\[Do]/g;
    $locRetStr = $locRetStr . $locTmp; 
  }
  return ($locRetStr);
}

sub DiceArray2String
{
  my $argDiceArray = shift @_;
  my $locCurrDiceVal = 1;
  my $locRetString = "";
  if (ref($argDiceArray) eq "ARRAY")
  { 
    foreach $locCurrDiceVal (@{$argDiceArray})
    {
      $locRetString = $locCurrDiceVal . "-" . $locRetString;
    }
  }
  else
  {
    die "Error: " . __LINE__ . "bad subroutine call.";
  }
  $locRetString =~ s/\-\Z//g; #get rid of trailing dash
  return $locRetString;
}

sub IncrementDiceArray
{
    my $argDiceArray = shift @_;
    my $argNumDiceSides = shift @_;
    my $locCurrDiceIX = 0;

    if (ref($argDiceArray) eq "ARRAY")
    { 

      $argDiceArray->[0]++;
      for ($locCurrDiceIX=0; $locCurrDiceIX < scalar(@{$argDiceArray});$locCurrDiceIX++)
      {
        if($argDiceArray->[$locCurrDiceIX] > $argNumDiceSides)
        {
          $argDiceArray->[$locCurrDiceIX] = 1;
          $argDiceArray->[($locCurrDiceIX + 1)]++;
        }
      }          

    }
}

sub print_DiceRoadDict_options_Usage
{
  my $desc_hash = shift @_;
  my $opt_hash = shift @_;
  my $tempkey = "";

  print "$0: Generates a Dice-Road Password Dictionary\n";
  if ( (ref($desc_hash) eq "HASH") &&  (ref($opt_hash) eq "HASH"))
  { 

    foreach $tempkey (sort keys %{$desc_hash})
    {
      print "Option -",$tempkey," : ",$desc_hash->{$tempkey};
      if (defined $opt_hash->{$tempkey})
      {
        print " [ default = ",$opt_hash->{$tempkey}," ]\n";
      }
      else
      {
        print "\n";
      }
    }
  }
  else
  {
    die "Program $0 Runtime Error Line: " . __LINE__; 
  }
} 

my $DiceRoadDict_options_string = "";
my $tmpstr;
foreach $tmpstr (sort keys %DiceRoadDict_options_description)
{
  if (defined $DiceRoadDict_options_defaults{$tmpstr})
  {
    $DiceRoadDict_options_string = $DiceRoadDict_options_string . $tmpstr . ":";
  }
  else
  {
    $DiceRoadDict_options_string = $DiceRoadDict_options_string . $tmpstr;

  }
}

#grab the command line options 
my %DiceRoadDict_options_hash = ();

if ( scalar(@ARGV) > 0 )
{
  Getopt::Std::getopts($DiceRoadDict_options_string,\%DiceRoadDict_options_hash);
}

#-h print help and exit 
if (defined $DiceRoadDict_options_hash{'h'})
{

  print_DiceRoadDict_options_Usage(\%DiceRoadDict_options_description,\%DiceRoadDict_options_defaults);
  exit(0);
}

#check all options against defined static minimums or maximums

foreach $tmpstr (sort keys %DiceRoadDict_options_hash)
{
  if (defined $DiceRoadDict_options_minimum{$tmpstr})
  {
    if (int($DiceRoadDict_options_hash{$tmpstr}) < int($DiceRoadDict_options_minimum{$tmpstr}))
    {
      print "Error: Option -",$tmpstr," value (",$DiceRoadDict_options_hash{$tmpstr},") is too low. Should be greater than ",$DiceRoadDict_options_minimum{$tmpstr},"\n";
      exit(-1);

    }
  }
  if (defined $DiceRoadDict_options_maximum{$tmpstr})
  {
    if (int($DiceRoadDict_options_hash{$tmpstr}) > int($DiceRoadDict_options_maximum{$tmpstr}))
    {
      print "Error: Option -",$tmpstr," value (",$DiceRoadDict_options_hash{$tmpstr},") is too low. Should be greater than ",$DiceRoadDict_options_maximum{$tmpstr},"\n";
      exit(-1);

    }
  }
}

#check to make sure the word minimum is > then the maximum

if ((defined $DiceRoadDict_options_hash{'C'}) && (defined $DiceRoadDict_options_hash{'c'}))
{
  if(int($DiceRoadDict_options_hash{'C'}) < int($DiceRoadDict_options_hash{'c'})) #our max is greater than our min.  bail out.
  {
      print "Error: Maximum word length selected is less than the minimum. (-c > -C) \n";
      exit(-1);
  }
}

#Should have workable options, now fill in the options not specified on the command line.

foreach $tmpstr (sort keys %DiceRoadDict_options_defaults)
{
  unless (defined $DiceRoadDict_options_hash{$tmpstr})
  {
    $DiceRoadDict_options_hash{$tmpstr} = $DiceRoadDict_options_defaults{$tmpstr};
  }
}


#copy the options and create the DiceRoadDict related globals
#Protect ourselves from crazy command line options
my $NumberOfDice = int($DiceRoadDict_options_hash{'n'});
my $DiceGeometry = int($DiceRoadDict_options_hash{'s'});
my $WordNumber = 0;
my $WordNumberMax = $DiceGeometry**$NumberOfDice;
my @MasterDiceArray = (1) x $NumberOfDice;
my $aspell_language_code = $DiceRoadDict_options_hash{'l'}; #Not checked...hope it is correct
my $aspell_language_string = " ";
#find our working language

if(defined $aspell_language_code_description_hash{$aspell_language_code})
{
  $aspell_language_string = $aspell_language_code_description_hash{$aspell_language_code};
}

my $aspell_dictionary_filename = "aspell-dictionary.txt";
my $aspell_dictionary_number_of_words = 0;
#this command dumps the aspell dictionary, removes suffixes such as
#possessives, filters words greater than or less then our specified
#bounds sorts and removes duplicates.
#
#Some of this filtration MAY NOT be desirable but was done to
#avoid groff formatting problems.
#
#Note that we use a random sort only to ensure a decent character distribution
#across the dictionary.
#
my $system_command_string = "aspell dump master " . $aspell_language_code . " | perl -pe \"s/[[:punct:]].+//g;\" | perl -ne \"/\\A.{" . int($DiceRoadDict_options_hash{'c'}) . "," . int($DiceRoadDict_options_hash{'C'}) . "}\\Z/ ? print : next;\" | sort -b  | perl -pe \"tr/A-Z/a-z/;\" | uniq | sort -R >" . $aspell_dictionary_filename;

my $syscall_result = system($system_command_string);

unless($syscall_result == 0)
{
  print "Error: Could not dump the ASPELL dictionary, Command was:\n";
  print $system_command_string,"\n";
  print "Result was: ",$syscall_result,"\n";
}
else
{
  $aspell_dictionary_number_of_words = `wc -l < $aspell_dictionary_filename`;
  die "wc failed: $?" if $?;
  chomp($aspell_dictionary_number_of_words);
}

print "Number of words in the Aspell Dictionary: ",$aspell_dictionary_number_of_words,"\n";

#we want our dictionary file to have more then enough words to do the job. 
if( $WordNumberMax > $aspell_dictionary_number_of_words)
{
    print "Error: The ASPELL dictionary has fewer words (",$aspell_dictionary_number_of_words,") then the number needed to print the directory (",$WordNumberMax,").\n";
    print "Reduce the number of dice, or expand the word length range to correct this error.\n";
    die;
}

#We know how many worda are delivered from aspell, now generate our random hexgrams

my $fh_random = new IO::File;
unless($fh_random->open("</dev/urandom") )
{
  die "Error: Could not open /dev/urandom !!!";
}
#my $RANDOM;
#open (RANDOM,"</dev/urandom");
my $random_raw;
$fh_random->read($random_raw,($aspell_dictionary_number_of_words*10) );
#read(RANDOM,$random_raw,($aspell_dictionary_number_of_words*10)); #make sure it exceeds the aspell dictionary  TGB 20101231 
#close(RANDOM);
$fh_random->close();

my @RawRandomDataArray = (unpack "c*", $random_raw);
my @RandomCodewordArray = ();
my $RandCode = "";
my $RandChar = "";

foreach $RandChar (@RawRandomDataArray)
{
  $RandChar = sprintf ("%c",$RandChar);
  if ($RandChar =~ /([[:graph:]])/)
  {
    $RandCode = $RandCode . $1;
    if ($RandCode =~ /[[:graph:]]{6,}/)
    {
      unless(defined $DuplicateDetector{$RandCode})
      {
      push @RandomCodewordArray,$RandCode;
      $DuplicateDetector{$RandCode} = 1;
      }
      $RandCode = "";
    }
  }
}
#End fill the random array

#Now generate the apg file
my $outapg_name = "apg-dictionary.txt";
#we use a weak random seed, but we are generating a dice table....
$system_command_string = "apg -a 0 -n " . int($aspell_dictionary_number_of_words * 1.25) . " -m 6 -x 9 -M sncl -c " . rand (9999999999999) . " >" . $outapg_name;
system($system_command_string);
my $APGword = "";
my $fh_apg_list = new IO::File;
unless($fh_apg_list->open("<$outapg_name") )
{
  die "Error: Could not open" . $outapg_name;
}

# we are now ready to start building the dictionary. 
my $fh_dictionary = new IO::File;
unless($fh_dictionary->open("<$aspell_dictionary_filename") )
{
  die "Error: Could not open" . $aspell_dictionary_filename;
}

my $output_name = "Dice-Road-Dictionary";
if (defined $DiceRoadDict_options_hash{'l'})
{
  $output_name = $output_name . "-" . $DiceRoadDict_options_hash{'l'} . "-lang";
}
if (defined $DiceRoadDict_options_hash{'n'})
{
  $output_name = $output_name . "-" . $DiceRoadDict_options_hash{'n'} . "-dice";
}
if (defined $DiceRoadDict_options_hash{'s'})
{
  $output_name = $output_name . "-" . $DiceRoadDict_options_hash{'s'} . "-sides";
}

my @outgroff_alternate_line_modifier = (".R\n",".B\n");
my $outgroff_alternate_line_ix = 0;


my $outgroff_name = $output_name . ".ms";
my $outtxt_name = $output_name . ".txt";

my $fh_outgroff = new IO::File;

system("cp Dice-Road-Dictionary-header.ms " . $outgroff_name);
unless($fh_outgroff->open(">>$outgroff_name") )
{
    &die_off($outgroff_name);
}

my $fh_outtxt = new IO::File;

unless($fh_outtxt->open(">$outtxt_name") )
{
    &die_off($outtxt_name);
}




#**********************************************************************
#Here is the word picker main loop
#Simple loop assumes that dictionary is already in random order
#**********************************************************************
my $AspellWord = "";



MAIN_WORD_LOOP: for ($WordNumber = 1; $WordNumber <= $WordNumberMax; $WordNumber++)
{
  unless($fh_dictionary->eof)
  {
    $RandCode = shift @RandomCodewordArray;

    $APGword = $fh_apg_list->getline;
    chomp $APGword;
    while ((defined $DuplicateDetector{$APGword}) && ($fh_apg_list->eof == 0) )
    {
      $APGword = $fh_apg_list->getline;
      chomp $APGword;
    }
    $DuplicateDetector{$APGword} = 1;

    $AspellWord = $fh_dictionary->getline;
    chomp $AspellWord;
    while ((defined $DuplicateDetector{$AspellWord}) && ($fh_dictionary->eof == 0) )
    {
      $AspellWord = $fh_dictionary->getline;
      chomp $AspellWord;
    }

    $DuplicateDetector{$AspellWord} = 1;
    #Now we are in our happy place: print to txt and groff files


    print $fh_outgroff $outgroff_alternate_line_modifier[$outgroff_alternate_line_ix];
    $outgroff_alternate_line_ix >= $#outgroff_alternate_line_modifier ? $outgroff_alternate_line_ix = 0 : $outgroff_alternate_line_ix++;

    printf($fh_outgroff "%-12s %-14s %-12s %-9s\n",DiceArray2String(\@MasterDiceArray),$AspellWord,Ascii2Groff($APGword),Ascii2Groff($RandCode));
    printf($fh_outtxt "%-20s %-20s %-15s %-15s\n",DiceArray2String(\@MasterDiceArray),$AspellWord,$APGword,$RandCode);
    print $fh_outgroff ".br\n";
    IncrementDiceArray(\@MasterDiceArray,$DiceGeometry);
  }
  else
  {
       last MAIN_WORD_LOOP;
  }
}

$fh_apg_list->close();
$fh_dictionary->close();
$fh_outgroff->close();
$fh_outtxt->close();


#******************************************************************************** 
# POD BLOCK
#**********************************************************************

=head1 NAME

Dice-Road-Dictionary-Generator.pl - Dice-Road DictionaryE<trade> Password Code Book Generator

=head1 SYNOPSIS

perl Dice-Road-Dictionary-Generator.pl -n 5 -l en (Generate a 5 dice
table in English with standard dice )

perl Dice-Road-Dictionary-Generator.pl -n 3 -s 20 -l en (Generate a 3
dice table in English with twenty-sided dice )

perl Dice-Road-Dictionary-Generator.pl -n 10 -s 2 -l en (Generate a 
10 dice table in English with coins as dice )

perl Dice-Road-Dictionary-Generator.pl -n 5 -l de (Generate a 5 dice
table in German with standard dice )

=head1 SLOGAN

The B<Dice-Road DictionaryE<trade> Password Code Book>: I<Your /dev/random home
away from home.>

=head1 DESCRIPTION

=head2 OVERVIEW

The Dice-Road DictionaryE<trade> Password Code Book is inspired by 
Arnold Reinhold's DicewareE<trade> Dictionary L<http://world.std.com/~reinhold/diceware.html>

The Dice-Road DictionaryE<trade> Password Code Book is a lookup table sorted in
dice-numeric order containing 4 columns:

=over 4

=item *

The Dice code: Dice-Road Dictionaries accommodate any number of dice
with any geometry (including the 2-sided dice called I<coins>).

=item *

An I<Aspell> Dictionary word: Dice-Road Dictionaries can be generated for
any of the 160 languages supported by GNU I<aspell> (see: aspell Supported Languages at
L<http://aspell.net/man-html/Supported.html>).

=item *

A GNU APG (automatic password generator) FIPS-181 compliant password.

=item *

A random ASCII (0x21-0x7e) hexagram generated from /dev/urandom.

=back

=head2 BACKGROUND


Generating secure passwords I<that will actually be used> requires
dealing with two conflicting requirements:

=over 2

=item *

The password should be easy to remember so that it will be used for
its intended purpose without inhibition.

=item *

The password should be totally random, made from the all available
characters that the keyboard can type.

=back

The GNU apg program (automatic password generator) does a good job
squaring the usability/randomness circle.  Unfortunately GNU apg does
not appear to be available for non-Linux operating systems.  Even if it
was, the source of randomness in non-open source systems may be
cracked or tapped.

While Dice-Road Dictionaries are intended for use "on the road" when a
reliable GNU-based system may not be available, there are strong
reasons to use physically-verifiable random number generators when
creating passwords for high stakes cryptographic payloads.

=head2 REQUIREMENTS

The Dice-Road DictionaryE<trade> perl program requires the following
GNU programs:

=over 4

=item *

perl

=item *

The I<aspell> Spell Checker

=item *

GNU apg: The Automated Password Generator

=item *

The Gnu Groff Typesetter I<(used to create the formatted pdf lists)>

=back

=head2 OTHER FEATURES

Each Dice-Road DictionaryE<trade> generated from the perl program will
be unique since the GNU I<aspell> Spell Checker Dictionary, the GNU
apg passwords and the /dev/urandom words are sorted in random
order. The program verifies that there are no duplicates within the
dictionary symbol set.

While the source of security for the Dice-Road DictionaryE<trade> is
dice randomness, random sorted dictionaries containing arbitrary word
sets will hinder the hypothetical "large, well funded organization" or
foreign government working to crack lawfully deployed passwords.

=head1 OPTIONS

Dice-Road-Dictionary-Generator.pl: Generates a Dice Road Password
Dictionary in groff and text format.

=over

=item Option -C

 : B<maximum> aspell dictionary word character length [ default = 7 ]

=item Option -c

 : B<minimum> aspell dictionary word character length [ default = 2 ]

=item Option -h

 : Print help.

=item Option -l

 : I<aspell> language code [ default = en ]

=item Option -n

 : number of dice for each dictionary entry [ default = 5 ]

=item Option -s

 : dice geometry: number of sides for each dice [ default = 6 ]

=back

=head1 CAVEATS

=head2 WHY DICE ?

Dice are not vulnerable to the the full spectrum of electronic
threats: interception, injection, intrusion. While physical attempts
to substitute loaded dice are possible, they are greatly reduced by
using clear casino dice).

There are many good password generation programs available: I<GNU apg>
for Linux systems or I<PWGEN> for windows, but many PC users are not
in a position to install these programs. Also, proper use of a
password generation program requires that the source code be examined
and verified.  The code that generates the random number stream should
also be examined to make sure it is not cracked or tapped.

Examining code is a task for experts.  It takes time and energy. Dice
are much easier to deal with and are intrinsically trustworthy.

There are also many password generation websites and random number
websites such as L<http://www.random.org> Likewise, some who need a
secure password may not have a secure Internet connection. In the age
when surveillance is pervasive, there may not be such a thing.


=head2 MINIMUM PASSWORD LENGTH

Arnold Reinhold recommends "five words for most users" for
  DicewareE<trade>.

L<http://world.std.com/~reinhold/dicewarefaq.html#howlong>

=head2 COLUMN PICKING

Each Dice Road Dictionary entry contains a Aspell dictionary word, an
APG FIPS-180 word and a random character hexagram. A rule for picking
the column (such as 1-2-3 for three throws) should be adhered to to
avoid selection bias creeping into the password selection.

=head2 KEYBOARD NOTES

All keyboards are vulnerable to both hardware and software key
logging.  And since keyboards are secret-software embedded systems,
there may be internal vulnerabilities as well.

The B<open source KeePass Password Safe> L<http://keepass.info> has an
optional built-in on-screen keyboard plugin obtainable at:
L<http://keepass.info/plugins.html#osk>

Open Solaris recommends transmitting passwords via a file (See
L<http://hub.opensolaris.org/bin/view/Community+Group+arc/passwords-cli>),
set to the minimum permissions to get the job done. Afterwards, it
should be destroyed using the GNU I<shred command>.
L<http://www.linfo.org/shred.html>


=head1 ACKNOWLEDGMENTS

Thanks to Arnold Reinhold-- I<one of the unsung heros of computer
security> for I<DicewareE<trade>
L<http://world.std.com/~reinhold/diceware.html>.

=head1 AUTHOR

Tom Borawski B<ChiefOperator (circle a) GeneralTelegraph.com>

=head1 COPYRIGHT

Copyright (c) 2011 by Tom Borawski I<ChiefOperator (circle a)
GeneralTelegraph.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.  This program is distributed in
the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more
details.  You should have received a copy of the GNU General Public
License along with this program.  If not, see
L<http://www.gnu.org/licenses/>.

=head1 AVAILABILITY

The project page is located at www.generaltelegraph.com.

L<http://www.generaltelegraph.com/dice-road-dictionary/dice-road-dictionary.html>

=cut

