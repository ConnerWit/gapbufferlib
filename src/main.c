#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* todo list
 *
 * cursor implementation
 *  moving left/right
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
} GapBuffer;

void *allocCheck(void *ptr) {
      if (ptr == NULL) {
            fprintf(stderr, "mem alloc fail");
            exit(1);
      }
      return ptr;
}

GapBuffer *initBuffer(const char *str) {
    size_t str_size = strlen(str);

    GapBuffer *Buffer = allocCheck(malloc(sizeof(GapBuffer)));
    Buffer->buf_size = str_size + GAP_SIZE;
    Buffer->buffer = allocCheck(malloc(Buffer->buf_size));

    memcpy(Buffer->buffer, str, str_size);

    Buffer->gap_start = str_size;
    Buffer->gap_end = str_size + GAP_SIZE;

    memset(Buffer->buffer + Buffer->gap_start, '_', GAP_SIZE);
    

    return Buffer;
}

int main() {
    char str[] = "kitten on pizza slice in space";

    GapBuffer *Buffer = initBuffer(str);

    printf("%s\n", Buffer->buffer);

    free(Buffer->buffer);
    free(Buffer);

    
    return 0;
}
