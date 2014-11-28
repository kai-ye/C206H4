#!/usr/bin/perl -wT
use strict;
#use CGI ':standard';
use CGI qw/ :standard -debug /;

my $membersFile = '../Users.txt';
my $REGISTERBUTTON = qq{
<br/>
<form action="../Registration.html">
	<input type = "submit" value = "Back to registration">
</form>
<br/>
};

my $name = param('name');
my $username = param('username');
my $password = param('password');
my $repassword = param('re-password');

my $MSG;

#Check name validity
if( not (length($name) <= 100 and $name =~ /[a-zA-Z]/ and $name =~ /^[a-zA-Z ]+$/ ) ){
	SHOW("Name must consist entirely of at most 100 ASCII letters and spaces.<br/>\n$REGISTERBUTTON");
}
else{
	$name =~ s/^\s+|\s+$//g;
	$name =~ s/\s{2,}/ /g;
	$MSG=$MSG."$name is a valid name.<br/>";
}

#Check username validity and availability
if ( not (length($username) <= 20 and $username =~ /^[a-zA-Z][a-zA-Z0-9]{3,}$/ ) ){
	SHOW("Username must consist entirely of 4-20 ASCII letters and digits, and start with a letter.<br/>\n$REGISTERBUTTON");
} 
else{
	if ( REGISTERED( $username ) ){
		SHOW("Username already exists. Please try a different one.<br/>\n$REGISTERBUTTON");
	}
}

#Check password validity
if ( length($password) < 4 or length($password) > 100 or $password =~ /\s/ ){
	SHOW("The password must be 4-30 ASCII characters, with no whitespaces.<br/>\n$REGISTERBUTTON");
}
else{
	$MSG=$MSG."$password is a valid password.<br/>";
}

#Check correctedness of re-typed password
if( $password ne $repassword ){
	SHOW("Re-typed password did not match.<br/>\n$REGISTERBUTTON");
}
else{
	$MSG=$MSG."Re-typed password matched.<br/>";
	REGISTER();
}


open(USERS, "<$membersFile");
$MSG=$MSG."<br/>Added user $username. The users file so far:<br/>";
while( my $line = <USERS> ){
	$MSG=$MSG."\'$line\'<br/>";
}
close(USERS);

SHOW( $MSG );


sub REGISTERED{#returns 1 if user with username $_[0] is registered, 0 if not
	open( USERS, "<$membersFile" );
	my ( $line, $name, $username );
	while ( $line = <USERS> ){
		( $name, $username ) = split( ',' , $line );
		if( $username eq $_[0] ){
			close( USERS );
			return 1;
		}
	}
	close( USERS );
	return 0;
}

sub REGISTER{#adds user to Members.csv
	open( USERS, ">>$membersFile");
	print USERS "$name,$username,$password\n";
	close(USERS);
}

sub SHOW{
	print "Content-type:text/html\n\n";
	print qq{

<head>
	<title>Registration</title>
</head>

<body>
	<center>
	<table>
	<tr>
		<td><a href="">Home</a>
	</tr>
	</table>
	</center>
	
	<br/><br/><br/><br/><br/><br/><br/><br/><br/>
		<center>
			$_[0]
		</center>
</body>

		};
	exit;
}