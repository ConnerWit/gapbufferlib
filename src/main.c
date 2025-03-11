#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buffer[50];
int gap_size;
int gap_left;
int gap_right;
int size;

void initGaps() {
    gap_size = 10;
    gap_left = 0;
    gap_right = gap_size - gap_left-1;
    size = 10;
}

void grow(int k, int position) {
    char a[size];

    for (int i = 0; i < size; i++) {
        a[i] = buffer[i];
    }
    for (int i = 0; i < k; i++) {
        buffer[position + i] = '_';
    }

    for (int i = position; i < size; i++) {
        buffer[i + k] = a[i];
    }

    size += k;
    gap_right += k;
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
            grow(10, gap_left);
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

    char input[1048] = "hello !";
    int position = 0;

    insert(input, position);

    printf("\nInserting a string to buffer\n'hello'\noutput:\n");
    for (int i = 0; i < size; i++) {
        printf("%c ", buffer[i]);
    }

    strcpy(input, "bro ");
    position = 0;

    insert(input, position);

    printf("\n\ninserting a string to buffer\n'world'\noutput:\n");
    for(int i = 0; i < size; i++) {
        printf("%c ", buffer[i]);
    }

    return 0;
}