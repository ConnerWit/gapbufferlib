#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* todo list
 *
 * inserting text
 *  growing gap
 *  cursor moves at end of inserted text
 *
 * deleting text
 *  shrinking gap
 *  cursor moves to text before deleted section
 *
 *
*/

size_t GAP_SIZE = 6;

typedef struct {
    char *buffer;
    size_t buf_size;
    size_t gap_start;
    size_t gap_end;
    size_t cursor_pos;
} GapBuffer;

void *allocCheck(void *ptr) {
      if (ptr == NULL) {
            fprintf(stderr, "mem alloc fail");
            exit(1);
      }
      return ptr;
}

GapBuffer *moveGap(GapBuffer *Buffer) {
    size_t gap_distance = Buffer->gap_end - Buffer->gap_start;

    if (Buffer->cursor_pos < Buffer->gap_start) {
        memmove(
                Buffer->buffer + Buffer->cursor_pos + gap_distance,
                Buffer->buffer + Buffer->cursor_pos,
                Buffer->gap_start - Buffer->cursor_pos
                );
        Buffer->gap_start = Buffer->cursor_pos;
        Buffer->gap_end = Buffer->gap_start + gap_distance;
    } else {
        memmove(
                Buffer->buffer + Buffer->gap_start,
                Buffer->buffer + Buffer->gap_end,
                Buffer->cursor_pos - Buffer->gap_end
               );
        Buffer->gap_start = Buffer->cursor_pos;
        Buffer->gap_end = Buffer->gap_start + gap_distance;
    }

    memset(Buffer->buffer + Buffer->gap_start, '_', gap_distance);

    return Buffer;
}

size_t clamp(size_t value, size_t min, size_t max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

GapBuffer *moveLeft(GapBuffer *Buffer, size_t to_move) {
    size_t current_pos = Buffer->cursor_pos;

    Buffer->cursor_pos = clamp(current_pos - to_move, 0, Buffer->buf_size);

    moveGap(Buffer);

    return Buffer;
}

GapBuffer *moveRight(GapBuffer *Buffer, size_t to_move) {
    size_t current_pos = Buffer->cursor_pos;

    Buffer->cursor_pos = clamp(current_pos + to_move, 0, Buffer->buf_size);

    moveGap(Buffer);

    return Buffer;
}


GapBuffer *initBuffer(const char *str) {
    size_t str_size = strlen(str);

    GapBuffer *Buffer = allocCheck(malloc(sizeof(GapBuffer)));
    Buffer->buf_size = str_size + GAP_SIZE;
    Buffer->buffer = allocCheck(malloc(Buffer->buf_size));

    memcpy(Buffer->buffer, str, str_size);

    Buffer->gap_start = str_size;
    Buffer->gap_end = str_size + GAP_SIZE;
    Buffer->cursor_pos = Buffer->gap_start;

    memset(Buffer->buffer + Buffer->gap_start, '_', GAP_SIZE);
    

    return Buffer;
}

int main() {
    char str[] = "kitten on pizza slice in space";

    GapBuffer *Buffer = initBuffer(str);

    printf("current buffer:\n%s\n", Buffer->buffer);

    moveLeft(Buffer, 4);

    printf("buffer after moving cursor to the left:\n%s\n", Buffer->buffer);

    free(Buffer->buffer);
    free(Buffer);

    
    return 0;
}
