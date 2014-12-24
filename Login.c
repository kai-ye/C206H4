#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ERRORLOG "Errors.log"
#define MAXKEYLEN 20
#define MAXVALUELEN 40
#define MAXNAMELEN 100
#define MEMBERSCSV "../CSV/Members.csv"
#define MEMBERSCSVCOLS 200
#define LOGGEDINCSV "../CSV/LoggedIn.csv"
#define LOGGEDINCSVCOLS 20
#define CATALOGUEHTML "../Catalogue.html"
#define CATALOGUEHTMLCOLS 300
#define GRABPOINT "<!--C hidden input grab-->"
#define GREETGRABPOINT "<!--C greet grab-->"

typedef struct map{//struct for linked pairs of key and value
	char *key, *value;
	struct map *next;
}MAP;

void putsDecoded( char*, int*, const char, const size_t);
void getMAP( MAP*, int*);
void getInput( MAP*, int);
char* getValue( const MAP*, const char*);
char* matchingName( const char*, const char* );
int loggedIn ( const char* );
void logInAndShowCatalogue( const char*, const char*);
void checkMalloc( const void*, const char*);
void exitWith( const char*, const int);
void report( const char* );



void printAllInput( MAP* param ){
	printf( "Content-type:text/html\n\n");
	while( param != NULL ){
		printf("Key: \'%s\'; Value: \'%s\'\n <br/>", param->key, param->value);
		param=param->next;
	}
}



int main(int argc, char* argv[]){
	char *name, *username, *password, line[MEMBERSCSVCOLS+3];
	MAP *paramFirst;//pointer to first pair of input (name,value) of linked MAP's
	int len = atoi( getenv("CONTENT_LENGTH") );
	if( len == 0 ){
		printf("No input was received.");
		return EXIT_FAILURE;
	}

	checkMalloc( paramFirst = (MAP*) malloc( sizeof(MAP) ), "input");
	
	getInput( paramFirst, len);

	if( ( username = getValue( paramFirst, "username" ) ) == NULL )
		exitWith( "Username input not received.", EXIT_FAILURE );
	if( ( password = getValue( paramFirst, "password" ) ) == NULL )
		exitWith( "Password input not received.", EXIT_FAILURE );

	if( ( name = matchingName( username, password ) ) == NULL )
		exitWith( "No match for the username and password entered.", EXIT_FAILURE);
	/*if( loggedIn( username ) )//whether already logged in
		exitWith( "You are already logged in from somewhere else.", EXIT_FAILURE);*/
	logInAndShowCatalogue( username , name);

	return 0;
}


//Searches linked MAP's starting at param for MAP with specified key, and returns its value.
//Returns NULL if not found.
char* getValue( const MAP* param, const char* key ){
	while( param != NULL && strcmp( param->key, key ) != 0 ){
		param = param->next;
	}
	if( param != NULL ) return param->value;
	return NULL;
}

//Reads up to len characters of default cgi input into linked MAP's starting at paramCurr
void getInput( MAP* paramCurr, int len){
	getMAP( paramCurr, &len );//get first pair of (input name, input value)
	while( len > 0 ){
		checkMalloc( paramCurr->next = (MAP*) malloc( sizeof(MAP) ), "input");
		paramCurr = paramCurr->next;
		getMAP( paramCurr, &len);
	}
}

//Decodes cgi input up to *lenPtr characters or until '&', and stores into param
void getMAP( MAP* param, int *lenPtr ){
	checkMalloc( param->key = malloc(MAXKEYLEN + 1), "input name");
	putsDecoded( param->key, lenPtr, '=', MAXKEYLEN );
	checkMalloc( param->value = malloc(MAXVALUELEN + 1), "input value");
	putsDecoded( param->value, lenPtr, '&', MAXVALUELEN );
}

//Decodes cgi input up to *lenPtr characters or the delim character and stores in s
//Returns empty string if input to be decoded has length > maxLen, but reads and discards input anyway
void putsDecoded( char* s, int* lenPtr, const char delim, const size_t maxLen ){
	int i=0, c;
	while( *lenPtr > 0 && ( c = getchar() ) != delim ){
		if( i < maxLen ) {
			if( c == '%' ){//decode escaped hexadecimal ASCII character
				*lenPtr = *lenPtr - 3;//Deduct from lengthof remaining input
				scanf("%2x", &c);
			}
			else{
				--*lenPtr;//Deduct from length of remaining input
				if ( c == '+' )//replace + with space
					c = ' ';
			}
			s[i] = c;
			i++;
		}
		else {//Input exceeds length
			if( i ) i = 0;
			--*lenPtr;//Deduct from length of remaining input
		}
	}
	s[i] = '\0';
	if ( c == delim ) --*lenPtr;
}

