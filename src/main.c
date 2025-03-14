#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>  // For _getch() on Windows

#define MAX_LINE_LENGTH 1024
#define MAX_FILENAME_LENGTH 256

// Function to display file content with line numbers
void display_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int line_number = 1;
    
    // Clear screen (Windows-specific)
    system("cls");
    
    printf("File: %s\n", filename);
    printf("--------------------------------------------------\n");
    
    // Read and display each line of the file
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        printf("%4d | %s", line_number++, line);
        // If the line doesn't end with a newline, add one
        if (line[strlen(line) - 1] != '\n') {
            printf("\n");
        }
    }

    fclose(file);
    printf("--------------------------------------------------\n");
    printf("Press any key to continue...");
    _getch();  // Wait for key press
}

// Function to get filename from user
void get_filename(char* filename) {
    printf("Enter filename to open: ");
    fgets(filename, MAX_FILENAME_LENGTH, stdin);
    
    // Remove newline character if present
    size_t len = strlen(filename);
    if (len > 0 && filename[len-1] == '\n') {
        filename[len-1] = '\0';
    }
}

int main() {
    char filename[MAX_FILENAME_LENGTH];
    char choice;
    
    do {
        // Clear screen
        system("cls");
        
        printf("Simple File Editor\n");
        printf("1. Open a file\n");
        printf("q. Quit\n");
        printf("\nEnter your choice: ");
        
        choice = _getch();
        printf("%c\n", choice);
        
        switch (choice) {
            case '1':
                get_filename(filename);
                display_file(filename);
                break;
            case 'q':
            case 'Q':
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Press any key to continue...");
                _getch();
        }
    } while (choice != 'q' && choice != 'Q');
    
    return 0;
}