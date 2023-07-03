// Randomly choose a champion for each player
// Champion are read from file and players requested on the fly
// Version: 07_03_2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_CHAMPIONS 200
#define MAX_NAME_LENGTH 100

int main() {
    // Open champion file
    char fileName[] = "champions.txt";
    FILE *championsFile;
    championsFile = fopen(fileName, "r");
    // Check if file was correctly opened
    if (!championsFile) {
        printf("Error opening file %s\n", fileName);
        printf("<press any key to quit>");
        getchar();
        return 1;
    }

    // Creating variables
    char line[MAX_NAME_LENGTH];
    char *championsList[MAX_NUM_CHAMPIONS];
    int count = 0;

    // Writes file to list
    while (fgets(line, sizeof(line), championsFile) != NULL) {
        // Remove newline character from each line
        line[strcspn(line, "\n")] = '\0';

        // Split line into first name and second name
        char *firstName = strtok(line, " ");
        char *secondName = strtok(NULL, " ");

        // Allocating memory for the name and copy it into the list
        char *championName;
        if (secondName != NULL) {
            championName = malloc(strlen(firstName) + strlen(secondName) + 2);
            sprintf(championName, "%s %s", firstName, secondName);
        } else {
            championName = malloc(strlen(firstName) + 1);
            sprintf(championName, "%s", firstName);
        }

        // Adding champion name to list
        championsList[count] = championName;
        ++count;

        // Check if list size was reached
        if (count >= MAX_NUM_CHAMPIONS) {
            printf("Maximum number of champions reached.\nChange MAX_NUM_CHAMPIONS variable");
            break;
        }
    }
    // Close file
    fclose(championsFile);
    
    return 1;
}