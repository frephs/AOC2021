#include <stdio.h>
#define FN "input.txt"
#define MAXCHAREXT 9+2*90+100
#define NROWS 5
#define NCOL 5

#define FN "input.txt"

typedef struct bingo_{
     int card[NROWS][NCOL];
     int won;
     struct bingo_ *next;
}bingo_t;

typedef struct extractions_{
     int ext;
     struct extractions_ *next;
}extractions_t;

//Lists are redundant but were implemented to exercise for my FDI exam

int main(){
     extractions_t *hExt;
     getExtractions(FN, *hExt);

}

FILE *getExtractions(char filename[], extractions_t *hExt){
     FILE *fp;
     char exts[MAXCHAREXT];
     int n, pow;
     extractions_t *tmp, *curr;
     hExt = NULL;
     fp = fopen(filename, "r");
     if(fp){
          fgets(fp, MAXCHAREXT, exts);
          for(len=0; exts[len]!='\0';len++);
          for(i=len-1, n=0, pow=1; i>=0; i--){
               if(exts[i]!=','){
                    n+= (exts[i] - '0')*pow;
                    pow *= 10;
               }
               else{
                    pow = 1;
                    tmp = malloc(sizeof(extractions_t));
                    if(tmp){
                         tmp->ext = n;
                         if(!hExt){
                              hExt=tmp;
                              hExt->next = NULL;
                         }else{
                              for(curr = *hExt;curr->next;curr=curr->next);
                              curr->next = tmp;
                              tmp->next = NULL;
                         }
                    }else{
                         printf("Could not allocate memory\n");
                    }
                    n = 0;
               }

          }
          for(curr=hExt;curr;curr=curr->next){
               printf("%d -> ", curr->ext);
          }
     }else{
          printf("Could not open the file")
     }
     return fp;
}
