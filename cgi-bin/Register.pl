#!/usr/bin/perl -wT
use strict;
#use CGI ':standard';
use CGI qw/ :standard -debug /;

my $membersFile = '../CSV/Members.csv';
my $REGISTERBUTTON = qq{
<br/>
<form action="../Registration.html">
	<input type = "submit" value = "Back to registration">
</form>
<form action="../index.html">
	<input type = "submit" value = "Back to home">
</form>
<br/>
};

my $name = param('name');
my $username = param('username');
my $password = param('password');
my $repassword = param('re-password');

#Check name validity
if( not ( $name =~ /[a-zA-Z]/ and $name =~ /^[a-zA-Z ]{1,100}$/ ) ){
	SHOW("Name must consist of at most 100 ASCII letters and spaces.<br/>
		\n$REGISTERBUTTON");
}
$name =~ s/^\s+|\s+$//g;	#remove leading and trailing whitespaces
$name =~ s/\s{2,}/ /g;		#replace multiple spaces with single ones

#Check username validity and availability
if ( not (length($username) <= 20 and $username =~ /^[a-zA-Z][a-zA-Z0-9]{2,}$/ ) ){
	SHOW("Username must consist of 3-20 ASCII letters and digits,
		and start with a letter.<br/>\n$REGISTERBUTTON");
} 
else{
	if ( REGISTERED( $username ) ){
		SHOW("Username already exists. Please try a different one.<br/>
			\n$REGISTERBUTTON");
	}
}

#Check password validity
if ( $password !~ /^[a-zA-Z0-9]{4,30}$/ ){
	SHOW("Password must consist of 4-30 ASCII letters and digits.<br/>
		\n$REGISTERBUTTON");
}

#Check correctedness of re-typed password
if( $password ne $repassword ){
	SHOW("Re-typed password did not match.<br/>\n$REGISTERBUTTON");
}
REGISTER();


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
	SHOW( "Registration complete. Welcome." );
}

sub SHOW{
	print "Content-type:text/html\n\n";
	print qq{

<head>
	<link rel="stylesheet" type="text/css" href="http://www.cs.mcgill.ca/~kye/style1.css"/>
	<title>Registration</title>
</head>

<body>
	<center>
        <table class="menu">
            <tr>
                <td><a href="http://www.cs.mcgill.ca/~kye/index.html">Home</a></td>
				<td><a href="http://www.cs.mcgill.ca/~kye/Catalogue.html">Catalogue</a></td>
                <td><a href="http://www.cs.mcgill.ca/~kye/Login.html" target="_blank">Login</a></td>
                <td><a href="http://www.cs.mcgill.ca/~kye/Registration.html">Register</a></td>
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
