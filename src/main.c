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

void *allocCheck(void *ptr) {
  if(ptr == NULL) {
    fprintf(stderr, "mem alloc fail");
    exit(1);
  }
  return ptr;
}

void freeRope(struct Rope *root) {
  if(root == NULL) return;
  freeRope(root->left);
  freeRope(root->right);
  free(root);
}

struct Rope *initializeRope(char *string) {
  size_t len = strlen(string);
  
  if(len <= LEAF_LEN) {
    struct Rope *node = allocCheck(malloc(sizeof(struct Rope)));
    strcpy(node->data, string);
    node->weight = len;
    node->left = NULL;
    node->right = NULL;
    printf("created leaf node with %s\n", node->data);
    return node;
  }

  size_t len1 = len / 2;
  size_t len2 = len - len1;

  char *s1 = allocCheck(malloc(len1 + 1));
  char *s2 = allocCheck(malloc(len2 + 1));

  strncpy(s1, string, len1);
  s1[len1] = '\0';
  strncpy(s2, string + len1, len2);
  s2[len2] = '\0';

  struct Rope *node = allocCheck(malloc(sizeof(struct Rope)));
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

struct Rope *insert(struct Rope *root, char *newStr, size_t pos) {
  if(root == NULL) return NULL;

  size_t bufferLen = 1024;
  char *buffer = allocCheck(malloc(bufferLen));
  buffer[0] = '\0';
  printRope(root, 0, buffer);

  size_t oldLen = strlen(buffer);
  size_t newLen = strlen(newStr);
  char *result = allocCheck(malloc(oldLen + newLen + 1));

  strncpy(result, buffer, pos);
  result[pos] = '\0';
  strcat(result, newStr);
  strcat(result, buffer + pos);

  struct Rope *newRoot = initializeRope(result);

  free(buffer);
  free(result);
  freeRope(root);

  return newRoot;
}

int main() {
  char string[] = "this is a string that i want to store inside a leaf node";
  struct Rope *root = initializeRope(string);

  size_t len1 = strlen(string) + 1;
  char *output = allocCheck(malloc(len1));
  output[0] = '\0';
  printRope(root, 0, output);
  printf("\noutput:\n%s\n\n", output);

  char newStr[] = ", this is the added text inside the rope struct,";
  root = insert(root, newStr, 16);

  size_t len2 = strlen(string) + strlen(newStr) + 1;
  char *output2 = allocCheck(malloc(len2));
  output2[0] = '\0';
  printRope(root, 0, output2);
  printf("\noutput:\n%s\n\n", output2);
  

  freeRope(root);
	return 0;
}
