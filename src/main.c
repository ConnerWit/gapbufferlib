#include <stdio.h>
#include <stdlib.h>

#define GAP_SIZE 10
#define GAP_LEFT 0
#define GAP_RIGHT (GAP_SIZE - GAP_LEFT-1)

char buffer[1024];
int size = 10;

void grow(int k, int pos) {
    char a[size];

    for (int i = pos; i < size; i++) {
        a[i - pos] = buffer[i];
    }
    for (int i = 0; i < k; i++) {
        buffer[i + pos] = '_';
    }
    for (int i = 0; i < pos + k; i++) {
        buffer[pos + k + i] = a[i];
    }
    size += k;
    GAP_RIGHT += k;
}

void insert(char *filebuffer, int pos, int len) {
    int i = 0;
    int gap_left = GAP_LEFT;
    int gap_right = GAP_RIGHT;

    while (i < len) {
        if (GAP_RIGHT == GAP_LEFT) {
            int k = 10;
            grow(k, pos);
        }
        buffer[GAP_LEFT] = filebuffer[i];
        gap_left++;
        i++;
        pos++;
    }
}

int main() {
    for (int i = 0; i < 10; i++) {
        buffer[i] = '_';
    }
    printf("initialized gap buffer of size 10\n");
    for(int i = 0; i < size; i++) {
        printf("%c", buffer[i]);
    }
    printf("\n");

    FILE *file = fopen("testfile.c", "r");
    if (file == NULL) {
        perror("error opening file");
        return 1;
    }
    char filebuffer[1024];
    int i = 0, c;
    while ((c = fgetc(file)) != EOF) {
        filebuffer[i] = c;
        i++;
    }
    int pos = 0;

    printf("%s", filebuffer);

    insert(filebuffer, pos, strlen(filebuffer));

    return 0;
}
