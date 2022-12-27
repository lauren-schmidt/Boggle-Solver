#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "llist.h"
#include "hashmap.h"
#include "boggle.h"

// constant values on all the dice in global
const char *boggle_dice[] = {
    "AAAFRS",
    "AAEEEE",
    "AAFIRS",
    "ADENNN",
    "AEEEEM",
    "AEEGMU",
    "AEGMNN",
    "AFIRSY",
    "BJKQXZ",
    "CCENST",
    "CEIILT",
    "CEILPT",
    "CEIPST",
    "DDHNOT",
    "DHHLOR",
    "DHLNOR",
    "DHLNOR",
    "EIIITT",
    "EMOTTT",
    "ENSSSU",
    "FIPRSY",
    "GORRVW",
    "IPRRRY",
    "NOOTUW",
    "OOOTTU",
};

// initialize a boggle instance (do not edit until after the TODO: comment)
boggle_t *bg_init(int seed, char *dict_fn)
{
  // You get this first part of the function for free :)

  // set a random seed
  srandom(seed);

  // create the instance
  boggle_t *bg = calloc(sizeof(boggle_t), 1);

  // setup the board by rolling dice
  int used[BOGGLE_NUM_DICE] = {0};
  for (int i = 0; i < BOGGLE_NUM_DICE; i++)
  {
    while (1)
    {
      // choose a dice to roll
      int d = random() % BOGGLE_NUM_DICE;
      if (used[d])
        continue; // keep choosing if already rolles

      // choose a random side
      int s = random() % BOGGLE_DICE_SIDES;
      bg->board[i / BOGGLE_DIMENSION][i % BOGGLE_DIMENSION] = boggle_dice[d][s];
      used[d] = 1; // mark that dice as rolled
      break;
    }
  }

  // Open the dictionary file and create a hashmap
  FILE *dictFile;
  char line[1024];
  hashmap_t *dict;

  // Error check
  if ((dictFile = fopen(dict_fn, "r")) == NULL)
  {
    perror("error opening dictionary file");
    exit(1);
  }

  // Put all valid words (greater than or eq to 3 letterss, less than or eq to 8)
  // into the "dict" hashmap
  dict = hm_init();

  // Scan through the file, add each word, and close out the file.
  while (fscanf(dictFile, "%1023s", line) != EOF)
  {
    for (char *temp = line; (*temp = toupper(*temp)) != 0; temp++)
    {
      if (strlen(line) >= 3 && strlen(line) <= 8)
      {
        hm_add(dict, line);
      }
    }
  }
  fclose(dictFile);

  bg->dict = dict;

  return bg;
}

// delete/free the boggle instance (do not edit)
void bg_delete(boggle_t *bg)
{
  // You get this function for free :)
  hm_delete(bg->dict);
  free(bg);
}

// print the board using fancy unicode (do not edit)
void bg_print(boggle_t *bg)
{
  printf(".-----------.\n");
  for (int r = 0; r < 5; r++)
  {
    printf("| ");
    for (int c = 0; c < 5; c++)
    {
      printf("%c", bg->board[r][c]);
      if (bg->board[r][c] == 'Q')
        printf("u");
      else
        printf(" ");
    }
    printf("|\n");
  }
  printf("'-----------'\n");
}

// count the points up (do not edit)
//  3 and 4 length words are worth 1 point
//  5 length worth 2
//  6 length worth 3
//  7 length worth 5
//  8 length worth 11
//  note we stop searching at 8 length words
int bg_count_points(boggle_t *bg, llist_t *words)
{
  int pts = 0;
  for (ll_node_t *n = words->head; n; n = n->next)
  {
    if (hm_check(bg->dict, n->val))
    {
      switch (strlen(n->val))
      {
      case 0:
      case 1:
      case 2:
        break;
      case 3:
      case 4:
        pts += 1;
        break;
      case 5:
        pts += 2;
        break;
      case 6:
        pts += 3;
        break;
      case 7:
        pts += 5;
        break;
      default:
        pts += 11;
      }
    }
  }
  return pts;
}

// TODO: complete the board search algorithm below using your hashmap

