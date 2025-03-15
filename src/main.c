#include <stdio.h>
#include <string.h>

void editLine(int currentLine, char *buffer) {
    for (int i = 0; i < currentLine; i++) {
        buffer = strchr(buffer, '\n') + 1;
    }

    char *lineEnd = strchr(buffer, '\n');
    char saved[1034] = {0};
    strcpy(saved, lineEnd);
    scanf("%s", buffer);
    strcpy(buffer + strlen(buffer), saved);
}

int main(int argc, char** argv) {
    FILE *f = fopen(argv[1], "r");
    char buffer[1024] = {0};
    fread(buffer, 1024, 1, f);
    fclose(f);
    printf("content:\n%s\n", buffer);
    int currentLine = 0;
    scanf("%d", &currentLine);

    editLine(currentLine, buffer);

    f = fopen(argv[1], "w");
    fwrite(buffer, strlen(buffer), 1, f);
    fclose(f);

    return 0;
}