#!/usr/bin/perl -wT
use strict;
#use CGI ':standard';
use CGI qw/ :standard -debug /;

my membersFile = '../Users.txt';

my $name = param('name');
my $username = param('username');
my $password = param('password');
my $repassword = param('re-password');

my $MSG;

#Check name validity
if( not (length($name) <= 100 and $name =~ /[a-zA-Z]/ and $name =~ /^[a-zA-Z ]+$/ ) ){
	$MSG=$MSG."Name must consist entirely of at most 100 ASCII letters and spaces.<br/>";
}
else{
	$name =~ s/^\s+|\s+$//g;
	$name =~ s/\s{2,}/ /g;
	$MSG=$MSG."$name is a valid name.<br/>";
}

#Check username validity and availability
if ( not (length($username) <= 20 and $username =~ /^[a-zA-Z][a-zA-Z0-9]{3,}$/ ) ){
	$MSG=$MSG."Username must consist entirely of 4-20 ASCII letters and digits, and start with a letter.<br/>";
} 
else{
	if ( REGISTERED( $username ) ){
		$MSG=$MSG."Username already exists. Please try a different one.<br/>";
	}
	else{
		REGISTER( $username );
	}
}


#Check password validity
if ( length($password) < 4 or length($password) > 100 or $password =~ /\s/ ){
	$MSG=$MSG."The password must be 4-30 ASCII characters, with no whitespaces.<br/>";
}
else{
	$MSG=$MSG."$password is a valid password.<br/>";
}

#Check correctedness of re-typed password
if( $password ne $repassword ){
	$MSG=$MSG."Re-typed password did not match.<br/>";
}
else{
	$MSG=$MSG."Re-typed password matched.<br/>";
}

open( USERS, ">>$membersFile");
print USERS "$name,$username,$password\n";
close(USERS);

open(USERS, "<$membersFile");
$MSG=$MSG."<br/>The file so far:<br/>";
while( my $line = <USERS> ){
	$MSG=$MSG."\'$line\'<br/>";
}
close(USERS);


sub REGISTERED{
	my $line;
	while ( $line = <USERS> ){
		if()
	}
	return 0;
}

sub REGISTER{
	
}

SHOW( $MSG );

sub SHOW{
	print "Content-type:text/html\n\n";
	print qq{
		<br/><br/><br/><br/><br/><br/><br/><br/><br/>
		<center>
			$_[0]
		</center>
		};
	exit;
}
