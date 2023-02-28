#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
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
    StackNode* top_node = NULL;

    while(!feof(fp))
    {
        int box_num, width, height;
        int num_read = fscanf(fp, "%d(%d,%d)\n", &box_num, &width, &height);
        if(num_read == 3)
        {
            StackNode* new_snode = malloc(sizeof(*new_snode));
            TreeNode* new_tnode = malloc(sizeof(*new_tnode));
            Box* box_info = malloc(sizeof(*box_info));

            *box_info = (Box){.box_num = box_num, .width = width, .height = height};
            *new_tnode = (TreeNode){.type = BOX, .info = box_info, .left = NULL, .right = NULL};
            *new_snode = (StackNode){.prev = NULL, .info = &new_tnode};

            push(&top_node, new_snode);
        }

        char split_type;
        num_read = fscanf(fp, "%c\n", &split_type);
        if(num_read == 1)
        {
            StackNode* new_snode = malloc(sizeof(*new_snode));
            TreeNode* new_tnode = malloc(sizeof(*new_tnode));
            Split* split_info = malloc(sizeof(*split_info));

            *split_info = (Split){.split_type = split_type};
            *new_tnode = (TreeNode){.type = BOX, .info = split_info, .left = NULL, .right = NULL};
            *new_snode = (StackNode){.prev = NULL, .info = &new_tnode};

            push(&top_node, new_snode);

        }     
    }
}

int main(int argc, char* argv[])
{
    char a = 'A';
    char a2 = 'B';
    StackNode* node1 = malloc(sizeof(*node1));
    *node1 = (StackNode){.info = &a, .prev = NULL};
    StackNode* node2 = malloc(sizeof(*node2));
    *node2 = (StackNode){.info = &a2, .prev = NULL};
    StackNode* stack_top = NULL;

    push(&stack_top, node1);
    printf("%c", *(char*)(stack_top -> info));
    push(&stack_top, node2);
    printf("%c", *(char*)(stack_top -> info));
    pop(&stack_top);
    printf("%c", *(char*)(stack_top -> info));
    StackNode* n = pop(&stack_top);
    printf("%c", *(char*)(n -> info));
    free(node1);
    free(node2);

    return EXIT_SUCCESS;
}
