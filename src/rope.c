#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define MIN_GAP_SIZE 256
#define MAX_GAP_SIZE 4096
#define GAP_RATIO 0.25

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

size_t calculateGapSize(size_t file_size) {
    size_t gap_size = file_size * GAP_RATIO;

    if(gap_size < MIN_GAP_SIZE) return MIN_GAP_SIZE;
    if(gap_size > MAX_GAP_SIZE) return MAX_GAP_SIZE;

    return gap_size;
}

size_t getFileSize(char* file_path) {
    FILE* fp = fopen(file_path, "r");
    if(!fp) return SIZE_MAX;

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    fclose(fp);
    return file_size;
}

char* readFileContent(char* file_path, size_t file_size) {
    FILE* fp = fopen(file_path, "r");
    if(!fp) return NULL;

    char* buffer = allocCheck(malloc(file_size + 1));
    fread(buffer, 1, file_size, fp);
    buffer[file_size] = '\0';

    fclose(fp);
    return buffer;
}


Rope* ropeInit(char* file_path) {
    size_t file_size = getFileSize(file_path);
    if(file_size == SIZE_MAX) return NULL;
    char* file_content = readFileContent(file_path, file_size);
    if(!file_content) return NULL;

    Rope* root = allocCheck(malloc(sizeof(Rope)));
    root->left = NULL;
    root->right = NULL;
    root->is_leaf = true;
    root->weight = file_size;

    size_t gap_size = calculateGapSize(file_size);
    root->leaf_data.buffer = malloc(file_size + gap_size);
    root->leaf_data.buffer_size = file_size + gap_size;
    root->leaf_data.gap_start = file_size;
    root->leaf_data.gap_end = file_size + gap_size;
    root->leaf_data.text_length = file_size;

    memcpy(root->leaf_data.buffer, file_content, file_size);

    free(file_content);
    return root;
}

int main() {

    return 0;
}
