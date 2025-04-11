#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEAF_LEN 5

typedef struct Rope {
	char data[LEAF_LEN + 1];
	size_t weight;
  struct Rope *left;
  struct Rope *right;
} Rope;

void *allocCheck(void *ptr) {
  if(ptr == NULL) {
    fprintf(stderr, "mem alloc fail");
    exit(1);
  }
  return ptr;
}

void freeRope(Rope *root) {
  if(root == NULL) return;
  freeRope(root->left);
  freeRope(root->right);
  free(root);
}

char *initializeString(Rope *rope) {
  size_t len = rope->weight * 2;
  char *output = allocCheck(malloc(len));
  output[0] = '\0';
}

void storeRope(struct Rope *root, char *output) {
  if(root == NULL) return;

  if(root->left == NULL && root->right == NULL) {
    strcat(output, root->data);
  } else {
    storeRope(root->left, output);
    storeRope(root->right, output);
  }
}

Rope *initializeRope(char *str) {
  size_t len = strlen(str);
  
  if(len <= LEAF_LEN) {
    Rope *node = allocCheck(malloc(sizeof(Rope)));
    strcpy(node->data, str);
    node->weight = len;
    node->left = NULL;
    node->right = NULL;
    return node;
  }

  size_t len1 = len / 2;
  size_t len2 = len - len1;

  char *s1 = allocCheck(malloc(len1 + 1));
  char *s2 = allocCheck(malloc(len2 + 1));

  strncpy(s1, str, len1);
  s1[len1] = '\0';
  strncpy(s2, str + len1, len2);
  s2[len2] = '\0';

  Rope *node = allocCheck(malloc(sizeof(Rope)));
  node->data[0] = '\0';
  node->weight = len1;
  node->left = initializeRope(s1);
  node->right = initializeRope(s2);

  free(s1);
  free(s2);
  return node;
}

void balanceCheck(Rope *rope) {
  //check if rebalancing is needed
  //char *str = initializeString(rope);
  //storeRope(rope, str);
  //initializeRope(str);
}

Rope *concatenate(Rope *rope, Rope *secondRope) {
  Rope *resultRope = allocCheck(malloc(sizeof(Rope)));
  resultRope->data[0] = '\0';
  resultRope->weight = rope->weight;
  resultRope->left = rope;
  resultRope->right  = secondRope;
  //balanceCheck(resultRope);

  return resultRope;
}

void search() {
  //find index of position given
}

void split() {
  //search for index to split at
  //split functionality
}

void insert() {
  //split at index
  //concatenate first half with new string
  //concatenate fist half with second half
}

void delete() {
  //split two times to isolate text to be deleted
  //free middle
  //concatenate first and last half
}

int main() {
  char str[100] = "this is a string i want to store in the new rope data structure.";
  Rope *rope = initializeRope(str);

  char *output = initializeString(rope);
  storeRope(rope, output);
  printf("%s", output);

  freeRope(rope);
  return 0;
}
