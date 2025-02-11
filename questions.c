/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2024, Rupam Mittal, Rishab Singh
 * All rights reserved.
 *
 * Part of the Jeopardy game simulation for Tutorial 4 of the Operating Systems course.
 * This file manages questions, including their initialization, display,
 * and the validation of player answers.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "questions.h" // Include the definitions for question structures and related functions.

// Global array storing all game questions.
question questions[NUM_QUESTIONS];

// Converts a string to lowercase to standardize answer checking.
void stringToLower(char *s) {
    for (int i = 0; s[i]; i++) {
        s[i] = tolower((unsigned char)s[i]);
    }
}

// Initializes the game questions with the provided categories and questions.
void initialize_game(void) {
    strcpy(questions[0].category, "Threads");
    strcpy(questions[0].question, "This is the smallest unit of execution within a process.");
    strcpy(questions[0].answer, "thread");
    questions[0].value = 100;
    questions[0].answered = false;

    strcpy(questions[1].category, "Threads");
    strcpy(questions[1].question, "Threads within the same process share this type of memory.");
    strcpy(questions[1].answer, "heap");
    questions[1].value = 200;
    questions[1].answered = false;

    strcpy(questions[2].category, "Threads");
    strcpy(questions[2].question, "In multithreading, this mechanism prevents multiple threads from accessing shared resources simultaneously.");
    strcpy(questions[2].answer, "lock");
    questions[2].value = 300;
    questions[2].answered = false;

    strcpy(questions[3].category, "Threads");
    strcpy(questions[3].question, "A thread waiting indefinitely for a resource held by another thread is called a ___.");
    strcpy(questions[3].answer, "deadlock");
    questions[3].value = 400;
    questions[3].answered = false;

    strcpy(questions[4].category, "Processes");
    strcpy(questions[4].question, "A process is an instance of a running ___.");
    strcpy(questions[4].answer, "program");
    questions[4].value = 100;
    questions[4].answered = false;

    strcpy(questions[5].category, "Processes");
    strcpy(questions[5].question, "This is the first process started during system boot on UNIX systems.");
    strcpy(questions[5].answer, "init");
    questions[5].value = 200;
    questions[5].answered = false;

    strcpy(questions[6].category, "Processes");
    strcpy(questions[6].question, "When a process creates another process, the original is called the ___ process.");
    strcpy(questions[6].answer, "parent");
    questions[6].value = 300;
    questions[6].answered = false;

    strcpy(questions[7].category, "Processes");
    strcpy(questions[7].question, "The system call used to create a new process in UNIX.");
    strcpy(questions[7].answer, "fork");
    questions[7].value = 400;
    questions[7].answered = false;

    strcpy(questions[8].category, "Structure");
    strcpy(questions[8].question, "The core component of an operating system is called the ___.");
    strcpy(questions[8].answer, "kernel");
    questions[8].value = 100;
    questions[8].answered = false;

    strcpy(questions[9].category, "Structure");
    strcpy(questions[9].question, "An operating system that separates hardware from user applications is known as a ___ system.");
    strcpy(questions[9].answer, "layered");
    questions[9].value = 200;
    questions[9].answered = false;

    strcpy(questions[10].category, "Structure");
    strcpy(questions[10].question, "This OS structure runs most of its services in user space, with minimal kernel functions.");
    strcpy(questions[10].answer, "microkernel");
    questions[10].value = 300;
    questions[10].answered = false;

    strcpy(questions[11].category, "Structure");
    strcpy(questions[11].question, "The interface between the user and the OS, often a command line, is the ___.");
    strcpy(questions[11].answer, "shell");
    questions[11].value = 400;
    questions[11].answered = false;
}

// Displays each category and the dollar values of unanswered questions.
void display_categories(void) {
    int question_index = 0;
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        printf("%s", categories[i]);
        for (int j = 0; j < NUM_QUESTIONS_PER_CATEGORY; j++) {
            if (questions[question_index].answered) {
                printf(" \e[9m$%i\e[0m", questions[question_index].value);
            } else {
                printf(" $%i", questions[question_index].value);
            }
            if (j < NUM_QUESTIONS_PER_CATEGORY - 1) {
                printf(",");
            }
            question_index++;
        }
        printf("\n");
    }
}

// Displays the specified question for a given category and dollar value.
void display_question(char *category, int value) {
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            printf("Category: %s $%d\n", questions[i].category, questions[i].value);
            printf("Question: %s\n", questions[i].question);
            break;
        }
    }
}

// Checks if a question in a given category and dollar value has already been answered.
bool already_answered(char *category, int value) {
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            return questions[i].answered;
        }
    }
    return false;
}

// Validates a player's answer for a specified category and dollar value.
ValidationResult valid_answer(char *category, int value, char *answer) {
    ValidationResult result = {false, 0};
    stringToLower(answer);
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            result.question = i;
            if (strcmp(questions[i].answer, answer) == 0) {
                result.valid = true;
                return result;
            }
            break;
        }
    }
    return result;
}
