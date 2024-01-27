#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"

int main(int argc, char*argv[]){
    if(argc < 2){
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movie_info.txt\n");
        return EXIT_FAILURE;
    }
    
	struct movie *list = processFile(argv[1]);
    int menuChoice = 0;

    do {
        menuChoice = displayMenu();
        menuSwitch(menuChoice, list);
        
    } while(menuChoice != 4);


    freeMovieList(list);
    return 0;
}
