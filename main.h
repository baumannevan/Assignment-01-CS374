#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/* struct for movie information */
struct movie
{
    char *title;
    int *year;
    char *languages;
    double *rating;
    struct movie *next;
};

// POTENTAL MEMORY ERROR HERE!!! 
struct movie *createMovie(char *currLine){
    struct movie *currMovie = malloc(sizeof(struct movie));

    // For use with strtok_r
    char *saveptr;

    // The first token is the title
    char *token = strtok_r(currLine, ",", &saveptr);
    currMovie->title = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->title, token);

    // The next token is the year
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->year = calloc(strlen(token) + 1, sizeof(int));
    // strcpy(currMovie->year, token);
    *(currMovie->year) = atof(token);


    // The next token is the languages
    token = strtok_r(NULL, ",", &saveptr);
    currMovie->languages = calloc(strlen(token) + 1, sizeof(char));
    strcpy(currMovie->languages, token);

    // The last token is the rating
    token = strtok_r(NULL, "\n", &saveptr);
    currMovie->rating = calloc(strlen(token) + 1, sizeof(double));
    // strcpy(currMovie->rating, token);
    *(currMovie->rating) = strtod(token, NULL);


    // Set the next node to NULL in the newly created Movie entry
    currMovie->next = NULL;

    return currMovie;
}

struct movie *processFile(char *filePath){
    FILE *movieFile = fopen(filePath, "r");

    if (movieFile == NULL){
    	printf("could not open file");
    }

    char* currLine = NULL;
    size_t len = 0;
    size_t nread; 
    // char *token; //compiler says unsed?
    int numMovies = 0;

    // head of the linked list
    struct movie *head = NULL;
    // tail of the linked list
    struct movie *tail = NULL;

    if ((nread = getline(&currLine, &len, movieFile)) != -1) {
        free(currLine);  // Discard the first line
        currLine = NULL;
        len = 0;
    }

    while ((nread = getline(&currLine, &len, movieFile)) != -1){
        // create newNode from the current line
        struct movie *newNode = createMovie(currLine);
        numMovies += 1;

        // check to see if head
        if (head == NULL){
            // fist node. set head and tail to curr node
            head = newNode;
            tail = newNode;
        } else {
            // not first node. add curr node to next;
            tail->next = newNode;
            tail = newNode;
     	 }
	free(currLine);
	currLine = NULL;
	len = 0;
    }
    printf("Processed file %s and parsed data for %d movies.\n", filePath, numMovies);

    free(currLine);
    fclose(movieFile);
    
    return head;
}

// function to clear the memory allocated to the movies struct
void freeMovieList(struct movie* head)
{
    // node to allow free of nodes without losing them
   struct movie* tmp;

   while (head != NULL)
    {
        // freeing all the data!
       tmp = head;
       head = head->next;
       free(tmp->title);
       free(tmp->year);
       free(tmp->languages);
       free(tmp->rating);
       
       free(tmp);
   }
    free(head); // free final original node

}

// function to display and get menu choice from user
int displayMenu(){
    int choice = 0;

    do {
        printf("\n1. Show movies released in the specified year\n");	
        printf("2. Show highest rated movie for each year\n");
        printf("3. Show the title and year of release of all movies in a specific language\n");
        printf("4. Exit from the program\n\n");
        printf("Enter a choice from 1 to 4: ");
        
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
            // Clear the input buffer in case of invalid input
            while (getchar() != '\n');
            printf("You entered an incorrect choice. Try Again.\n\n");
        }
    
    } while (choice < 1 || choice > 4); // componet in input validation
    
    return choice;
}

