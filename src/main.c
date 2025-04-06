#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LEAF_LEN 8

struct Rope {
	char data[LEAF_LEN + 1];
	size_t weight;
  struct Rope *left;
  struct Rope *right;
};

void *allocCheck(void *ptr, size_t size) {
  if(ptr == NULL) {
    fprintf(stderr, "mem alloc fail", size);
    exit(1);
  }
  return ptr;
}

struct Rope *initializeRope(char *string) {
  size_t len = strlen(string);
  
  if(len <= LEAF_LEN) {
    struct Rope *node = allocCheck(malloc(sizeof(struct Rope)), sizeof(struct Rope));

    strcpy(node->data, string);
    node->weight = len;
    node->left = NULL;
    node->right = NULL;
    printf("created leaf node with %s\n", node->data);
    return node;
  }

  size_t len1 = len / 2;
  size_t len2 = len - len1;

  char *s1 = allocCheck(malloc(len1 + 1), len1 + 1);
  char *s2 = allocCheck(malloc(len2 + 1), len2 + 1);


  strncpy(s1, string, len1);
  s1[len1] = '\0';

  strncpy(s2, string + len1, len2);
  s2[len2] = '\0';

  struct Rope *node = allocCheck(malloc(sizeof(struct Rope)), sizeof(struct Rope));

  node->data[0] = '\0';
  node->weight = len1;

  node->left = initializeRope(s1);
  node->right = initializeRope(s2);

  printf("created internal node with weight: %d\n", node->weight);

  free(s1);
  free(s2);

  return node;
}

void printRope(struct Rope *root, size_t level, char *output) {
  if(root == NULL) return;

  if(root->left == NULL && root->right == NULL) {
    strcat(output, root->data);
  } else {
    printRope(root->left, level + 1, output);
    printRope(root->right, level + 1, output);
  }
}

void freeRope(struct Rope *root) {
  if(root == NULL) return;
  freeRope(root->left);
  freeRope(root->right);
  free(root);
}

int main() {
  char string[] = "this is a string that i want to store inside a leaf node";
  struct Rope *root = initializeRope(string);

  size_t len = strlen(string) + 1;
  char *output = allocCheck(malloc(len), len);
  output[0] = '\0';

  printRope(root, 0, output);

  printf("%s", output);

  freeRope(root);
	return 0;
}
