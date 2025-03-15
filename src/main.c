#include <stdio.h>
#include <stdlib.h>

void openFile(char *filename) {
    system("cls");
    FILE *file = fopen(filename, "a+");

    char line[256];

    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            printf("%s", line);
        }
        fclose(file);
    } else {
        fprintf(stderr, "unable to open file\n");
    }
}

void newFile(char *filename) {
    system("cls");
    FILE *file = fopen(filename, "w");
}

int main() {
    system("cls");
    printf("CEDIT 2025\nenter filename to open / create: ");
    char filename[256];
    scanf("%s", filename);

    FILE *file = fopen(filename, "r");

    if (file != NULL) {
        openFile(filename);
    } else {
        newFile(filename);
    }
    
    return 0;
}
