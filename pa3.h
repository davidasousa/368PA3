#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

typedef struct _TreeNode {
    struct _TreeNode* left;
    struct _TreeNode* right;
    enum type{SPLIT,BOX} type;
    char split_type;
    int box_num;
    int width;
    int height;
    int xdim;
    int ydim;
} TreeNode;

typedef struct _StackNode {
    struct _StackNode* prev;
    TreeNode* TNode;
} StackNode;

void create_stack(StackNode** stack_top, FILE* fp);

TreeNode* stack_to_tree(StackNode** stack_top);

// By here the stack has been destroyed and the tree has been fully  generated

TreeNode* reroot_right_left(TreeNode* root);

TreeNode* reroot_left_right(TreeNode* root);

TreeNode* reroot_right_right(TreeNode* root);

TreeNode* reroot_left_left(TreeNode* root);

TreeNode* copy_tree(TreeNode* root);

TreeNode* make_output1(TreeNode* root, FILE* fp);

TreeNode* make_output2(TreeNode* root, FILE* fp);

void find_reroot_dim(TreeNode* root, bool* is_left);

TreeNode* make_output3(TreeNode* root, FILE* fp);
