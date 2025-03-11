#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buffer[50];
int gap_size;
int gap_left;
int gap_right;
int size;

void initGaps() {
    int gap_size = 10;
    int gap_left = 0;
    int gap_right = gap_size - gap_left-1;
    int size = 10;
}

void grow(int k, int position) {
    char a[size];

    for (int i = 0; i < size; i++) {
        a[i - position] = buffer[i];
    }

    for (int i = 0; i < k; i++) {
        buffer[i + position] = '_';
    }
}

void left(int position) {
    while(position < gap_left) {
        gap_left--;
        gap_right--;
        buffer[gap_right+1] = buffer[gap_left];
        buffer[gap_left] = '_';
    }
}

void right(int position) {
    while (position > gap_left) {
        gap_left++;
        gap_right++;
        buffer[gap_left-1] = buffer[gap_right];
        buffer[gap_right] = '_';
    }
}

void moveCursor(int position) {
    if(position < gap_left) {
        left(position);
    }
    else {
        right(position);
    }
}

void insert(char input[5], int position) {
    size_t len = strlen(input);
    int i = 0;

    if(position != gap_left) {
        moveCursor(position);
    }

    while(i < len) {
        if(gap_right == gap_left) {
            int k = 10;
            grow(k, position);
        }

        buffer[gap_left] = input[i];
        gap_left++;
        i++;
        position++;
    }
}

int main() {
    initGaps();
    system("cls");

    for(int i = 0; i < 10; i++) {
        buffer[i] = '_';
    }

    printf("initializing the gap buffer with size 10\n");

    for (int i = 0; i < size; i++) {
        printf("%c ", buffer[i]);
    }
    printf("\n");

    char input[5] = "hello";
    int position = 0;

    insert(input, position);

    return 0;
}