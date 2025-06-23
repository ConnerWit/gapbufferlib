#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEAF_LEN 5
#define MAX_IMBALANCE 3

typedef struct Rope {
  char data[LEAF_LEN + 1];
  size_t weight;
  struct Rope *left;
  struct Rope *right;
} Rope;

void *allocCheck(void *ptr) {
      if (ptr == NULL) {
            fprintf(stderr, "mem alloc fail");
            exit(1);
      }
      return ptr;
}

void freeRope(Rope *root) {
      if (root == NULL)
            return;

      freeRope(root->left);
      freeRope(root->right);
      free(root);
}

size_t calculateRopeLength(Rope *root) {
      if (root == NULL)
            return 0;

      if (root->left == NULL && root->right == NULL) {
            return strlen(root->data);
      }

      return calculateRopeLength(root->left) + calculateRopeLength(root->right);
}

char *initializeString(Rope *root) {
      if (root == NULL)
        return 0;

      size_t stringLength = calculateRopeLength(root) + 1;
      char *output = allocCheck(malloc(stringLength));
      output[0] = '\0';

      return output;
}

void storeRope(struct Rope *root, char *output) {
      if (root == NULL)
            return;

      if (root->left == NULL && root->right == NULL) {
            strcat(output, root->data);
      } else {
            storeRope(root->left, output);
            storeRope(root->right, output);
      }
}

Rope *initializeRope(char *str) {
      size_t len = strlen(str);

      if (len <= LEAF_LEN) {
            Rope *leafNode = allocCheck(malloc(sizeof(Rope)));
            strcpy(leafNode->data, str);
            leafNode->weight = len;
            leafNode->left = NULL;
            leafNode->right = NULL;

            return leafNode;
      }

      size_t len1 = len / 2;
      size_t len2 = len - len1;

      char *str1 = allocCheck(malloc(len1 + 1));
      char *str2 = allocCheck(malloc(len2 + 1));

      strncpy(str1, str, len1);
      str1[len1] = '\0';
      strncpy(str2, str + len1, len2);
      str2[len2] = '\0';

      Rope *node = allocCheck(malloc(sizeof(Rope)));
      node->data[0] = '\0';
      node->weight = len1;
      node->left = initializeRope(str1);
      node->right = initializeRope(str2);

      free(str1);
      free(str2);
      return node;
}

int getHeight(Rope *root) {
      if (root == NULL)
            return 0;

      if (root->left == NULL && root->right == NULL) {
            return 1;
      }

      int leftHeight = getHeight(root->left);
      int rightHeight = getHeight(root->right);

      return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}

bool isBalanced(Rope *root) {
      int leftHeight = getHeight(root->left);
      int rightHeight = getHeight(root->right);

      if (abs(leftHeight - rightHeight) > MAX_IMBALANCE) {
            return false;
      }

      return true;
}

void updateWeight(Rope *root) {
      if (root == NULL)
            return;

      if (root->left == NULL) {
            root->weight = 0;
      } else if (root->left->left == NULL && root->left->right == NULL) {
            root->weight = strlen(root->left->data);
      } else {
            root->weight = calculateRopeLength(root->left);
      }
}

Rope *rotateRight(Rope *y) {
      if (y == NULL || y->left == NULL)
            return y;

      Rope *x = y->left;
      Rope *T2 = x->right;

      x->right = y;
      y->left = T2;

      updateWeight(y);
      updateWeight(x);

      return x;
}

Rope *rotateLeft(Rope *x) {
      if (x == NULL || x->right == NULL)
            return x;

      Rope *y = x->right;
      Rope *T2 = y->left;

      y->left = x;
      x->right = T2;

      updateWeight(x);
      updateWeight(y);

      return y;
}

Rope *rebalance(Rope *root) {
      if (root == NULL)
            return 0;

      int leftHeight = getHeight(root->left);
      int rightHeight = getHeight(root->right);

      if (leftHeight > rightHeight + MAX_IMBALANCE) {
            Rope *leftChild = root->left;
            if (leftChild == NULL)
                  return root;

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
            if (rightChild == NULL)
                  return root;

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

Rope *concatenate(Rope *first, Rope *second) {
      if (first == NULL)
            return second;
      if (second == NULL)
            return first;

      Rope *resultRope = allocCheck(malloc(sizeof(Rope)));
      resultRope->data[0] = '\0';
      resultRope->left = first;
      resultRope->right = second;
      updateWeight(resultRope);

      if (!isBalanced(resultRope)) {
            resultRope = rebalance(resultRope);
      }

      return resultRope;
}

size_t search(Rope *root, size_t val) {
      if (root == NULL)
            return 1;

      if (root->right == NULL && root->left == NULL) {
            return root->data[val];
      }

      if (val < root->weight) {
            return search(root->left, val);
      } else {
            return search(root->right, val - root->weight);
      }
}

void split(Rope *root, size_t val) {

}

void insert() {
      // split at index
      // concatenate first half with new string
      // concatenate fist half with second half
}

void delete() {
      // split two times to isolate text to be deleted
      // free middle
      // concatenate first and last half
}

void printRope(Rope *root) {
      if (root == NULL)
            return;

      char *str = initializeString(root);
      storeRope(root, str);
      printf("%s", str);
      free(str);
}

int main() {
      char str[100] =
              "this is a string i want to store in the new rope data structure";
      Rope *rope = initializeRope(str);
      printRope(rope);

      freeRope(rope);
      return 0;
}
