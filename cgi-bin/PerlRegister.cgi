#!/usr/bin/perl -wT
use strict;
#use CGI ':standard';
use CGI qw/ :standard -debug /;

print "Content-type:text/html\n\n";
print 'this',"<br/>";
my $name=param('name');
my $username=param('username');
my $password=param('password');
my $repassword=param('re-password');

if( $name =~ /^[a-zA-Z ]+$/ ){
	print "$name is all letters.<br/>";
}
else{
	print "$name is not all letters.<br/>";
}

print "Password is \'$password\'<br/>";

if ( $password =~ / / ){
	print "The password may not contain whitespaces.<br/>";
}
