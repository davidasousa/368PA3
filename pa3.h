#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct _TreeNode {
    struct _TreeNode* left;
    struct _TreeNode* right;
    enum type{SPLIT,BOX} type;
    void* info;
} TreeNode;

typedef struct _StackNode {
    struct _StackNode* prev;
    void* info;
} StackNode;

typedef struct _Split {
    char split_type;
} Split;

typedef struct _Box {
    int box_num;
    int width;
    int height;
} Box;

void create_stack(StackNode** stack_top, FILE* fp);
