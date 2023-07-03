// Randomly choose a champion for each player
// Champion are read from file and players requested on the fly
// Version: 07_03_2023

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Global constants
#define MAX_NUM_CHAMPIONS 200
#define MAX_NAME_LENGTH 100

// Global variables
char *charactersList[MAX_NUM_CHAMPIONS];
int numberOfCharacters = 0;
char *playerList[10];
long numberPlayers;
char input[10];
char *endPointer;

void read_file() {
    char *fileName;
    fileName = "champions.txt";

    // Open selected file
    FILE *charactersFile;
    charactersFile = fopen(fileName, "r");
    // Check if file was correctly opened
    if (!charactersFile) {
        printf("Error opening file %s\nFile can be downloaded from the github repository\n", fileName);
        printf("<press enter to quit>");
        getchar();
        return;
    }

    // Writes file to list
    char line[MAX_NAME_LENGTH];
    while (fgets(line, sizeof(line), charactersFile) != NULL) {
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
        charactersList[numberOfCharacters] = championName;
        ++numberOfCharacters;

        // Check if list size was reached
        if (numberOfCharacters >= MAX_NUM_CHAMPIONS) {
            printf("Maximum number of champions reached\n");
            break;
        }
    }
    // Close file
    fclose(charactersFile);
}

void get_num_players() {
    // Getting number of players
    while (1) {
        // Getting user input
        printf("Enter number of players (maximum 10) >> ");
        fgets(input, sizeof(input), stdin);
        numberPlayers = strtol(input, &endPointer, 10);

        // Check if input is valid
        if (errno == ERANGE || *endPointer != '\n' || numberPlayers < 1 || numberPlayers > 10) {
            printf("Invalid input\n");
        } else
            break;
    }
}

void get_players_nickname() {
    // Creating variables
    char playerName[MAX_NAME_LENGTH];

    // Getting players nicknames
    for (int player = 0; player < numberPlayers; ++player) {
        printf("Enter player %d nickname >> ", player + 1);
        scanf("%s", playerName);
        getchar();

        // Allocating memory
        char *playerNickname = malloc(strlen(playerName) + 1);

        // Copying player nickname to list
        strcpy(playerNickname, playerName);
        playerList[player] = playerNickname;
    }
}

void choosing_characters() {
    // Randomly choosing a champion from list and assigning it to player
    long userChoice;
    srand(clock());
    while (1) {
        for (int player = 0; player < numberPlayers; ++player) {
            int selectedChampion = rand() % numberOfCharacters;
            printf("%s >> %s\n", playerList[player], charactersList[selectedChampion]);
        }

        // Prompt user to re-roll or quit program
        printf("1. Re-roll\n0. Quit\n>> ");
        fgets(input, sizeof(input), stdin);
        userChoice = strtol(input, &endPointer, 10);
        if (userChoice == 0)
            break;
    }
}

int main() {
    read_file();
    get_num_players();
    get_players_nickname();
    choosing_characters();

    // Free allocated memory
    for (int champion = 0; champion < numberOfCharacters; ++champion) {
        free(charactersList[champion]);
    }
    for (int player = 0; player < numberPlayers; ++player) {
        free(playerList[player]);
    }

    return 1;
}
