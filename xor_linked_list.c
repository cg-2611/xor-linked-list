#include "xorll.h"

#define MAX_SIZE 64

// returns the xor of node1 and node 2
struct node *XOR(struct node *node1, struct node *node2)
{
    return (struct node*) ((uintptr_t)(node1) ^ (uintptr_t)(node2));
}

// allocates memory for a new node and its string, copies the string to the node and returns it
struct node *create_new_node(const char *node_string)
{
    struct node *new_node = malloc(sizeof(*new_node));

    if (new_node != NULL)
    {
        new_node->string = malloc((strlen(node_string) + 1) * sizeof(*(new_node->string)));

        if (new_node->string != NULL)
        {
            strcpy(new_node->string, node_string);
            return new_node;
        }

        free(new_node);
        new_node = NULL;
    }

    return NULL;
}

// deallocates the memory of the node and its string
void free_node(struct node *node)
{
    free(node->string);
    node->string = NULL;

    free(node);
    node = NULL;
}

// traverses the linked list until it either reaches the end or the target and populates the nodes array
int traverse_list(struct node **nodes, struct node **head_p, const char *target)
{
    int target_found = 0;

    struct node *previous_node = NULL;
    struct node *current_node = *head_p;
    struct node *next_node = NULL;

    // only traverse when the list is not empty
    if (current_node != NULL)
    {
        while (current_node != NULL)
        {
            if (target != NULL)
            {
                if (strcmp(current_node->string, target) == 0)
                {
                    // stop looping if a target is provided and it is found in the list
                    target_found = 1;
                    break;
                }
            }

            // break when the end of the loop is reached
            if ((next_node = XOR(previous_node, current_node->link)) == NULL)
            {
                break;
            }

            previous_node = current_node;
            current_node = next_node;
        }

        // if the loop has been broken, update the next node in the list
        if (current_node != NULL)
        {
            next_node = XOR(previous_node, current_node->link);
        }
    }

    nodes[0] = previous_node; // the node that comes before the current node in the linked list
    nodes[1] = current_node;  // either the target node if provided and found, or the end node of the linked list
    nodes[2] = next_node;     // the node that comes after the current node in the linked list

    return target_found;
}

// inserts a node at the beginning of the linked list
void insert_node_front(struct node **head_p, const char *node_string)
{
    if ((strlen(node_string) + 1) * sizeof(*node_string) > MAX_SIZE)
    {
        fprintf(stderr, "Cannot insert node \"%s\": string too many bytes, must be %d bytes at most\n", node_string, MAX_SIZE);
        return;
    }

    struct node *new_node = create_new_node(node_string);

    if (new_node == NULL)
    {
        fprintf(stderr, "Cannot insert node \"%s\": a problem occurred when allocating memory\n", node_string);
        return;
    }

    // adjust the links for the new node and if the list is not empty, then adjust the links for the old head node
    new_node->link = XOR(NULL, *head_p);

    if (*head_p != NULL)
    {
        struct node *head_link = XOR((*head_p)->link, NULL);
        (*head_p)->link = XOR(new_node, head_link);
    }

    *head_p = new_node;
}

// inserts a node at the end of the linked list
void insert_node_end(struct node **head_p, const char *node_string)
{
    if ((strlen(node_string) + 1) * sizeof(*node_string) > MAX_SIZE)
    {
        fprintf(stderr, "Cannot insert node \"%s\": string too many bytes, must be %d bytes at most\n", node_string, MAX_SIZE);
        return;
    }

    if (*head_p == NULL)
    {
        insert_node_front(head_p, node_string);
        return;
    }

    struct node *nodes[3];
    traverse_list(nodes, head_p, NULL);

    // get the current final node in the list
    struct node *end_node = nodes[1];

    struct node *new_node = create_new_node(node_string);

    if (new_node == NULL)
    {
        fprintf(stderr, "Cannot insert node \"%s\": a problem occurred when allocating memory\n", node_string);
        return;
    }

    // adjust eh links for the new node and the old end node
    new_node->link = XOR(end_node, NULL);
    end_node->link = XOR(end_node->link, XOR(new_node, NULL));
}

