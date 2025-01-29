/*
 * Version: 1.2.4
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

// Global constants
#define MAX_NUM_CHARACTER 200
#define MAX_NAME_LENGTH 100

// Global variables
char* g_characters_list[MAX_NUM_CHARACTER];
char* g_player_list[10];
char g_input[10];
char* g_end_pointer;
int g_number_of_characters = 0;
long g_number_players;
long g_game_choice;

void read_file() {
	// Reset global variables
	for (int character = 0; character < g_number_of_characters; ++character) {
		free(g_characters_list[character]);
	}
	g_number_of_characters = 0;

	// Selecting file
	char* file_name;
	if (g_game_choice == 1)
		file_name = "champions.txt";
	else
		file_name = "agents.txt";

	// Open selected file
	FILE* characters_file = fopen(file_name, "r");
	// Check if file was correctly opened
	if (!characters_file) {
		printf("Error opening file %s\nFile can be downloaded from the github repository\n", file_name);
		printf("<press enter to quit>");
		getchar();
		exit(0);
	}

	// Writes file to list
	char line[MAX_NAME_LENGTH];
	while (fgets(line, sizeof(line), characters_file) != NULL) {
		// Remove newline character from each line
		line[strcspn(line, "\n")] = '\0';

		// Split line into first name and second name
		char* first_name = strtok(line, " ");
		char* second_name = strtok(NULL, " ");

		// Allocating memory for the name and copy it into the list
		char* character_name;
		if (second_name != NULL) {
			character_name = malloc(strlen(first_name) + strlen(second_name) + 2);
			sprintf(character_name, "%s %s", first_name, second_name);
		} else {
			character_name = malloc(strlen(first_name) + 1);
			sprintf(character_name, "%s", first_name);
		}

		// Adding character name to list
		g_characters_list[g_number_of_characters] = character_name;
		++g_number_of_characters;

		// Check if list size was reached
		if (g_number_of_characters >= MAX_NUM_CHARACTER) {
			printf("Maximum number of champions reached\n");
			break;
		}
	}
	// Close file
	fclose(characters_file);
}

void choose_game() {
	// Prompt user to select a game
	printf("Choose a game\n1. League of Legends\n2. Valorant\n>> ");
	fgets(g_input, sizeof(g_input), stdin);
	g_game_choice = strtol(g_input, &g_end_pointer, 10);
}

void split_teams() {
	// Get max number of players per team
	const long players_per_team = (g_number_players + 1) / 2;

	// Split teams between the first half and second half of the list
	printf("\n---Team 1---\n");
	for (int curr_player = 0; curr_player < players_per_team; ++curr_player) {
		printf("%s\n", g_player_list[curr_player]);
	}
	printf("\n---Team 2---\n");
	for (long curr_player = players_per_team; curr_player < g_number_players; ++curr_player) {
		printf("%s\n", g_player_list[curr_player]);
	}

	printf("\n<press enter to continue>\n");
	getchar();
}

void shuffle_teams() {
	// Set rand seed
	srand(clock());

	// Go through players
	for (long current_index = g_number_players - 1; current_index > 0; --current_index) {
		// Randomly pick a player index on list
		const long random_index = rand() % (current_index + 1);

		// Swap players
		char* tempPlayer = g_player_list[current_index];
		g_player_list[current_index] = g_player_list[random_index];
		g_player_list[random_index] = tempPlayer;
	}

	split_teams();
}

void get_num_players() {
	// Getting number of players
	while (1) {
		// Getting user input
		printf("Enter number of players (maximum 10) >> ");
		fgets(g_input, sizeof(g_input), stdin);
		g_number_players = strtol(g_input, &g_end_pointer, 10);

		// Check if input is valid
		if (errno == ERANGE || *g_end_pointer != '\n' || g_number_players < 1 || g_number_players > 10) {
			printf("Invalid input\n");
		} else
			break;
	}
}

void get_players_nicknames() {
	// Getting players nicknames
	for (int player = 0; player < g_number_players; ++player) {
		char player_name[MAX_NAME_LENGTH];
		printf("Enter player %d nickname >> ", player + 1);
		scanf("%s", player_name);
		getchar();

		// Allocating memory
		char* player_nickname = malloc(strlen(player_name) + 1);

		// Copying player nickname to list
		strcpy(player_nickname, player_name);
		g_player_list[player] = player_nickname;
	}

	// Prompt user to randomly separate teams or not
	printf("1. Randomly split teams\n0. Continue\n>> ");
	fgets(g_input, sizeof(g_input), stdin);
	long userInput = strtol(g_input, &g_end_pointer, 10);

	if (userInput == 1) {
		if (g_number_players % 2 == 0) {
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
	// Set rand seed
	srand(clock());

	// Randomly choosing a champion from list and assigning it to player
	printf("\n");
	for (int player = 0; player < g_number_players; ++player) {
		int selectedCharacter = rand() % g_number_of_characters;
		printf("%s >> %s\n", g_player_list[player], g_characters_list[selectedCharacter]);
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
		printf(
			"1. Re-roll\n"
			"2. Shuffle Teams\n"
			"3. Change number of players\n"
			"4. Change players nickname\n"
			"5. Change game\n"
			"0. Quit\n>> "
		);
		fgets(g_input, sizeof(g_input), stdin);
		long user_input = strtol(g_input, &g_end_pointer, 10);

		switch (user_input) {
		case 0:
			// Free allocated memory
			for (int character = 0; character < g_number_of_characters; ++character) {
				free(g_characters_list[character]);
			}
			for (int player = 0; player < g_number_players; ++player) {
				free(g_player_list[player]);
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
