/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2024, Rupam Mittal, Rishab Singh
 * All rights reserved.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "questions.h"
#include "players.h"
#include "jeopardy.h"

#define BUFFER_LEN 256  // Buffer length for input
#define NUM_PLAYERS 4   // Number of players in the game

#define WHAT_IS "what is"  // Keyword for valid responses
#define WHO_IS "who is"    // Keyword for valid responses

// ANSI color codes for terminal text formatting
#define COLOR_RESET "\033[0m"
#define COLOR_CYAN "\033[36m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_GREEN "\033[32m"
#define COLOR_RED "\033[31m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_BLUE "\033[34m"
#define COLOR_BOLD "\033[1m"
#define COLOR_UNDERLINE "\033[4m"

// Function to trim leading spaces
char* ltrim(char *s) {
    while (isspace((unsigned char)*s)) {
        s++;
    }
    return s;
}

// Function to trim trailing spaces
char* rtrim(char *s) {
    if (!*s) return s;

    char* back = s + strlen(s) - 1;
    while (back >= s && isspace((unsigned char)*back))
        back--;
    *(back + 1) = '\0';
    return s;
}

// Function to trim both leading and trailing spaces
char* trim(char *s) {
    return rtrim(ltrim(s));  
}

// Function to tokenize user input
void tokenize(char *input, char *tokens[], int maxTokens) {
    int tokenCount = 0;
    char *trimmedInput = trim(input);
    char *token = strtok(trimmedInput, " ");

    while (token != NULL && tokenCount < maxTokens) {
        tokens[tokenCount++] = token;
        token = strtok(NULL, " ");
    }

    tokens[tokenCount] = NULL;
}

// Function to compare players' scores for sorting
int compare_players(const void *a, const void *b) {
    const player *playerA = (const player *)a;
    const player *playerB = (const player *)b;

    if (playerA->score > playerB->score) return -1;
    else if (playerA->score < playerB->score) return 1;
    else return 0;
}

// Function to display final game results
void show_results(player *players, int num_players) {
    printf("%s\n", COLOR_BOLD COLOR_MAGENTA "\n================ FINAL RESULTS ================\n" COLOR_RESET);
    
    qsort(players, num_players, sizeof(player), compare_players);
    for (int i = 0; i < num_players; i++) {
        if (i == 0) {
            printf("%i. %s(Winner)%s %s%s%s - Score: %s$%i%s\n", i + 1, COLOR_GREEN, COLOR_RESET, COLOR_BOLD, players[i].name, COLOR_RESET, COLOR_GREEN, players[i].score, COLOR_RESET);
        } else {
            printf("%i. %s%s%s - Score: $%i\n", i + 1, COLOR_BOLD, players[i].name, COLOR_RESET, players[i].score);
        }
    }
    printf("%s==============================================\n" COLOR_RESET, COLOR_BOLD);
}

// Function to display welcome message
void display_welcome() {
    printf("%s%s\n", COLOR_BOLD COLOR_CYAN, "\n================== WELCOME TO JEOPARDY ==================\n");
    printf("%sTest your knowledge in Threads, Processes, and OS Structure!%s\n", COLOR_MAGENTA, COLOR_RESET);
    printf("%sType 'help' for available commands.%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s========================================================\n\n" COLOR_RESET, COLOR_BOLD);
}

// Function to display the question board
void display_board() {
    printf("%s\n", COLOR_BOLD COLOR_BLUE "==================== QUESTION BOARD ====================\n" COLOR_RESET);
    display_categories();
    printf("%s========================================================\n" COLOR_RESET);
}

// Function to display the prompt for user input
void display_prompt() {
    printf("%s\n", COLOR_BOLD COLOR_YELLOW "Enter a command (type 'help' for options):" COLOR_RESET);
    printf("> ");
}

