#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include "pa3.h"

static void push(StackNode** stack_top, StackNode* new_node)
{
    new_node -> prev = *stack_top;
    *stack_top = new_node;
}

static StackNode* pop(StackNode** stack_top)
{
    if((*stack_top) -> prev != NULL)
    {
        StackNode* temp = *stack_top;
        *stack_top = (*stack_top) -> prev;
        return temp;
    }
    else
    {
        StackNode* temp = *stack_top;
        *stack_top = NULL;
        return temp;
    }
}

void create_stack(StackNode** stack_top, FILE* fp)
{
    while(!feof(fp))
    {
        int box_num, width, height;

        int num_read = fscanf(fp, "%d(%d,%d)\n", &box_num, &width, &height);
        if(num_read == 3)
        {
            StackNode* Snode = malloc(sizeof(*Snode));
            TreeNode* Tnode = malloc(sizeof(*Tnode));
            *Tnode = (TreeNode){.type = BOX, .box_num = box_num, .width = width, .height = height};
            Snode -> TNode = Tnode;
            Snode -> prev = *stack_top;
            push(stack_top, Snode);
        }
        else
        {
            char split_type;
            int num_read2 = fscanf(fp, "%c\n", &split_type);
            if(num_read2 == 1)
            {
                StackNode* Snode = malloc(sizeof(*Snode));
                TreeNode* Tnode = malloc(sizeof(*Tnode));
                *Tnode = (TreeNode){.type = SPLIT, .split_type = split_type};
                Snode -> TNode = Tnode;
                Snode -> prev = *stack_top;
                push(stack_top, Snode);
            }
        }
    }
    return;
}

TreeNode* stack_to_tree(StackNode** stack_top)
{
    StackNode* new_node = pop(stack_top);
    TreeNode* new_tnode = ((TreeNode*)(new_node -> TNode));
    free(new_node);

    if(new_tnode -> type == BOX)
        return new_tnode;

    if(new_tnode -> type == SPLIT)
    {
        new_tnode -> right = stack_to_tree(stack_top);
        new_tnode -> left = stack_to_tree(stack_top);

        return new_tnode;
    }
    return NULL;
}
    
TreeNode* reroot_right_left(TreeNode* root)
{
    TreeNode* A = root;
    TreeNode* B = root -> right;
    TreeNode* temp = root -> right -> right;

    if(root -> right -> type == BOX)
        return A;

    B -> right = A;
    A -> right = temp;

    return B;
}

// check before if any of the nodes are null 

TreeNode* reroot_left_right(TreeNode* root)
{
    TreeNode* A = root;
    TreeNode* B = root -> left;
    TreeNode* temp = root -> left -> left;

    if(root -> left -> type == BOX)
        return A; 

    B -> left = A;
    A -> left = temp;

    return B;
}

TreeNode* reroot_right_right(TreeNode* root)
{
    TreeNode* A = root;
    TreeNode* B = root -> right;
    TreeNode* temp = root -> right -> left;

    B -> left = A;
    A -> right = temp;

    return B;
}

TreeNode* reroot_left_left(TreeNode* root)
{
    TreeNode* A = root;
    TreeNode* B = root -> left;
    TreeNode* temp = root -> left -> right;

    B -> right = A;
    A -> left = temp;

    return B;
}

static void write_output(TreeNode* head, FILE* fp)
{
    if(head == NULL)
        return;

    if(head -> type == SPLIT)
        fprintf(fp, "%c\n", head -> split_type);
    if(head -> type == BOX)
        fprintf(fp, "%d(%d,%d)\n", head -> box_num, head -> width, head -> height);

    write_output(head -> left, fp);
    write_output(head -> right, fp);

    return;
}

TreeNode* make_output1(TreeNode* root, FILE* fp)
{
    while(root -> left -> type != BOX)
    {
        root = reroot_left_right(root);
        if(!(root -> right -> type == BOX))
            root = reroot_right_left(root);
        else
            break;
    }

    write_output(root, fp);

    return root;
}

TreeNode* make_output2(TreeNode* root, FILE* fp)
{
    while(root -> right -> type != BOX)
    {
        root = reroot_right_left(root);
        if(!(root -> left -> type == BOX))
            root = reroot_left_right(root);
        else
            break;
    }

    write_output(root, fp);

    return root;
}

TreeNode* copy_tree(TreeNode* root)
{
    if(root == NULL)
        return NULL;

    TreeNode* new_root = malloc(sizeof(*new_root));
    *new_root = *root;
    new_root -> left = copy_tree(root -> left);
    new_root -> right = copy_tree(root -> right);

    return new_root;
}

static void free_tree(TreeNode* head) // Free the actual nodes and the info for the boxes
{ 
    if(head == NULL)
        return;

    free_tree(head -> left);
    free_tree(head -> right);

    free(head);        
    return;
}

static int find_split_width(TreeNode* head)
{
    if(head == NULL)
        return 0;

    if(head -> type == BOX)
        return  head -> width;

    int width_left = find_split_width(head -> left);
    int width_right = find_split_width(head -> right);
 
    if(head -> split_type == 'V')
    {
        head -> width = width_left + width_right;
        return width_left + width_right;
    }
    else
    {
        if(width_right > width_left)
        { 
            head -> width = width_right;
            return width_right;
        }
        else
        {
            head -> width = width_left;
            return width_left;
        }
    }
}

