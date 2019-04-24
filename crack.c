#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5.h"

const int PASS_LEN=20;        // Maximum any password will be
const int HASH_LEN=33;        // Length of MD5 hash strings
const int DYNAMIC_SIZE=100;

// Given a hash and a plaintext guess, return 1 if
// the hash of the guess matches the given hash.
// That is, return 1 if the guess is correct.
int tryguess(char *hash, char *guess)
{
    int isEqual=0;
    
    // Hash the guess using MD5
    char* g = md5(guess, strlen(guess));
    // Compare the two hashes
    isEqual = (strcmp(g, hash) == 0);

    free(g);
    
    return isEqual;
}

// Read in the dictionary file and return the array of strings
// and store the length of the array in size.
// This function is responsible for opening the dictionary file,
// reading from it, building the data structure, and closing the
// file.
char **read_dictionary(char *filename, int *size) //Ponter to a pointer
{
    // Open the file
    FILE *f = fopen(filename, "r");
    
    if(!f)
    {
        printf("Couldn't open the file for reading\n");
        exit(1);
    }
    
    int arrlen = DYNAMIC_SIZE;
    
    // Allocate space for a 100 char *
    char** lines = (char **)malloc(arrlen * sizeof(char *));
    
    // Chunk of memory that's unnecesarily big, in this case
    // the longest possible length of a string is 'PASS_LEN'
    char buf[PASS_LEN];
    
    int i = 0;
    while(fgets(buf, PASS_LEN, f) != NULL)
    {
        
        // Check if array is full        
        if( i == arrlen)
        {
            // If realloc can't find space in memory, it will
            // return  a NULL character.
            arrlen += DYNAMIC_SIZE;
            
            char ** newlines = realloc(lines, arrlen*sizeof(char*));
            if(!newlines)
            {
                fprintf(stderr, "Can't realloc\n");
                exit(1);
            }
            lines = newlines;
        }
         
        // Trim off the newline character from the array.
        buf[strlen(buf)- 1] = '\0';
        
        // Calculate the length of buf
        int slen = strlen(buf);
        
        // Use malloc to allocate memory for string
        char * str = (char *)malloc((slen + 1) * sizeof(char));
        
        // Copy string to str
        strcpy(str, buf); 
        
        // Attach str to data structure 
        lines[i] = str;
        i++;
    }
    
    *size = i;   // Set the length of the array of character pointers
    fclose(f);

    return lines;
}


int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        printf("Usage: %s hash_file dict_file\n", argv[0]);
        exit(1);
    }

    // Read the dictionary file into an array of strings.
    int dlen;
    char **dict = read_dictionary(argv[2], &dlen);

    // Open the hash file for reading.
    FILE * hashes = fopen(argv[1], "r");
    // For each hash, try every entry in the dictionary.
    char line[HASH_LEN];
    while(fgets(line, HASH_LEN, hashes) != NULL)
    {
        for(int i = 0; i < dlen; i++)
        {
            char * line2 = dict[i];
            if(tryguess(line, line2) == 1)
            {
                printf("%s\n", line2);
                break;
            }
        }
        
    }
    
    //free(dict[0);
    for(int i = 0; i < dlen; i++)
    {
        free(dict[i]);
    }
    free(dict);
    
    fclose(hashes);
}
