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

Rope* ropeLeafFromBuffer(const char* buffer, size_t length) {
    Rope* node = allocCheck(malloc(sizeof(Rope)));
    node->left = NULL;
    node->right = NULL;
    node->is_leaf = true;
    node->weight = length;

    size_t gap_size = calculateGapSize(length);
    node->leaf_data.buffer = malloc(length + gap_size);
    node->leaf_data.buffer_size = length + gap_size;
    node->leaf_data.gap_start = length;
    node->leaf_data.gap_end = length + gap_size;
    node->leaf_data.text_length = length;

    memcpy(node->leaf_data.buffer, buffer, length);

    return node;
}

Rope* ropeInit(char* file_path) {
    size_t file_size = getFileSize(file_path);
    if(file_size == SIZE_MAX) return NULL;
    char* file_content = readFileContent(file_path, file_size);
    if(!file_content) return NULL;

    Rope* root = ropeLeafFromBuffer(file_content, file_size);
    free(file_content);
    return root;
}

Rope* ropeSplitLeaf(Rope* node, size_t index, Rope** right_out) {
    if (!node || !node->is_leaf || index > node->leaf_data.text_length) return NULL;

    size_t text_len = node->leaf_data.text_length;
    char* temp_buffer = allocCheck(malloc(text_len));
    size_t cursor = 0;

    memcpy(temp_buffer, node->leaf_data.buffer, node->leaf_data.gap_start);
    cursor += node->leaf_data.gap_start;

    size_t after_gap_len = text_len - node->leaf_data.gap_start;
    memcpy(temp_buffer + cursor,
            node->leaf_data.buffer + node->leaf_data.gap_end,
            after_gap_len);

    Rope* left_node = ropeLeafFromBuffer(temp_buffer, index);
    Rope* right_node = ropeLeafFromBuffer(temp_buffer + index, text_len - index);

    free(node->leaf_data.buffer);
    free(node);

    free(temp_buffer);
    *right_out = right_node;
    return left_node;
}

size_t totalLength(Rope* node) {
    if (!node) return 0;
    if (node->is_leaf) {
        return node->leaf_data.text_length;
    }
    return totalLength(node->left) + totalLength(node->right);
}

Rope* makeInternalNode(Rope* left, Rope* right) {
    Rope* node = allocCheck(malloc(sizeof(Rope)));
    node->is_leaf = false;
    node->left = left;
    node->right = right;
    node->weight = totalLength(left);
    return node;
}

void ropeSplit(Rope* node, size_t index, Rope** left_out, Rope** right_out) {
    if (!node) return;

    if (node->is_leaf) {
        Rope* right_leaf = NULL;
        Rope* left_leaf = ropeSplitLeaf(node, index, &right_leaf);
        *left_out = left_leaf;
        *right_out = right_leaf;
        return;
    }

    if (index < node->weight) {
        Rope* left_sub = NULL, *right_sub = NULL;
        ropeSplit(node->left, index, &left_sub, &right_sub);

        Rope* new_right = makeInternalNode(right_sub, node->right);
        *left_out = left_sub;
        *right_out = new_right;
    } else {
        Rope *left_sub = NULL, *right_sub = NULL;
        ropeSplit(node->right, index - node->weight, &left_sub, &right_sub);

        Rope* new_left = makeInternalNode(node->left, left_sub);
        *left_out = new_left;
        *right_out = right_sub;
    }
}

size_t ropeHeight(Rope* node) {
    if (!node) return 0;
    if (node->is_leaf) return 1;
    size_t left_h = ropeHeight(node->left);
    size_t right_h = ropeHeight(node->right);
    return 1 + (left_h > right_h ? left_h : right_h);
}

Rope* ropeConcatBalanced(Rope* left, Rope* right) {
    if (!left) return right;
    if (!right) return left;

    int height_diff = (int)ropeHeight(left) - (int)ropeHeight(right);

    if (abs(height_diff) <= 1) {
        return makeInternalNode(left, right);
    } else if (height_diff > 1) {
        Rope* new_left = left->left;
        Rope* new_right = ropeConcatBalanced(left->right, right);
        return makeInternalNode(new_left, new_right);
    } else {
        Rope* new_left = ropeConcatBalanced(left, right->left);
        Rope* new_right = right->right;
        return makeInternalNode(new_left, new_right);
    }
}

Rope* insert(Rope* root, size_t index, const char* text) {
    if (!root) return root;

    Rope *left_tree = NULL, *right_tree = NULL;
    ropeSplit(root, index, &left_tree, &right_tree);

    Rope* new_node = ropeLeafFromBuffer(text, strlen(text));
    Rope* combined = ropeConcatBalanced(left_tree, new_node);
    Rope* result = ropeConcatBalanced(combined, right_tree);

    return result;
}

Rope* delete(Rope* root, size_t range_start, size_t range_end) {
    if (range_start >= range_end || !root) return root;

    Rope *left_tree = NULL, *mid_tree = NULL, *right_tree = NULL;
    ropeSplit(root, range_end, &left_tree, &right_tree);
    ropeSplit(left_tree, range_start, &left_tree, &mid_tree);
    Rope* result = ropeConcatBalanced(left_tree, right_tree);
    
    return result;
}
    

void ropePrint(Rope* node) {
    if (!node) return;

    if (node->is_leaf) {
        printf("\"");
        for (size_t i = 0; i < node->leaf_data.gap_start; i++) {
            putchar(node->leaf_data.buffer[i]);
        }
        for (size_t i = node->leaf_data.gap_end;
             i < node->leaf_data.gap_start + (node->leaf_data.text_length - node->leaf_data.gap_start);
             i++) {
            putchar(node->leaf_data.buffer[i]);
        }
        printf("\"");
    } else {
        printf("(");
        ropePrint(node->left);
        printf(" + ");
        ropePrint(node->right);
        printf(")");
    }
}

int main() {
    // Rope* r1 = ropeLeafFromBuffer("Hello ", 6);
    // Rope* r2 = ropeLeafFromBuffer("World!", 6);

    // Rope* root = ropeConcatBalanced(r1, r2);
    // printf("Initial rope: ");
    // ropePrint(root);
    // printf("\n");

    // Rope *left = NULL, *right = NULL;
    // ropeSplit(root, 7, &left, &right);
    // printf("After split at 7:\n");
    // printf("Left: "); ropePrint(left); printf("\n");
    // printf("Right: "); ropePrint(right); printf("\n");

    return 0;
}