int main() {
    bool allAnswered = false;  // Flag to check if all questions are answered
    player players[NUM_PLAYERS];  // Array to store players
    char buffer[BUFFER_LEN];  // Buffer to store user input
    char *tokens[BUFFER_LEN];  // Array to store tokenized input

    display_welcome();  // Display welcome message

    // Get player names
    for (int i = 0; i < NUM_PLAYERS; i++) {
        printf("%sEnter name for player %d:%s ", COLOR_CYAN, i + 1, COLOR_RESET);
        fgets(buffer, BUFFER_LEN, stdin);
        strtok(buffer, "\n");  // Remove newline character
        strcpy(players[i].name, buffer);
        players[i].score = 0;
    }

    initialize_game();  // Initialize game questions
    print_players(players, NUM_PLAYERS);  // Display player scores

    display_board();  // Display the question board

    // Display available commands
    printf("%s\n", COLOR_YELLOW "Available commands:\n* display\n* exit\n* pick [category] [value] [user]\n  e.g. pick Processes 100 User1" COLOR_RESET);

    // Main game loop
    while (fgets(buffer, BUFFER_LEN, stdin) != NULL) {
        tokenize(buffer, tokens, BUFFER_LEN);
        if (tokens[0] == NULL) continue;  // Skip if no input

        if (strcmp(tokens[0], "exit") == 0) {
            break;  // Exit the game
        } else if (strcmp(tokens[0], "help") == 0) {
            // Display help message
            printf("%s\n", COLOR_YELLOW "Available commands:\n* display\n* exit\n* pick [category] [value] [user]\n  e.g. pick Processes 100 User1" COLOR_RESET);
        } else if (strcmp(tokens[0], "display") == 0) {
            // Display question board and player scores
            display_board();
            print_players(players, NUM_PLAYERS);
        } else if (strcmp(tokens[0], "pick") == 0 && tokens[1] && tokens[2] && tokens[3]) {
            // Handle picking a question
            char *category = tokens[1];
            char *user = tokens[3];

            int playerIndex = -1;
            for (int i = 0; i < NUM_PLAYERS; i++) {
                if (strcasecmp(players[i].name, user) == 0) {  // Case-insensitive comparison
                    playerIndex = i;
                    break;
                }
            }

            if (playerIndex == -1) {
                printf("%sInvalid player \"%s\". Please try again.%s\n", COLOR_RED, user, COLOR_RESET);
                continue;
            }

            tokens[2] = strtok(tokens[2], "$\n");
            int value = atoi(tokens[2]);

            if (value == 0 || (value != 100 && value != 200 && value != 300 && value != 400)) {
                printf("%sInvalid question value \"%s\". Please enter 100, 200, 300, or 400.%s\n", COLOR_RED, tokens[2], COLOR_RESET);
                continue;
            }

            if (!already_answered(category, value)) {
                char answer[BUFFER_LEN] = { 0 };
                char* trimmedAnswer;

                display_question(category, value);
                printf("%sEnter your answer:%s ", COLOR_BLUE, COLOR_RESET);

                while (fgets(answer, BUFFER_LEN, stdin) != NULL) {
                    stringToLower(answer);
                    trimmedAnswer = trim(answer);
                    if (trimmedAnswer[0] == '\0') continue;

                    if (strncmp(trimmedAnswer, WHAT_IS, strlen(WHAT_IS)) == 0) {
                        trimmedAnswer += strlen(WHAT_IS);
                        break;
                    } else if (strncmp(trimmedAnswer, WHO_IS, strlen(WHO_IS)) == 0) {
                        trimmedAnswer += strlen(WHO_IS);
                        break;
                    } else {
                        printf("%sThe correct form for your response would be 'What is...' or 'Who is...'? Please try again.%s\n", COLOR_RED, COLOR_RESET);
                        printf("%sEnter your answer:%s ", COLOR_BLUE, COLOR_RESET);
                    }
                }

                trimmedAnswer = trim(trimmedAnswer);
                ValidationResult ans = valid_answer(category, value, trimmedAnswer);

                if (ans.valid) {
                    printf("%sCorrect answer! %s%s%s earned %d points.%s\n", COLOR_GREEN, COLOR_CYAN, user, COLOR_RESET, value, COLOR_RESET);
                    players[playerIndex].score += value;
                } else {
                    printf("%sIncorrect answer! The correct answer is: %s%s%s\n", COLOR_RED, COLOR_YELLOW, questions[ans.question].answer, COLOR_RESET);
                }

                questions[ans.question].answered = true;

                allAnswered = true;
                for (int i = 0; i < NUM_QUESTIONS; i++) {
                    if (!questions[i].answered) {
                        allAnswered = false;
                    }
                }

                if (allAnswered) break;
            } else {
                printf("%sQuestion already answered.%s\n", COLOR_RED, COLOR_RESET);
            }
        } else {
            printf("%sInvalid command or incorrect format. Please use 'pick [category] [value] [user]'.%s\n", COLOR_RED, COLOR_RESET);
        }
        display_prompt();
    }

    if (allAnswered) show_results(players, NUM_PLAYERS);
    return EXIT_SUCCESS;
}