// helper functions for the recursion, declaring here so can call below
void _bg_all_words(boggle_t *bg,                                        // boggle instance
                   hashmap_t *hm,                                       // hashmap to track found words
                   int r, int c,                                        // current row (r) / column (c)
                   char *last_word,                                     // what the last word was
                   int last_visited[BOGGLE_DIMENSION][BOGGLE_DIMENSION] // what has been previously visited
                                                                        // don't want to double back
);

// entry function for finding all the words
llist_t *bg_all_words(boggle_t *bg)
{
  // 0'ed out -- currently visited nowhere (all 0's), and the current word is the empty string
  int visited[BOGGLE_DIMENSION][BOGGLE_DIMENSION] = {0};
  char cur_word[9] = {0}; // empty string
  // Note using byte array of length 9 b/c max word length is 8!

  // Initialize a hashmap to track status
  hashmap_t *hm = hm_init();

  // Start a recursion at every space on the board
  for (int i = 0; i < BOGGLE_DIMENSION; i++)
  {
    for (int j = 0; j < BOGGLE_DIMENSION; j++)
    {
      _bg_all_words(bg, hm, i, j, cur_word, visited);
    }
  }

  // Return a linkedlist of words
  llist_t *foundWords = ll_init();
  for (int i = 0; i < hm->num_buckets; i++)
  {
    if (hm->buckets[i] != NULL)
    {
      ll_node_t *curWord = hm->buckets[i]->head;
      while (curWord != NULL)
      {
        ll_push(foundWords, curWord->val);
        curWord = curWord->next;
      }
    }
  }

  hm_delete(hm); // delete the hashmap

  // return the list of found words
  return foundWords;
}

void _bg_all_words(boggle_t *bg, hashmap_t *hm, int r, int c,
                   char *last_word, int last_visited[BOGGLE_DIMENSION][BOGGLE_DIMENSION])
{

  // Starter Variables
  char cur_word[9] = {0}; // use to store the current word
  int visited[BOGGLE_DIMENSION][BOGGLE_DIMENSION] = {0};

  // BOTTOM-OUT CASE 1: Word is over 9 characters (including null terminator at the end)
  if (strlen(last_word) > 8)
  {
    return;
  }

  // BOTTOM-OUT CASE 2: Outside of board dimensions
  if (r >= BOGGLE_DIMENSION || r < 0 || c >= BOGGLE_DIMENSION || c < 0)
  {
    return;
  }

  // BOTTOM-OUT CASE 3: Have already visited this dice.
  if (last_visited[r][c] == 1)
  {
    return;
  }

  // Set our 'visited' square to the last visited square:
  for (int i = 0; i < BOGGLE_DIMENSION; i++)
  {
    for (int j = 0; j < BOGGLE_DIMENSION; j++)
    {
      visited[i][j] = last_visited[i][j];
    }
  }

  // Attempt to create a new word
  strcpy(cur_word, last_word);
  cur_word[strlen(cur_word)] = bg->board[r][c];

  // Special case -- if current letter Q, add a u if we have the space
  if (bg->board[r][c] == 'Q' && strlen(cur_word) < 8)
  {
    cur_word[strlen(cur_word)] = 'U';
  }
  cur_word[strlen(cur_word)] = '\0';

  // Check to see if this 'new word' is in our dictionary file
  // If it is a word and we have not found it, save it in the hashmap.
  if (hm_check(bg->dict, cur_word) == 1 && hm_check(hm, cur_word) == 0)
  {
    hm_add(hm, cur_word);
  }
  // Mark that we have visited this tile:
  visited[r][c] = 1;

  // Perform 8 recursive calls on all surrounding dice:
  _bg_all_words(bg, hm, r - 1, c, cur_word, visited);
  _bg_all_words(bg, hm, r - 1, c - 1, cur_word, visited);
  _bg_all_words(bg, hm, r - 1, c + 1, cur_word, visited);
  _bg_all_words(bg, hm, r + 1, c, cur_word, visited);
  _bg_all_words(bg, hm, r + 1, c - 1, cur_word, visited);
  _bg_all_words(bg, hm, r + 1, c + 1, cur_word, visited);
  _bg_all_words(bg, hm, r, c - 1, cur_word, visited);
  _bg_all_words(bg, hm, r, c + 1, cur_word, visited);
}
