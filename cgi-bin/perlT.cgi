#!/usr/bin/perl -wT
use strict;
use CGI ':standard';

print "Content-Type: text/html\n\n";

print "<html>";
print "<head><title>Perl generated</title></head>";
print "<body bgcolor=\"#CC00CC\">";

for( my $i=0; $i<5; $i++ ){
	print "\n\n$i\n\n";
}

this ( 'that', 'other' );

sub this{
	my ( $n1, $n2 ) = @_;
	print "\nSubroutine \'this\' with parameters $n1 and $n2\n";
}


foreach my $name (param()){
	my $val = param($name);
	print "<br/>\n$name has value $val\n<br/>";
}

if (param('checkbo1')){
	print "\nCheckbox present.\n<br/>";
}

if(param('password')){
	print "\nPassword present.\n<br/>";
}

my @array=('checkbo1','password');
#my ($a1,$a2)=(param('checkbo1'),param('password'));
my ($a1) = param('checkbo1'); my $a2='dum';
print "<br/>\n$a1,$a2\n<br/>";

print "</body>";

print "</html>";
