my $string = "I am not a window washer.";
if($string =~ ?what pattern would I search for?) {
  # Oops has invalid character
} else {
  # String is perfect only alphanumeric
}



if ($string =~ m/[^a-zA-Z0-9]/){
    print "The string contains non-alphanumeric characters";
}



if ($value =~ m/[^a-zA-Z0-9]/) {}
// it's true if the string contains (=~) something other (^) than letters and numbers (a-zA-Z0-9)



print "bad string: $string\n"
  if $string !~ /^[a-zA-Z0-9]+$/;
  
print "bad string: $string\n"
  unless $string =~ /^[a-zA-Z0-9]+$/;