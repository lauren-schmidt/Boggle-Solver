#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <math.h>
#include <string.h>

#include "llist.h"
#include "hashmap.h"

// local function for hashing an input value (do not edit)
unsigned int _hash(hashmap_t *hm, char *val)
{
  // based on Java's String.hashcode()
  double k = 0;
  double len = (double)strlen(val);
  int i = 1;
  for (char *c = val; *c; c++, i++)
  {
    k += (*c) * pow(31, len - i);
  }

  return (unsigned int)(((unsigned long)k) % hm->num_buckets);
}

// Resize the hash map by doubling the number of buckets and rehashing.
void _resize(hashmap_t *hm)
{
  // Create a new hashmap with double the number of buckets
  int orig_num_buckets = hm->num_buckets;
  int new_buckets = orig_num_buckets * 2;
  llist_t **new_bucket_list = calloc(new_buckets, sizeof(llist_t *));

  for (int i = 0; i < new_buckets; i++)
  {
    new_bucket_list[i] = ll_init();
  }

  // Rehash according to new hashmap
  hm->num_buckets = new_buckets;

  for (int i = 0; i < orig_num_buckets; i++)
  {
    // Crea†e a temp node
    ll_node_t *temp = hm->buckets[i]->head;
    while (temp != NULL)
    {
      char *val = temp->val;
      unsigned int h = _hash(hm, val); // Get hash
      ll_push(new_bucket_list[h], val);
      temp = temp->next;
    }
  }

  // Set buckets to new bucket list, make sure that orig bucket list doesn't get leaked
  llist_t **orig_bucket_list = hm->buckets;
  hm->buckets = new_bucket_list;

  // Delete original buckets
  for (int i = 0; i < orig_num_buckets; i++)
  {
    ll_delete(orig_bucket_list[i]);
  }

  free(orig_bucket_list);
}

// Initialize a hashmap with initial number of buckets
hashmap_t *hm_init()
{

  // Create the hashmap
  hashmap_t *hm = calloc(1, sizeof(hashmap_t));

  // Create the buckets and initialize each linked list
  hm->buckets = calloc(HM_INIT_NUM_BUCKETS, sizeof(llist_t *));

  for (int i = 0; i < HM_INIT_NUM_BUCKETS; i++)
  {
    hm->buckets[i] = ll_init();
  }

  // Initialize parameters of hm struct
  hm->num_buckets = HM_INIT_NUM_BUCKETS;
  hm->size = 0;

  return hm;
}

// Delete/deallocate the hashmap
void hm_delete(hashmap_t *hm)
{

  // Iterate through hm->buckets and delete the lls in each bucket
  for (int i = 0; i < hm->num_buckets; i++)
  {
    ll_delete(hm->buckets[i]);
  }

  free(hm->buckets);

  // Delete the hashmap
  free(hm);
}

// Add a string value to the hashmap
void hm_add(hashmap_t *hm, char *val)
{
  // You get this function for free :)

  // Check the load on the hashtable, if > max load, resize!
  if (((hm->size + 1) / (double)hm->num_buckets) > HM_MAX_LOAD)
  {
    _resize(hm);
  }

  unsigned int h = _hash(hm, val); // Get hash
  ll_push(hm->buckets[h], val);    // Note that list does string dup
  hm->size++;                      // Increase size
}

// See if a string value is in the hashmap
bool hm_check(hashmap_t *hm, char *val)
{
  // Find bucket and then iterate through linked list
  unsigned int h = _hash(hm, val);

  // Create a pointer to the linkedlist
  llist_t *currBucket = hm->buckets[h];

  // Create a current node to iterate through the ll
  ll_node_t *current = currBucket->head;

  // Iterate through the ll, checking to see if each value compares correctly.
  while (current != NULL)
  {
    if (strcmp(current->val, val) == 0)
    {
      return true;
    }
    current = current->next;
  }

  return false; // Otherwise return false
}
