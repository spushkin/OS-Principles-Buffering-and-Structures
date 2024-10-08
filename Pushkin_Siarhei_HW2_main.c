/**************************************************************
* Class::  CSC-415-01 Summer 2024
* Name:: Siarhei Pushkin
* GitHub-Name:: spushkin
* Project:: Assignment 2 – Stucture in Memory and Buffering
*
* File:: Pushkin_Siarhei_HW2_main.c
*
* Description:: This program captures information from 
* command-line arguments, stores it in a structure, writes the 
* data using a provided function, and handles additional text 
* data by reading strings into a fixed-size buffer, processing 
* them in chunks, and committing the buffer when full. It also
* performs a final check to ensure successful execution.
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assignment2.h"

int main(int argc, char *argv[]) {

    // allocate and populate personalInfo structure
    personalInfo *pInfo = (personalInfo *)malloc(sizeof(personalInfo));
    if (!pInfo) {
        printf("Failed to allocate memory\n");
        return 1;
    }

    pInfo->firstName = argv[1];
    pInfo->lastName = argv[2];
    pInfo->studentID = 922907437;
    pInfo->level = SENIOR;

    pInfo->languages = KNOWLEDGE_OF_C | KNOWLEDGE_OF_JAVA | KNOWLEDGE_OF_PYTHON 
    | KNOWLEDGE_OF_JAVASCRIPT;
    strncpy(pInfo->message, argv[3], sizeof(pInfo->message) - 1);
    pInfo->message[sizeof(pInfo->message) - 1] = '\0';

    // write personalInfo and check for success
    if (writePersonalInfo(pInfo) != 0) {
        printf("Failed to write Personal Info\n");
        free(pInfo);
        return 1;
    }

    // allocate buffer for block operations
    char *buffer = (char *)malloc(BLOCK_SIZE);
    if (!buffer) {
        printf("Failed to allocate buffer\n");
        free(pInfo);
        return 1;
    }

    // read strings and copy to buffer, commit when full
    size_t buffer_index = 0;
    const char *str;
    while ((str = getNext()) != NULL) {
        size_t str_len = strlen(str);

        while (str_len > 0) {
            size_t space_left = BLOCK_SIZE - buffer_index;
            size_t copy_len = (str_len < space_left) ? str_len : space_left;

            memcpy(buffer + buffer_index, str, copy_len);
            buffer_index += copy_len;
            str += copy_len;
            str_len -= copy_len;

            if (buffer_index == BLOCK_SIZE) {
                commitBlock(buffer);
                buffer_index = 0;
            }
        }
    }

    if (buffer_index > 0) {
        commitBlock(buffer);
    }

    free(buffer);
    free(pInfo);

    return checkIt();
}
