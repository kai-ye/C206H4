#include <stdio.h>
#include <stdlib.h>

#define MAXKEYLEN 30
#define MAXVALUELEN 40

typedef struct map{//struct for linked pairs of key and value
	char *key, *value;
	struct map *next;
}MAP;

void putsDecoded( char*, int*, char);
void getMAP( MAP*, int*);
void getInput( MAP*, int);
void checkMalloc( void*, char*);



void printInput( MAP* param ){
	while( param != NULL ){
		printf("Key: \'%s\'; Value: \'%s\'\n <br/>", param->key, param->value);
		param=param->next;
	}
}



int main(int argc, char* argv[]){

	MAP *paramFirst;//pointer to first pair of (input name, input value)
	int len = atoi( getenv("CONTENT_LENGTH") );
	
	printf("Content-type:text/html\n\n");

	if( len == 0 ){
		printf("No input was received.");
		return EXIT_FAILURE;
	}

	checkMalloc( paramFirst = (MAP*) malloc( sizeof(MAP) ), "input");
	
	getInput( paramFirst, len);

	printInput( paramFirst );

	return 0;
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
	checkMalloc( param->key = malloc(MAXKEYLEN), "input name");
	putsDecoded( param->key, lenPtr, '=' );
	checkMalloc( param->value = malloc(MAXVALUELEN), "input value");
	putsDecoded( param->value, lenPtr, '&' );
}

//Decodes cgi input up to *lenPtr characters or the delim character and stores in s
void putsDecoded( char* s, int* lenPtr, char delim ){
	int i=0, c;
	while( *lenPtr > 0 && ( c = getchar() ) != delim ){
		if( c == '%' ){//decode escaped hexadecimal ASCII character
			*lenPtr = *lenPtr - 3;
			scanf("%2x", &c);
		}
		else{
			--*lenPtr;
			if ( c == '+' )
				c = ' ';
		}
		s[i] = c;
		i++;
	}
	s[i] = '\0';
	if ( c == delim ) --*lenPtr;
}

//Checks whether ptr successfully malloced. If not, display message and exit failure.
void checkMalloc( void* ptr, char* name){
	if( ptr == NULL ){
		printf("No memory to store %s.",name);
		exit(EXIT_FAILURE);
	}
}
