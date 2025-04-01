#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LEAF_LEN 8

struct Rope {
	char data[LEAF_LEN + 1];
	int weight;
  struct Rope *left;
  struct Rope *right;
};

struct Rope *initializeRope(char *string) {
  int len = strlen(string);
  
  if(len <= LEAF_LEN) {
    struct Rope *node = malloc(sizeof(struct Rope));
    if(node == NULL) {
      fprintf(stderr, "mem alloc fail 0\n");
      exit(1);
    }

    strcpy(node->data, string);
    node->weight = len;
    node->left = NULL;
    node->right = NULL;
    printf("created leaf node with %s\n", node->data);
    return node;
  }

  int len1 = len / 2;
  int len2 = len - len1;

  char *s1 = malloc(len1 + 1);
  char *s2 = malloc(len2 + 1);

  if(s1 == NULL || s2 == NULL) {
    fprintf(stderr, "mem alloc fail 1\n");
    exit(1);
  }

  strncpy(s1, string, len1);
  s1[len1] = '\0';

  strncpy(s2, string + len1, len2);
  s2[len2] = '\0';

  struct Rope *node = malloc(sizeof(struct Rope));
  if(node == NULL) {
    fprintf(stderr, "mem alloc fail 2\n");
    exit(1);
  }

  node->data[0] = '\0';
  node->weight = len1;

  node->left = initializeRope(s1);
  node->right = initializeRope(s2);

  printf("created internal node with weight: %d\n", node->weight);

  free(s1);
  free(s2);

  return node;
}

void printRope(struct Rope *root, int level) {
  if(root == NULL) return;

  for(int i = 0; i < level; i++) printf(" ");

  if(root->left == NULL && root->right == NULL) {
    printf("leaf: \"%s\"\n", root->data);
  } else {
    printf("node: weight=%d\n", root->weight);
    printRope(root->left, level + 1);
    printRope(root->right, level + 1);
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

  printf("\nrope:\n");
  printRope(root, 0);

  freeRope(root);
	return 0;
}
