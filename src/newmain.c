#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LEAF_LEN 5
#define MAX_IMBALANCE 3

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

int getHeight(Rope *rope) {
  if (rope == NULL) {
    return 0;
  }

  if (rope->left == NULL && rope->right == NULL) {
    return 1;
  }

  int leftHeight = getHeight(rope->left);
  int rightHeight = getHeight(rope->right);

  return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

bool balanceCheck(Rope *rope) {
  int leftHeight = getHeight(rope->left);
  int rightHeight = getHeight(rope->right);

  if (abs(leftHeight - rightHeight) > MAX_IMBALANCE) {
    return false;
  }

  return true;
}

Rope *rotateRight(Rope *y) {
  Rope *x = y->left;
  Rope *T2 = x->right;

  x->right = y;
  y->left = T2;

  y->weight = y->weight - x->weight;
  if (T2 != NULL) {
    y->weight += T2->weight;
  }

  return x;
}

Rope *rotateLeft(Rope *x) {
  Rope *y = x->right;
  Rope *T2 = y->left;

  y->left = x;
  x->right = T2;

  Rope *xright = x->right;
  Rope *yleft = y->left;

  y->weight = x->weight - xright->weight;
  if (yleft != NULL) {
    y->weight += yleft->weight;
  }

  return y;
}

Rope *rebalance(Rope *root) {
  int leftHeight = getHeight(root->left);
  int rightHeight = getHeight(root->right);

  if (leftHeight > rightHeight + MAX_IMBALANCE) {
    Rope *leftChild = root->left;
    int leftLeftHeight = getHeight(leftChild->left);
    int leftRightHeight = getHeight(leftChild->right);

    if (leftLeftHeight >= leftRightHeight) {
      return rotateRight(root);
    } else {
      root->left = rotateLeft(leftChild);
      return rotateRight(root);
    }
  } else if (rightHeight > leftHeight + MAX_IMBALANCE) {
    Rope *rightChild = root->right;
    int rightLeftHeight = getHeight(rightChild->left);
    int rightRightHeight = getHeight(rightChild->right);

    if (rightRightHeight >= rightLeftHeight) {
      return rotateLeft(root);
    } else {
      root->right = rotateRight(rightChild);
      return rotateLeft(root);
    }
  }
  
  return root;
}

Rope *concatenate(Rope *rope, Rope *secondRope) {
  Rope *resultRope = allocCheck(malloc(sizeof(Rope)));
  resultRope->weight = rope->weight;
  resultRope->left = rope;
  resultRope->right  = secondRope;
  
  if (!balanceCheck(resultRope)) {
    resultRope = rebalance(resultRope);
  }

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
