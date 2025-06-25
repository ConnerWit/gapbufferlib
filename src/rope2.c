#include <stdio.h>
#include <string.h>

#define LEAF_lEN 512

typedef struct LeafBuffer {
    char* buffer;
    size_t buffer_size;
    size_t gap_start;
    size_t gap_end;
    size_t text_length;
} LeafBuffer;

typedef struct Rope {
    LeafBuffer leaf_data;
    size_t weight;
    struct Rope* left;
    struct Rope* right;
    bool is_leaf;
} Rope;

void *allocCheck(void *ptr) {
      if (ptr == NULL) {
            fprintf(stderr, "mem alloc fail");
            exit(1);
      }
      return ptr;
}

Rope* ropeInit(char* str) {
    size_t len = strlen(str);
    if (len <= LEAF_LEN) {

    }
}

int main() {

    return 0;
}
