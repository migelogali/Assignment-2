#include <stdio.h>
#include <stdlib.h> // for EXIT_SUCCESS and EXIT_FAILURE
#include <string.h>

// function declarations
struct movie* createMovie(char* title, int year, char* languages, double ratingValue);
void countMovies(char* filePath);
struct movie* processMovieFile(char* filePath);
void script();
void optionOne ();
void optionTwo ();
void optionThree ();
void freeMovieList(struct movie* list);
void freeMovie(struct movie* curr);



/* struct for movie information */
struct movie {
    char* title;
    int year;
    char* languages;
    double ratingValue;
    // pointer to the next element in the linked list
    struct movie* next;
};

/* Create a movie structure using dynamic memory allocation */
struct movie* createMovie(char* title, int year, char* languages, double ratingValue) {
    struct movie* currMovie = malloc(sizeof(struct movie));
    
    // Allocate memory for the title in the structure
    currMovie->title = calloc(strlen(title) + 1, sizeof(char));
    // Copy the value of title into the title in the structure
    strcpy(currMovie->title, title);

    currMovie->year = year;

    currMovie->languages = calloc(strlen(languages) + 1, sizeof(char));
    strcpy(currMovie->languages, languages);

    currMovie->ratingValue = ratingValue;

    // Set the next node to NULL in the newly created movie entry
    currMovie->next = NULL;
    return currMovie;
}

void countMovies(char* filePath) {
    int movie_count = -1;
    char *currLine = NULL;
    size_t len = 0;
    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");
    // Read the file line by line
    while(getline(&currLine, &len, movieFile) != -1) {
        movie_count += 1;
    }
    printf("Processed file %s and parsed data for %d movies\n", filePath, movie_count);
    printf("\n");

    // Free the memory allocated by getline for currLine
    free(currLine);
    // Close the file
    fclose(movieFile);
}

/*
* Function: processMovieFile
* Opens a file, reads and prints each line
* filePath: path to the file
*
* This function shows sample code that opens a file, then in a loop reads and
prints each line in that file.
* You can use this code however you want in your Prog Assignment 2.
*/
struct movie* processMovieFile(char* filePath) {
    char *currLine = NULL;
    size_t len = 0;
    // Open the specified file for reading only
    FILE *movieFile = fopen(filePath, "r");
    // skip header line
    getline(&currLine, &len, movieFile);

    struct movie* head = NULL;
    struct movie* tail = NULL;
    struct movie* newMovie;

    // Read the file line by line
    while(getline(&currLine, &len, movieFile) != -1) {
        // replace new line character with null terminato character for each line
        currLine[strcspn(currLine, "\n")] = '\0';

        char* saveptr;
        char* token = strtok_r(currLine, ",", &saveptr);
        char* title = token;

        token = strtok_r(NULL, ",", &saveptr);
        int year = atoi(token);

        token = strtok_r(NULL, ",", &saveptr);
        char* languages = token;

        token = strtok_r(NULL, ",", &saveptr);
        // converting string to a double
        double ratingValue = atof(token);

        // build in all characteristics of new movie and add it to linked list
        newMovie = createMovie(title, year, languages, ratingValue);

        if (head == NULL) {
            //for first element of linked list
            head = newMovie;
            tail = newMovie;
        }
        else {
            tail->next = newMovie;
            tail = newMovie;
        }
    }

    free(currLine);
    fclose(movieFile);
    return head;
}   

void script() {
    printf("1. Show movies released in the specified year\n");
    printf("2. Show highest rated movie for each year\n");
    printf("3. Show the title and year of release of all movies in a specific language\n");
    printf("4. Exit from the program\n");
    printf("\n");
}

void optionOne (struct movie* head) {
    int inpYear;
    int numMovies = 0;
    printf("Enter the year for which you want to see movies: ");
    scanf("%d", &inpYear);

    struct movie* curr = head;
    while (curr != NULL) {
        if (curr->year == inpYear) {
            printf("%s\n", curr->title);
            numMovies += 1;
        }
        curr = curr->next;
    }
    if (numMovies == 0) {
        printf("No data about movies released in the year %d\n", inpYear);
    }
    printf("\n");
}

void optionTwo (struct movie* head) {
    int years[122];
    // initialize all values in years array to 0
    memset(years, 0, sizeof(years));

    int startYear = 1900;
    double maxRating;
    struct movie* currFirstLoop = head;
    int year = currFirstLoop->year;
    while (currFirstLoop != NULL) {
        year = currFirstLoop->year;
        // only print if there is data for that year
        if (years[year - startYear] == 0) {
            maxRating = currFirstLoop->ratingValue;
            // so that it doesn't need to revisit conditional again
            years[year - startYear] += 1;

            struct movie* bestMovie = currFirstLoop;
            // don't need to start from beginning and waste time
            struct movie* currSecondLoop = currFirstLoop->next;
            while (currSecondLoop != NULL) {
                if (currSecondLoop->year == year) {
                    if (currSecondLoop->ratingValue > maxRating) {
                        bestMovie = currSecondLoop;
                        maxRating = currSecondLoop->ratingValue;
                    }
                }
                currSecondLoop = currSecondLoop->next;
            }
            printf("%d %.1f %s\n", bestMovie->year, bestMovie->ratingValue, bestMovie->title);
        }
        currFirstLoop = currFirstLoop->next;
    }
    printf("\n");
}

void optionThree (struct movie* head) {
    int count = 0;
    struct movie* curr = head;
    char entLanguage[20];
    char languagesCopy[100];

    printf("Enter the language for which you want to see movies: ");
    scanf("%s", entLanguage);
    while (curr != NULL) {
        // start at second character, cut off end bracket by reducing length by 2
        strncpy(languagesCopy, curr->languages + 1, strlen(curr->languages) - 2);
        // add null terminator as a character to end of string
        languagesCopy[strlen(curr->languages) - 2] = '\0';

        char* token = strtok(languagesCopy, ";");
        while (token != NULL) {
            if(strcmp(token, entLanguage) == 0) {
                printf("%d %s\n", curr->year, curr->title);
                count += 1;
                // save the extra time
                break;
            }
            // otherwise, keep checking rest of string
            token = strtok(NULL, ";");
        }
        curr = curr->next;
    }
    if (count == 0) {
        printf("No data about movies released in %s\n", entLanguage);
    }
    printf("\n");
}

/*
* Free the memory allocated for elements in the list
*/
void freeMovieList(struct movie* list) {
    struct movie* curr;
    while(list != NULL){
        curr = list;
        list = list->next;
        freeMovie(curr);
    }
}

/**
* Free the memory for one struct
*/
void freeMovie(struct movie* curr) {
    free(curr->title);
    free(curr->languages);
    free(curr);
}

int main ( int argc, char **argv ) {
    int choice;
    struct movie* head = NULL;

    if (argc < 2) {
        printf("You must provide the name of the file to process\n");
        printf("Example usage: ./movies movies.csv\n");
        return EXIT_FAILURE;
    }

    countMovies(argv[1]);

    head = processMovieFile(argv[1]);

    while (1) {
        script();

        printf("Enter a choice from 1 to 4: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                optionOne(head);
                break;
            case 2:
                optionTwo(head);
                break;
            case 3:
                optionThree(head);
                break;
            case 4:
                freeMovieList(head);
                return EXIT_SUCCESS;
            default:
                printf("You entered an incorrect choice. Try again.\n");
                printf("\n");
        }
    }

    // Free the memory dynamically allocated for the movies
    freeMovieList(head);
    return EXIT_SUCCESS;
}