// function to display movies relased in a target year  
void moviesYear(struct movie *list){
    int year = 0; // target year
    bool match = false; // bool to allow for no success message
    
    //input validation for target year
    do {
        printf("Enter the year for which you want to see movies: ");
        // user input for target year, with input validation
        if (scanf("%d", &year) != 1 || year < 1900 || year > 2021) {
            // Clear the input buffer in case of invalid input
            while (getchar() != '\n');
            printf("You entered an incorrect year. Try Again.\n\n");
        }

    } while (year < 1900 || year > 2021);

    // move through movie linked list
    while (list != NULL)
    {
        // if movie year is equal to target year
        if (*list->year == year){
            match = true;
            printf("==>%s\n", list->title);
        }
        list = list->next;
    }
    // if no match display no match to user
    if(!match){
        printf("No data about movies released in the year %d\n", year);
    }

    return;
}

// struct existing only for movies rated function. holds only nessasary information.
struct year_rating{
    char *title;
    int year;
    double highest_rating;
};

// function to display highest rated movie per unique year
void moviesRated(struct movie *head)
{
    // Assuming the years are positive integers
    const int max_year = 2022; // from assigment requirments assume 2022 is max year.

    // Initialize an array to store the highest rating and title for each year
    struct year_rating *highest_ratings = (struct year_rating *)malloc(max_year * sizeof(struct year_rating));
    if (highest_ratings == NULL)
    {
        // error hadeling for knowing if something breaks
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the array
    for (int i = 0; i < max_year; ++i)
    {
        highest_ratings[i].title = NULL;
        highest_ratings[i].year = 0;
        highest_ratings[i].highest_rating = 0.0;
    }

    // Traverse the linked list
    struct movie *current = head;

    while (current != NULL)
    {
        // Ensure that year, rating, and title pointers are not NULL
        if (current->year != NULL && current->rating != NULL && current->title != NULL)
        {
            int year = *(current->year);
            double rating = *(current->rating);

            // Update the highest rating and title for the corresponding year
            if (rating > highest_ratings[year].highest_rating)
            {
                // Free previously allocated title
                free(highest_ratings[year].title);

                // Allocate memory for the new title and copy the current movie's title
                highest_ratings[year].title = strdup(current->title);

                highest_ratings[year].year = year;
                highest_ratings[year].highest_rating = rating;
            }
        }

        current = current->next;
    }

    // Print results
    for (int year = 0; year < max_year; ++year)
    {
        if (highest_ratings[year].highest_rating > 0)
        {
            printf("%d %.1f %s\n", highest_ratings[year].year, highest_ratings[year].highest_rating, highest_ratings[year].title);
        }

        // Free allocated title
        free(highest_ratings[year].title);
    }

    // Free allocated memory
    free(highest_ratings);
}

// Languages function to search for movies realesed in a specific language
void moviesLanguages(struct movie *list){
    struct movie* curr = list;
    // int seving as a bool for if no languages have been found
    int found = 0;

    // gets users taget language from user
    char *targetLang[20];
    printf("Enter the Language: ");
    scanf("%s", *targetLang);

    // iterate through movies liked list
    while (curr != NULL) {
        // make a copy of the languages string for tokenization
        char languagesCopy[100];
        strcpy(languagesCopy, curr->languages);

        // check to see if targetLang is within a languages string
        char *token = strtok(languagesCopy, ";[]");
        
        while (token != NULL) {
            // Check for exact match using strcmp
            if (strcmp(token, *targetLang) == 0) {
                printf("%d %s\n", *curr->year, curr->title);
                found = 1;
                break;  // No need to continue checking tokens for this movie
            }

            token = strtok(NULL, ";[]");
        }

        // move to the next movie
        curr = curr->next;
    }
    // if match is not found inform user
    if (!found){
            printf("No data about movies released in %s\n", *targetLang);
    }
}

// Menu switch, taking user input and routing to appriate function.
void menuSwitch(int choice, struct movie *list){
    switch (choice){
        case 1:
            moviesYear(list);
            break;
        case 2:
            moviesRated(list);
            break;
        case 3:
            moviesLanguages(list);
            break;
        case 4:
            freeMovieList(list);
            exit(0);

            break;
        default: 
            printf("Invalid choice\n");
    }
}

#endif