// inserts a node before a given node in the linked list
void insert_before_node(struct node **head_p, const char *target, const char *node_string)
{
    if ((strlen(node_string) + 1) * sizeof(*node_string) > MAX_SIZE)
    {
        fprintf(stderr, "Cannot insert node \"%s\": string too many bytes, must be %d bytes at most\n", node_string, MAX_SIZE);
        return;
    }

    // if the list is empty, insert the node at the beginning
    if (*head_p == NULL)
    {
        insert_node_front(head_p, node_string);
        return;
    }

    struct node *nodes[3];
    int target_exists = traverse_list(nodes, head_p, target);

    if (!target_exists)
    {
        fprintf(stderr, "Cannot insert node \"%s\": target node \"%s\" is not in the list\n", node_string, target);
        return;
    }

    // get the target node and the node that comes before it in the list
    struct node *previous_node = nodes[0];
    struct node *current_node = nodes[1];

    // if the target node is the head node, insert the new node at the beginning of the list
    if (previous_node == NULL)
    {
        insert_node_front(head_p, node_string);
        return;
    }

    struct node *new_node = create_new_node(node_string);

    if (new_node == NULL)
    {
        fprintf(stderr, "Cannot insert node \"%s\": a problem occurred when allocating memory\n", node_string);
        return;
    }

    // adjust the links for the new node
    new_node->link = XOR(previous_node, current_node);

    // adjust the links for the surrounding nodes
    previous_node->link = XOR(previous_node->link, XOR(current_node, new_node));
    current_node->link = XOR(XOR(previous_node, new_node), current_node->link);
}

// inserts a node before a given node in the linked list
void insert_after_node(struct node **head_p, const char *target, const char *node_string)
{
    if ((strlen(node_string) + 1) * sizeof(*node_string) > MAX_SIZE)
    {
        fprintf(stderr, "Cannot insert node \"%s\": string too many bytes, must be %d bytes at most\n", node_string, MAX_SIZE);
        return;
    }

    // if the list is empty, insert the node at the beginning
    if (*head_p == NULL)
    {
        insert_node_front(head_p, node_string);
        return;
    }

    struct node *nodes[3];
    int target_exists = traverse_list(nodes, head_p, target);

    if (!target_exists)
    {
        fprintf(stderr, "Cannot insert node \"%s\": target node \"%s\" is not in the list\n", node_string, target);
        return;
    }

    // get the target node and the node that comes after it in the list
    struct node *current_node = nodes[1];
    struct node *next_node = nodes[2];

    // if the target node is the head node, insert the new node at the beginning of the list
    if (next_node == NULL)
    {
        insert_node_end(head_p, node_string);
        return;
    }

    struct node *new_node = create_new_node(node_string);

    if (new_node == NULL)
    {
        fprintf(stderr, "Cannot insert node \"%s\": a problem occurred when allocating memory\n", node_string);
        return;
    }

    // adjust the links for the new node
    new_node->link = XOR(current_node, next_node);

    // adjust the links for the surrounding nodes
    current_node->link = XOR(XOR(next_node, new_node), current_node->link);
    next_node->link = XOR(next_node->link, XOR(current_node, new_node));
}

// removes the node at the beginning of the list
int remove_front_node(struct node **head_p, char *removed_string)
{
    if (*head_p == NULL)
    {
        fprintf(stderr, "Cannot remove node: list is empty\n");
        return -1;
    }

    struct node *previous_head = *head_p;

    // get the new head pointer
    *head_p = XOR(NULL, previous_head->link);

    if (*head_p != NULL)
    {
        // adjust the links for the new head node as long as the list is not empty
        (*head_p)->link = XOR(NULL, XOR(previous_head, (*head_p)->link));
    }

    // copy the string from the removed node to the argument
    strcpy(removed_string, previous_head->string);

    free_node(previous_head);
    return 0;
}

// remove the node at the end of the list
int remove_end_node(struct node **head_p, char *removed_string)
{
    if (*head_p == NULL)
    {
        fprintf(stderr, "Cannot remove node: list is empty\n");
        return -1;
    }

    struct node *nodes[3];
    traverse_list(nodes, head_p, NULL);

    // get the end node end node and the node that comes before it
    struct node *previous_end = nodes[1];
    struct node *new_end = nodes[0];

    // get the new end node
    new_end = XOR(previous_end->link, NULL);

    if (new_end != NULL)
    {
        // adjust the links for the new end node as long as the list is not empty
        new_end->link = XOR(XOR(previous_end, new_end->link), NULL);
    }

    // copy the string from the removed node to the argument
    strcpy(removed_string, previous_end->string);

    free_node(previous_end);
    return 0;
}

