// Randomly choose a champion for each player
// Champion are read from file and players requested on the fly
// Version: 1.1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Global constants
#define MAX_NUM_CHARACTER 200
#define MAX_NAME_LENGTH 100

// Global variables
char *charactersList[MAX_NUM_CHARACTER];
int numberOfCharacters = 0;
char *playerList[10];
long numberPlayers;
char input[10];
char *endPointer;
long userChoice;

void read_file() {
    // reset global variables
    numberOfCharacters = 0;
    for (int character = 0; character < numberOfCharacters; ++character) {
        free(charactersList[character]);
    }

    // selecting file
    char *fileName;
    if (userChoice == 1)
        fileName = "champions.txt";
    else
        fileName = "agents.txt";

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
        char *characterName;
        if (secondName != NULL) {
            characterName = malloc(strlen(firstName) + strlen(secondName) + 2);
            sprintf(characterName, "%s %s", firstName, secondName);
        } else {
            characterName = malloc(strlen(firstName) + 1);
            sprintf(characterName, "%s", firstName);
        }

        // Adding character name to list
        charactersList[numberOfCharacters] = characterName;
        ++numberOfCharacters;

        // Check if list size was reached
        if (numberOfCharacters >= MAX_NUM_CHARACTER) {
            printf("Maximum number of champions reached\n");
            break;
        }
    }
    // Close file
    fclose(charactersFile);
}

void choose_game() {
    printf("Choose a game\n1. League of Legends\n2. Valorant\n>> ");
    fgets(input, sizeof(input), stdin);
    userChoice = strtol(input, &endPointer, 10);
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

void get_players_nicknames() {
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
    srand(clock());
    printf("\n");
    for (int player = 0; player < numberPlayers; ++player) {
        int selectedChampion = rand() % numberOfCharacters;
        printf("%s >> %s\n", playerList[player], charactersList[selectedChampion]);
    }

    printf("\n<press enter to continue>\n");
    getchar();
}

int main() {
    // Prompt user to choose a game
    choose_game();

    // Read file based on user choice
    read_file();

    // Prompt user to enter players information
    get_num_players();
    get_players_nicknames();

    // Randomly choosing character
    choosing_characters();

    // Main loop
    while (1) {
        // Interactive menu
        printf("1. Re-roll\n2. Change number of players\n3. Change players nickname\n4. Change game\n0. Quit\n>> ");
        fgets(input, sizeof(input), stdin);
        userChoice = strtol(input, &endPointer, 10);
        if (userChoice == 0) {
            break;
        } else if (userChoice == 1) {
            choosing_characters();
        } else if (userChoice == 2) {
            get_num_players();
            get_players_nicknames();
        } else if (userChoice == 3) {
            get_players_nicknames();
        } else if (userChoice == 4) {
            choose_game();
            read_file();
        } else {
            printf("Invalid input\n");
        }
    }

    // Free allocated memory
    for (int character = 0; character < numberOfCharacters; ++character) {
        free(charactersList[character]);
    }
    for (int player = 0; player < numberPlayers; ++player) {
        free(playerList[player]);
    }

    return 1;
}
