#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TupleInsert {
    char string;
    int num;
};

int initGaps() {
    int gap_size = 10;
    int gap_left = 0;
    int gap_right = gap_size - gap_left - 1;

    return gap_size, gap_left, gap_right;
}

char initBuffer() {
    FILE *file = fopen("testfile.c", "r");
    if(file == NULL) {
        perror("error opening file");
        return 1;
    }

    char buffer[1024];
    int i = 0, c;
    while((c = fgetc(file)) != EOF) {
        buffer[i] = c;
        i++;
    }

    return *buffer;
}

int addGapLeft(int gap_left) {
    int newGapLeft = gap_left + 1;
    return newGapLeft;
}

struct TupleInsert insert(char *buffer, int pos, int gap_left, int gap_right) {
    int len = strlen(buffer);
    int i = 0;
    char *gap_buffer = malloc(gap_left * sizeof(char));

    if(pos != gap_left) {
        //move cursor function
    }

    while(i < len) {
        if(gap_right == gap_left) {
            //grow gap function
        }
        char *gap_buffer = realloc(gap_buffer, gap_left * sizeof(char));
        gap_left = addGapLeft(gap_left);
        i++;
        pos++;

        struct TupleInsert r = {*gap_buffer, gap_left};
        return r;

        if(!gap_buffer) {
            perror("err allocating gap_buffer");
            char errchar[5] = "error";
            int errnum = 1;
            struct TupleInsert err = {*errchar, errnum};
            return err;
        }
        free(gap_buffer);
    }
}

int main() {
    initBuffer();
    char buffer = buffer;

    initGaps();
    int gap_left = gap_left;
    int gap_right = gap_right;
    
    int pos = 0;

    insert(&buffer, pos, gap_left, gap_right);

    return 0;
}
