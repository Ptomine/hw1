#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#define STR_LEN 1024

int isLater( int character )
{
	if( ((character >= 65 && character <= 90) || 
		(character >= 97 && character <= 122)) ) {
		return 1;
	} 
	return 0;
}

int checkWord( const char* word )
{
	int key = 0;
	while( *word ) {
		if( !isLater( *word ) ) {
			key=1;
			break;
		}
		word++;
	}
	return key;
}

int wordCount( const char * word, const int wordLenght, const char * const path )
{
	FILE* tmp = fopen( path, "r" );
	if( !tmp ) {
		printf("%s - wrong file direction!\n", path);
	 	return -1;
	} 	
	int offset = 0;
	int checkSeek = 0;
	int result = 0;
	while( !feof( tmp ) ) {
		checkSeek = fseek( tmp, offset, SEEK_SET );
		if( !checkSeek ) {
			char tmpStr[STR_LEN];
			char* checkStr = fgets( tmpStr, STR_LEN, tmp );
			if( checkStr ) {
				while( *checkStr ) {
					if( (*checkStr != *word) && ( *checkStr != (*word - 32) ) && ( (*checkStr - 32) != *word ) ) {
						checkStr++;
					} else {
						if( !strncmp( word + 1, checkStr + 1, wordLenght - 1 ) ) {
							if( !isLater(*(checkStr+wordLenght)) ) {
								if( !(checkStr == tmpStr) ) {
									if( !isLater(*(checkStr - 1))) {
										result++;
										checkStr += wordLenght;
									}
								} else {
									result++;
									checkStr += wordLenght;
								}	
							} 
						} 
						checkStr++;
					}
				}
				offset += strlen( tmpStr );
			} else {
				break;
			}
		} else {
			printf("File %s if  corupted!\n", path);
			break;
		}			
	}
	return result; 
}

float dispersionC( const int meanings[], const int count)
{
	int summ = 0;
	float result = 0;
	for( int i = 0; i < count; i++ ) {
		summ += meanings[i];
	}
	float mean = summ / count;
	float sqrs[count];
	for( int i = 0; i < count; i++ ) {
		sqrs[i] = pow( (meanings[i] - mean), 2.F );
		result += sqrs[i];
	}
	result = sqrt( result / (count - 1) );
	return result;
}

int main(const int argc, const char * const argv[])
{
	if (argc < 4) {
		printf("Please, input the word and path to the documents (at least 2 documents)!\n");
		return -1;
	}
	if( checkWord( argv[1] ) ) {
		printf("The word content some symbols of punctuation!\n");
		return -1;
	}	
	int* counter = (int*)malloc( sizeof( int ) );
	int findedWords = 0;
	int wordLen = strlen( argv[1] );
	int successFiles = 0;
	for( int i = 2; i < argc; i++ ) {
		findedWords = wordCount( argv[1], wordLen, argv[i] ); 
		if( findedWords >= 0 ) {
			counter[successFiles] = findedWords;
			printf( "%d words was be founded in %s\n", counter[successFiles], argv[i] );
			successFiles++;
		} else {
			printf("ERROR. File can\'t be read: %s\n", argv[i]);
		}
	}
	if( successFiles < 2 ) {
		printf("Need more correct arguments to solving.\n");
		return -1;
	}
	printf("Dispersion = %f\n", dispersionC (counter, successFiles));
	return 0;
}