static int find_split_height(TreeNode* head)
{
    if(head == NULL)
        return 0;

    if(head -> type == BOX)
        return head -> height;

    int height_left = find_split_height(head -> left);
    int height_right = find_split_height(head -> right);
 
    if(head -> split_type == 'H')
    {
        head -> height = height_left + height_right;
        return height_left + height_right;
    }
    else
    {
        if(height_right > height_left)
        { 
            head -> height = height_right;
            return height_right;
        }
        else
        {
            head -> height = height_left;
            return height_left;
        }
    }
}
/*
static void ta(TreeNode* root)
{
    if(root == NULL)
        return;

    if(root -> type == BOX)
        printf("%d \n", root -> box_num);
    else if(root -> type == SPLIT)
    {
        printf("%c ", root -> split_type);
        if(root -> left -> type == BOX)
            printf("%d ",root -> left -> box_num);
        else
            printf("%c ", root -> left -> split_type);
        if(root -> right -> type == BOX)
            printf("%d\n",root -> right -> box_num);
        else
            printf("%c\n", root -> right -> split_type);
    }
        
    ta(root -> left);
    ta(root -> right);

    return;
}
*/

/*
    temp_root = root -> left -> left;
    new_head = reroot_left_left(root);
    x = find_split_width(new_head);
    y = find_split_height(new_head);
    new_head = reroot_right_right(new_head);
    temp_root -> xdim = x;
    temp_root -> ydim = y;

    temp_root = root -> left -> right;
    new_head = reroot_left_right(root);
    x = find_split_width(new_head);
    y = find_split_height(new_head);
    new_head = reroot_left_right(new_head);
    temp_root -> xdim = x;
    temp_root -> ydim = y;

    temp_root = root -> right -> right;
    new_head = reroot_right_right(root);
    x = find_split_width(new_head);
    y = find_split_height(new_head);
    temp_root -> xdim = x;
    temp_root -> ydim = y;
    new_head = reroot_left_left(new_head);     

    temp_root = root -> right -> left;
    new_head = reroot_right_left(root);
    x = find_split_width(new_head);
    y = find_split_height(new_head);
    new_head = reroot_right_left(new_head);
    temp_root -> xdim = x;
    temp_root -> ydim = y;

            printf("%c ", root -> split_type);
            printf("->");
            if(root -> left -> type == BOX)
                printf(" %d\n", root -> box_num);
            else
                printf(" %c\n", root -> split_type);
*/

void find_reroot_dim(TreeNode* root, bool* is_left)
{
    if(root == NULL || root -> type == BOX)
        return;
    
    if(*is_left)
    {
        // case 1

        // case 2
    }
    else
    {
        // case 3

        // case 4
    }

    *is_left = true;
    find_reroot_dim(root -> left, is_left);

    *is_left = false;
    find_reroot_dim(root -> right, is_left);

    return;
}

static void write_output3(TreeNode* head, FILE* fp)
{
    if(head == NULL)
        return;

    if(head -> type == SPLIT)
    {
        if(head -> xdim == 0)
        {
            fprintf(fp, "%c\n", head -> split_type);
        }
        else
        {
            fprintf(fp, "%c(%d,%d)\n", head -> split_type, head -> xdim, head -> ydim);
        }
    }
    if(head -> type == BOX)
    {
        if(head -> xdim == 0)
        {
            fprintf(fp, "%d\n", head -> box_num);
        }
        else
        {
            fprintf(fp, "%d(%d,%d)\n", head -> box_num, head -> xdim, head -> ydim);
        }
    }

    write_output3(head -> left, fp);
    write_output3(head -> right, fp);

    return;
}


int main(int argc, char* argv[])
{
    char* input_file = argv[1]; 
    FILE* fp = fopen(input_file, "r"); 
    if(fp == NULL) 
        return EXIT_FAILURE;

    StackNode* top_ptr = NULL;
    create_stack(&top_ptr, fp);
    fclose(fp);

    TreeNode* root1 = stack_to_tree(&top_ptr);
    TreeNode* root2 = copy_tree(root1);
    TreeNode* root3 = copy_tree(root1);

    fp = fopen(argv[2], "w");
    if(fp == NULL)
        return EXIT_FAILURE;

    TreeNode* temp = make_output1(root1, fp);
    fclose(fp);
    free_tree(temp); 

    FILE* fp2 = fopen(argv[3], "w");
    if(fp2 == NULL)
        return EXIT_FAILURE;

    TreeNode* temp2 = make_output2(root2, fp2);
    fclose(fp2);
    free_tree(temp2);

    FILE* fp3 = fopen(argv[4], "w");
    if(fp3 == NULL)
        return EXIT_FAILURE;

    bool is_left = false;
    find_reroot_dim(root3, &is_left);
    write_output3(root3, fp3);

    free_tree(root3);
    fclose(fp3);

    return EXIT_SUCCESS;
}