// removes the node that comes before given node in the linked list
int remove_node_before(struct node **head_p, const char *target, char *removed_string)
{
    if (*head_p == NULL)
    {
        fprintf(stderr, "Cannot remove node before \"%s\": list is empty\n", target);
        return -1;
    }

    struct node *nodes[3];
    int target_exists = traverse_list(nodes, head_p, target);

    if (!target_exists)
    {
        fprintf(stderr, "Cannot remove node before \"%s\": target node \"%s\" is not in the list\n", target, target);
        return -1;
    }

    // get the target node a and the nodes the come before and after it
    struct node *previous_node = nodes[0];
    struct node *current_node = nodes[1];
    struct node *next_node = nodes[2];

    if (previous_node == NULL)
    {
        fprintf(stderr, "Could not remove before node \"%s\": there is no node that comes before target node \"%s\"\n", target, target);
        return -1;
    }

    // shift the nodes so that current_node is the node that is to be removed
    next_node = current_node;
    current_node = previous_node;
    previous_node = XOR(current_node->link, next_node);

    // adjust the node links after the removal
    previous_node->link = XOR(previous_node->link, XOR(current_node, next_node));
    next_node->link = XOR(XOR(current_node, previous_node), next_node->link);

    // copy the string from the removed node to the argument
    strcpy(removed_string, current_node->string);

    free_node(current_node);
    return 0;
}

// removes the node that comes after given node in the linked list
int remove_node_after(struct node **head_p, const char *target, char *removed_string)
{
    if (*head_p == NULL)
    {
        fprintf(stderr, "Cannot remove node after \"%s\": list is empty\n", target);
        return -1;
    }

    struct node *nodes[3];
    int target_exists = traverse_list(nodes, head_p, target);

    if (!target_exists)
    {
        fprintf(stderr, "Cannot remove node after \"%s\": target node \"%s\" is not in the list\n", target, target);
        return -1;
    }

    // get the target node a and the nodes the come before and after it
    struct node *previous_node = nodes[0];
    struct node *current_node = nodes[1];
    struct node *next_node = nodes[2];

    if (next_node == NULL)
    {
        fprintf(stderr, "Could not remove before node \"%s\": there is no node that comes after target node \"%s\"\n", target, target);
        return -1;
    }

    // shift the nodes so that current_node is the node that is to be removed
    previous_node = current_node;
    current_node = next_node;
    next_node = XOR(previous_node, current_node->link);

    // adjust the node links after the removal
    previous_node->link = XOR(previous_node->link, XOR(current_node, next_node));
    next_node->link = XOR(XOR(current_node, previous_node), next_node->link);

    // copy the string from the removed node to the argument
    strcpy(removed_string, current_node->string);

    free_node(current_node);
    return 0;
}

// print the linked list showing the links between the nodes
void print_list(struct node *head)
{
    if (head == NULL)
    {
        printf("empty\n");
        return;
    }

    struct node *previous_node = NULL;
    struct node *current_node = head;
    struct node *next_node;

    do
    {
        printf("%s <-> ", current_node->string);

        next_node = XOR(previous_node, current_node->link);
        previous_node = current_node;
        current_node = next_node;
    } while (current_node != NULL);

    printf("NULL\n");
}

// deallocate memory for every node in the list
void free_list(struct node *head)
{
    if (head == NULL)
    {
        return;
    }

    struct node *temp_node;
    struct node *previous_node = NULL;
    struct node *current_node = head;
    struct node *next_node;

    do
    {
        temp_node = current_node;
        next_node = XOR(previous_node, current_node->link);
        previous_node = current_node;
        current_node = next_node;

        free_node(temp_node);
    } while (current_node != NULL);
}

int main(int argc, char *argv[])
{
    struct node *head = NULL;

    insert_node_front(&head, "Alpha");
    insert_node_front(&head, "Bravo");
    insert_node_end(&head, "Charlie");
    insert_before_node(&head, "Alpha", "Delta");
    insert_after_node(&head, "Delta", "Echo");
    print_list(head);

    char string[MAX_SIZE];
    int remove_status;

    remove_status = remove_node_before(&head, "Charlie", string);
    printf("\nRemoved %s (status: %d)\n\n", string, remove_status);
    print_list(head);

    remove_status = remove_node_after(&head, "Delta", string);
    printf("\nRemoved %s (status: %d)\n\n", string, remove_status);
    print_list(head);

    remove_status = remove_front_node(&head, string);
    printf("\nRemoved %s (status: %d)\n\n", string, remove_status);
    print_list(head);

    remove_status = remove_end_node(&head, string);
    printf("\nRemoved %s (status: %d)\n\n", string, remove_status);
    print_list(head);

    free_list(head);

    return 0;
}
