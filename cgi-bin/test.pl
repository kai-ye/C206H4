#!/usr/bin/perl

for( $i=0; $i<5; $i++ ){
	print "\n$i\n";
}

this ( 'that', 'other' );

sub this{
	( $n1, $n2 ) = @_;
	print "\nSubroutine \'this\' with parameters $n1 and $n2\n";
}
