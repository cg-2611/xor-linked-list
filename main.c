#include "xorll.h"

int main(int argc, char *argv[])
{
    struct node *head = NULL;

    insert_node_front(&head, "Alpha");
    insert_node_front(&head, "Bravo");
    insert_node_end(&head, "Charlie");
    insert_node_before(&head, "Alpha", "Delta");
    insert_node_after(&head, "Delta", "Echo");
    print_list(head);

    char string[MAX_SIZE];
    int remove_status;

    remove_status = remove_node_before(&head, "Charlie", string);
    printf("\nRemoved %s (status: %d)\n\n", string, remove_status);
    print_list(head);

    remove_status = remove_node_after(&head, "Delta", string);
    printf("\nRemoved %s (status: %d)\n\n", string, remove_status);
    print_list(head);

    remove_status = remove_node_front(&head, string);
    printf("\nRemoved %s (status: %d)\n\n", string, remove_status);
    print_list(head);

    remove_status = remove_node_end(&head, string);
    printf("\nRemoved %s (status: %d)\n\n", string, remove_status);
    print_list(head);

    free_list(head);

    return 0;
}