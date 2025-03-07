#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define MAX_SIZE 10000

int openFile(){
    system("cls");

    char ch[50];
    scanf("%s", ch);
    
    FILE *file = fopen(ch, "rb");
    if(!file) {
        printf("no such file or directory");
        return 1;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char *fstr = (char*)malloc(file_size + 1);
    if(!fstr) {
        perror("mem alloc fail");
        fclose(file);
        return 0;
    }

    fread(fstr, 1, file_size, file);
    fstr[file_size] = '\0';

    fclose(file);
    printf("%s", fstr);
    free(fstr);
    return 0;
}

int newFile() {
    printf("new");
     return 0;
}

void startProgram() {
    system("cls");
    printf("-CEDIT-2025-\n");
    char choice;
    printf("(O) open file | (N) new file | (X) exit program\n");
    scanf("%s", &choice);
    switch(choice) {
        case 'O':
            openFile();
            break;
        case 'N':
            newFile();
            break;
        case 'X':
            printf("closeX");
            break;
        default:
            printf("close");
            break;
    }
}

int main() {
    startProgram();

    return 0;
}