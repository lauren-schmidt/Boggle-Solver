#include <stdlib.h>
#include <string.h>

#include "llist.h"

// Initialize a linked list
llist_t *ll_init()
{
  // Create a new ll and allocate space for it.
  llist_t *list = malloc(sizeof(llist_t));

  // Initialize its variables to NULL since we have no info.
  list->size = 0;
  list->head = NULL;

  return list;
}

// Delete a linked list
void ll_delete(llist_t *ll)
{
  // Create two pointer nodes to keep track as we parse the ll.
  ll_node_t *current = ll->head;
  ll_node_t *next;

  // Only continue on the condition that the current node is not NULL.
  while (current != NULL)
  {
    // Set the next node and free the current. Assign the next to be the current.
    next = current->next;
    free(current->val);
    free(current);
    current = next;
  }
  free(ll);
}

// Push an item onto the front of the list
void ll_push(llist_t *ll, char *s)
{
  // Create new node and set its value
  ll_node_t *newNode = malloc(sizeof(ll_node_t));
  newNode->val = strdup(s);

  // Set the new node's next value to the current list's head.
  newNode->next = ll->head;

  // Set the current head to be the new node.
  ll->head = newNode;

  ll->size++;
}
