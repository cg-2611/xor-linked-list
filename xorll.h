#ifndef XORLL_H
#define XORLL_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 64

struct node
{
    char *string;
    struct node *link;
};

struct node *XOR(struct node *, struct node *);

int traverse_list(struct node **, struct node **, const char *);

struct node *create_new_node(const char *);
void free_node(struct node *);

void insert_node_front(struct node **, const char *);
void insert_node_end(struct node **, const char *);
void insert_node_before(struct node **, const char *, const char *);
void insert_node_after(struct node **, const char *, const char *);

int remove_node_front(struct node **, char *);
int remove_node_end(struct node **, char *);
int remove_node_before(struct node **, const char *, char *);
int remove_node_after(struct node **, const char *, char *);

void print_list(struct node *);
void free_list(struct node *);

#endif // XORLL_H
