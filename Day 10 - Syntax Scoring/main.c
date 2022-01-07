#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXS 150
#define FN "input.txt"

// ik i should free the memory am i going to? absolutely not

typedef struct chunks_{
     char c;
     struct chunks_ *next;
}chunks_t;

typedef struct lines_{
     char line[MAXS+1];
     struct lines_ *next;
     chunks_t *chunk;
     int scoreIllegalChunk;
}lines_t;
;
int matches(char a, char b);
void printLines(lines_t *h);
lines_t *getLines(char filename[]);
int scoreIllegalChunk(char c);
chunks_t * strToChunk(char str[]);
int corruptedChunk(chunks_t *h, chunks_t *prec);
int isOpening(char c);


int main(){
     int n, i;
     lines_t *h, *curr, *tmp;
     h = NULL;
     h = getLines(FN);
     //printf("%d %d %d %d", '['-']', '{'-'[', '('-')', '<'-'>');
     //printLines(h);
     for(curr=h, i=0;curr;curr=curr->next, i++){
          n = corruptedChunk(curr->chunk, curr->chunk);
          printf("Line %d: Score chunk: %d\n",i , n);
     }
}


int matches(char a, char b){
     return    (a=='{' && b== '}') ||
               (a=='[' && b== ']') ||
               (a=='(' && b== ')') ||
               (a=='<' && b== '>');
}

void printLines(lines_t *h){
     for(h;h;h=h->next){
          printf("%s\n", h->line);
     }
}

lines_t *getLines(char filename[]){
     FILE *fp;
     lines_t *tmp, *h, *prec;
     h = NULL;
     fp = fopen(filename, "r");
     if(fp){
          do{
               tmp = malloc(sizeof(lines_t));
               if(tmp){
                    fscanf(fp, "%s\n", tmp->line);
                    tmp->chunk = strToChunk(tmp->line);
                    tmp->next=NULL;
                    if(!h){
                         h = tmp;
                    }else{
                         for(prec=h;prec->next;prec=prec->next);
                         prec->next = tmp;
                    }
               }else{
                    printf("Problems allocating memory");
               }
          }while(!feof(fp));
     }else{
          printf("Trouble opening file");
     }
     return h;
}


int scoreIllegalChunk(char c){
     int n;
     n=0;
     switch(c){
          case ')': n = 3; break;
          case ']': n = 57; break;
          case '}': n = 1197; break;
          case '>': n = 25137; break;
     }
     return n;
}


chunks_t * strToChunk(char str[]){
     chunks_t *h,*tmp,*prec;
     int i;
     h=NULL;
     for(i=0;str[i]!='\0';i++){
          tmp = malloc(sizeof(chunks_t));
          if(tmp){
               tmp->c = str[i];
               tmp->next = NULL;
               if(!h){
                    h = tmp;
               }else{
                    for(prec =h; prec->next;prec=prec->next);
                    prec->next = tmp;
               }

          }else{
               printf("Problema di allocazione");
          }

     }
     return h;
}


int corruptedChunk(chunks_t *h, chunks_t *prec){
     int n;
     if(h && isOpening(h->c) && h->next && isOpening(h->next->c)){
          printf("Next! %c %c\n", h->c, h->next->c);
          n = corruptedChunk(h->next, h);
          if(!n )
          printf("Now confronting %c %c\n", h->c, h->next->c);
     }

     if(h && h->next && matches(h->c, h->next->c)){
          //free memory
          prec->next = h->next->next;
          printf("heyo closing brackets %c %c\n", h->c, h->next->c);
          return 0;
}
     if(h && h->next && isOpening(h->c) && !isOpening(h->next->c)  && !matches(h->c, h->next->c)){
          printf("Oh no %c %c\n", h->c, h->next->c);
          n = scoreIllegalChunk(h->next->c);
          return n;
     }
}

int isOpening(char c){
     return c=='(' || c== '{' || c=='<' || c=='[';
}
