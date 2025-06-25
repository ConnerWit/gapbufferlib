#include <stdio.h>

typedef struct LeafBuffer {
    char *buffer;
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

int main() {

    return 0;
}
