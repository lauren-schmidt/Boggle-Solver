#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "hashmap.h"
#include "llist.h"

#define MAX_WORD_LEN 1024


//Spell check program to test your hashmap dictionary
//
// HINT: Try starting with a smaller dictionary, maybe 20 words

int main(int argc, char * argv[]){

  FILE * dict_f;
  char word[MAX_WORD_LEN];
  hashmap_t * hm;
  char * line;
   
  if(argc < 2){
    printf("ERROR: require dictionary file\n");
    exit(1);
  }

  if((dict_f = fopen(argv[1],"r")) == NULL){
    perror("opening dictionary file");
    exit(1);
  }


  hm = hm_init();
  while(fscanf(dict_f, "%1023s", word) != EOF){
    for(char *c=word;(*c=toupper(*c)) != 0;c++);
    hm_add(hm,word);
  }
  fclose(dict_f);

  while(1){
    char * line = readline("spellcheck > ");
    if(line == NULL){
      hm_delete(hm);
      printf("\n");
      break;
    }

    if(0 == strtok(line, "")){
      continue;
    }

    char * tok = strtok(line, " ");
    do{
      for(char *c=tok;(*c=toupper(*c)) != 0;c++);
    
      if(hm_check(hm,tok)){
        printf("%s -> WORD\n",tok);
      }else{
        printf("%s -> not a word\n",tok);
      }
    }while((tok=strtok(NULL," ")) != NULL);
  
    free(line);
  }

  return 0;
}
