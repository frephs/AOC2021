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

typedef struct exp_scores_{
     long int score;
     struct exp_scores_ *next;
}exp_scores_t;

typedef struct lines_{
     char line[MAXS+1];
     struct lines_ *next;
     chunks_t *chunk;
     int scoreIllegalChunk;
}lines_t;


int matches(chunks_t *a, chunks_t *b);
void printLines(lines_t *h);
lines_t *getLines(char filename[]);
int scoreCorruptedChunk(char c);
long int scoreIncompleteChunk(char c);
chunks_t * strToChunk(char str[]);
chunks_t *illegalChunk(chunks_t *h, chunks_t *prec, int incomplete);
int isOpening(chunks_t *c);
exp_scores_t * insertExpectedScoresInOrder(exp_scores_t *e, long int score);
void printExpectedScores(exp_scores_t *exp);


int main(){
     int scoreCorrupted, i,j;
     long int scoreIncomplete;
     lines_t *h, *curr, *tmp;
     exp_scores_t *exps;
     chunks_t *curr2, *tmp2;

     exps = NULL;
     h = NULL; h = getLines(FN);

     for(curr=h, i=0, scoreCorrupted=0;curr;curr=curr->next){
          tmp2 = illegalChunk(curr->chunk, curr->chunk, 0);
          if(tmp2){
               scoreCorrupted += scoreCorruptedChunk(tmp2->c);
          }
          curr2= illegalChunk(curr->chunk, curr->chunk, 1);
          //printExpected(curr2);
          for(scoreIncomplete=0; curr2; curr2=curr2->next){
               scoreIncomplete = scoreIncomplete*5 + scoreIncompleteChunk(curr2->c);
          }
          //printf("Score incomplete: %ld\n",scoreIncomplete);
          if(scoreIncomplete){
               exps = insertExpectedScoresInOrder(exps, scoreIncomplete);
               i++;
          }
     }

     printf("Score chunk total %d: %d\n",j, scoreCorrupted);
     for(exps, j=0; j<i/2 && exps; exps=exps->next, j++);
     printf("Middle score for autocompletation: %ld\n", exps->score);
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

int matches(chunks_t *a, chunks_t *b){
     return    (a->c=='{' && b->c== '}') ||
     (a->c=='[' && b->c== ']') ||
     (a->c=='(' && b->c== ')') ||
     (a->c=='<' && b->c== '>');
}

char returnMatchingChar(chunks_t *a){
     if (a->c=='{')
     return  '}';
     if (a->c=='[')
     return  ']';
     if (a->c=='(')
     return  ')';
     if (a->c=='<')
     return  '>';
}

int scoreCorruptedChunk(char c){
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

long int scoreIncompleteChunk(char c){
     int n;
     n=0;
     switch(c){
          case ')': n = 1; break;
          case ']': n = 2; break;
          case '}': n = 3; break;
          case '>': n = 4; break;
     }
     return n;
}

void printLines(lines_t *h){
     for(h;h;h=h->next){
          printf("%s\n", h->line);
     }
}

int isOpening(chunks_t *c){
     return c->c=='(' || c->c== '{' || c->c=='<' || c->c=='[';
}

chunks_t * expected (chunks_t *expected, chunks_t *a){
     chunks_t *tmp;
     tmp = malloc(sizeof(chunks_t));
     if(tmp){
          tmp->c = returnMatchingChar(a);
          tmp->next = expected;
          expected = tmp;
     }else{
          printf("trouble allocating memory");
     }
     return expected;
}

void printExpected(chunks_t *exp){
     printf("Expected: ");
     for(exp;exp;exp=exp->next)
     printf("%c ", exp->c);
     printf("\n");

}

void printExpectedScores(exp_scores_t *exp){
     for(exp;exp;exp=exp->next)
     printf("%ld ", exp->score);
     printf("\n");

}

chunks_t *illegalChunk(chunks_t *h, chunks_t *prec, int incomplete){
     chunks_t *exp;
     exp = NULL;
     int ok;
     ok = 1;
     do{
          if(h && !isOpening(h)){
               ok = h->c == exp->c;
               //printf("Closing: %c, ", h->c);printExpected(exp);
               if(ok){
                    exp = exp->next;
               }
          }else if(h){
               exp = expected(exp, h);
               //printf("Adding: %c, ", h->c);printExpected(exp);
          }
          prec = h;
          h= h->next;
     }while(h && ok);
     if(!ok && !incomplete)
     return(prec);
     if(ok && incomplete)
     return exp;
     return 0;
}

exp_scores_t * insertExpectedScoresInOrder(exp_scores_t *e, long int score){
     exp_scores_t *tmp, *prec, *curr;
     tmp = malloc(sizeof(exp_scores_t));
     tmp->score = score;
     if(tmp){
          tmp->score = score;
          tmp->next = NULL;
          if(e && score < e->score){
               for(prec = e, curr = e->next; curr && score < curr->score;curr = curr->next, prec = prec->next);
               tmp->next = curr;
               prec->next = tmp;
          } else {
               /*caso e vuota o il primo elemento è maggiore del nuovo*/
               tmp->next = e;
               e = tmp;
          }
     }
     return e;
}
