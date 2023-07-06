// Version: 1.2

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
long gameChoice;

void read_file() {
    // Reset global variables
    numberOfCharacters = 0;
    for (int character = 0; character < numberOfCharacters; ++character) {
        free(charactersList[character]);
    }

    // Selecting file
    char *fileName;
    if (gameChoice == 1)
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
    // Prompt user to select a game
    printf("Choose a game\n1. League of Legends\n2. Valorant\n>> ");
    fgets(input, sizeof(input), stdin);
    gameChoice = strtol(input, &endPointer, 10);
}

void split_teams() {
    // Get max number of players per team
    int playersPerTeam = (numberPlayers + 1) / 2;

    // Split teams between the first half and second half of the list
    printf("\n---Team 1---\n");
    for (int currPlayer = 0; currPlayer < playersPerTeam; ++currPlayer) {
        printf("%s\n", playerList[currPlayer]);
    }
    printf("\n---Team 2---\n");
    for (int currPlayer = playersPerTeam; currPlayer < numberPlayers; ++currPlayer) {
        printf("%s\n", playerList[currPlayer]);
    }

    printf("\n<press enter to continue>\n");
    getchar();
}

void shuffle_teams() {
    // Set srand seed
    srand(clock());
    
    // Go through players
    for (int currentIndex = numberPlayers - 1; currentIndex > 0; --currentIndex) {
        // Randomly pick a player index on list
        int randomIndex = rand() % (currentIndex + 1);

        // Swap players
        char *tempPlayer = playerList[currentIndex];
        playerList[currentIndex] = playerList[randomIndex];
        playerList[randomIndex] = tempPlayer;
    }

    split_teams();
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

    // Prompt user to randomly separate teams or not
    long userInput;
    printf("1. Randomly split teams\n0. Continue\n>> ");
    fgets(input, sizeof(input), stdin);
    userInput = strtol(input, &endPointer, 10);
    
    if (userInput == 1) {
        if (numberPlayers % 2 == 0) {
            shuffle_teams();
        } else {
            printf("Teams will not be even\n");
            printf("\n<press enter to continue>\n");
            getchar();
            shuffle_teams();
        }
    }
}

void choosing_characters() {
    // Set srand seed
    srand(clock());

    // Randomly choosing a champion from list and assigning it to player
    printf("\n");
    for (int player = 0; player < numberPlayers; ++player) {
        int selectedCharacter = rand() % numberOfCharacters;
        printf("%s >> %s\n", playerList[player], charactersList[selectedCharacter]);
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
        long userInput;
        printf("1. Re-roll\n2. Shuffle Teams\n3. Change number of players\n4. Change players nickname\n"
               "5. Change game\n0. Quit\n>> ");
        fgets(input, sizeof(input), stdin);
        userInput = strtol(input, &endPointer, 10);
        switch (userInput) {
            case 0:
                // Free allocated memory
                for (int character = 0; character < numberOfCharacters; ++character) {
                    free(charactersList[character]);
                }
                for (int player = 0; player < numberPlayers; ++player) {
                    free(playerList[player]);
                }
                return 1;
            case 1:
                choosing_characters();
                break;
            case 2:
                shuffle_teams();
                break;
            case 3:
                get_num_players();
                get_players_nicknames();
                break;
            case 4:
                get_players_nicknames();
                break;
            case 5:
                choose_game();
                read_file();
                break;
            default:
                printf("Invalid input\n");
                break;
        }
    }
}