//Returns pointer to full name if a match is found for username and password, NUll otherwise
char* matchingName( const char* username, const char* password ){
	FILE *IN = fopen( MEMBERSCSV, "rt" );
	char line[MEMBERSCSVCOLS+3], *name, *token1, *token2, delim[]=",\n";
	fgets( line, MEMBERSCSVCOLS+3, IN );
	while( !feof( IN ) ){
		name = strtok ( line, delim);
		token1 = strtok( NULL, delim);
		token2 = strtok( NULL, delim);
		if( token1 != NULL && token2 != NULL
		    && strcmp( username, token1) == 0 && strcmp( password, token2) == 0 ){
			fclose( IN );
			return strdup( name );
		}
		fgets( line, MEMBERSCSVCOLS+3, IN );
	}
	fclose( IN );
	return NULL;
}

//Returns 1 if user already logged-in, 0 otherwise
int loggedIn( const char* username){
	FILE* IN = fopen( LOGGEDINCSV , "rt" );
	char line[LOGGEDINCSVCOLS+3];
	size_t length;
	fgets( line, LOGGEDINCSVCOLS+3, IN);
	while( !feof( IN ) ){
		length = strlen( line );
		if( line[length-1] == '\n')
			line[length-1] = '\0';//chomp newline
		if( strcmp( line, username) == 0 ){
			fclose( IN );
			return 1;
		}
		fgets( line, LOGGEDINCSVCOLS+3, IN);
	}
	fclose( IN );
	return 0;
}

//Appends username to loggedIn csv, and generates Catalogue from template, with hidden
// input username replacing the first line found to contain the GRABPOINT string
void logInAndShowCatalogue( const char* username, const char* name){
	FILE *OUT, *IN;
	char line[CATALOGUEHTMLCOLS+3];

	if( !loggedIn( username ) ) {//if not already logged in
		if( ( OUT = fopen( LOGGEDINCSV, "at") ) == NULL )
			exitWith( "Did someone just delete the loggedIn csv?", EXIT_FAILURE);
		fprintf( OUT, "%s\n", username );//appends username to loggedIn csv
		fclose( OUT );
	}
	
	//Generate Catalogue from static Catalogue html
	if( ( IN = fopen( CATALOGUEHTML, "rt") ) == NULL )
		exitWith( "Catalogue html could not be opened.", EXIT_FAILURE);
	
	printf("Content-type:text/html\n\n");
	fgets( line, CATALOGUEHTMLCOLS+3, IN );
	while( !feof( IN ) && strstr( line, GRABPOINT) == NULL ){//look for GRABPOINT
		fputs( line, stdout );
		if( strstr( line, GREETGRABPOINT) )//if GREETGRABPOINT found
			printf("Welcome, %s", name);//output greeting with name here
		fgets( line, CATALOGUEHTMLCOLS+3, IN );
	}

	if( feof( IN ) ){	//If GRABPOINT not found
		report("Login.c: grab point not found in Catalogue.html");
		puts("<p style=\"color:#CC0000\">Error: grab point not found in Catalogue.html</p>");
	}
	else{
		printf("<input type=\"hidden\" name=\"username\" value=\"%s\">", username);
		fgets( line, CATALOGUEHTMLCOLS+3, IN );//skip the line with GRABPOINT
		while( !feof( IN ) ){
			fputs( line, stdout);
			fgets( line, CATALOGUEHTMLCOLS+3, IN );
		}
	}
	fclose( IN );
}

//Checks whether ptr successfully malloced. If not, display message and exit failure.
void checkMalloc( const void* ptr, const char* nameOfString){
	if( ptr == NULL ){
		puts("Content-type:text/html\n\n");
		printf("No memory to store %s.", nameOfString);
		exit(EXIT_FAILURE);
	}
}

//Displays html with message, and exits with exitValue
void exitWith( const char* Message, const int exitValue){
	puts("Content-type:text/html\n\n");
	printf(
"<html>"
"<head>"
"	<link rel=\"stylesheet\" type=\"text/css\" href=\"http://www.cs.mcgill.ca/~kye/style1.css\"/>"
"	<title>Login failure</title>"
"</head>"
"<body>"
"<center>"
"<br/><br/><br/><br/><br/><br/>"
"	%s<br/>"
"Return to: "
"	<table class=\"menu\">"
"		<tr>"
"			<td><a href=\"http://www.cs.mcgill.ca/~kye/index.html\">Home</a></td>"
"            <td><a href=\"http://www.cs.mcgill.ca/~kye/Login.html\">Login</a></td>"
"		</tr>"
"	</table>"
"</center>"
"</body>"
"</html>",
		Message);
	exit( exitValue );
}

//Appends message to error log
void report( const char* Message){
	FILE *OUT;
	if( ( OUT = fopen( ERRORLOG, "at" ) ) != NULL ){
		fprintf( OUT, "%s\n", Message);
		fclose( OUT );
	}
}
