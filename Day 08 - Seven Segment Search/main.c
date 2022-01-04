#include <stdio.h>
#include <stdlib.h>

#define DISP 7
#define SIGPATTERNS 10
#define OUTVALUES 4

#define FN "input.txt"

typedef struct {
     char sigpatterns[SIGPATTERNS][DISP+1];
     char output[OUTVALUES][DISP+1];
     char map;
}entry_t;

typedef struct display_{
     entry_t entry;
     struct display_ *next;
}display_t;

display_t * getInput(char filename[], int sigpatterns, int outvalues);
void displayInput(display_t *entry,  int sigpatterns, int outvalues);
void *destroyEntries(display_t *entry);
void compileMap(char map[], char patterns[SIGPATTERNS][DISP+1], int sigpatterns);
int computenumber(display_t *entry, char output[], char map[], int disp, int sigpatterns);
int isInPattern(char c, char str[]);
int foundInSignalP(char c, char patterns[SIGPATTERNS][DISP+1], int sigpatterns);
int sumLenPatternsWhichContain(char c, char patterns[SIGPATTERNS][DISP+1], int sigpatterns);
int sameSignals(char sigpattern[], char output[]);

int main(){

     display_t *entries, *curr;
     char map[DISP+1];
     int i, cont, n;
     int alloutValues;
     char out[DISP+1];

     entries = getInput(FN, SIGPATTERNS, OUTVALUES);
     //displayInput(entries,  SIGPATTERNS, OUTVALUES);
     for(curr = entries, cont=0, alloutValues=0; curr; curr=curr->next){
          compileMap(map, curr->entry.sigpatterns, SIGPATTERNS);
          for(i=0;i<OUTVALUES;i++){
               n = computenumber(curr, curr->entry.output[i], map, DISP, SIGPATTERNS);
               if(n == 7 || n==1 || n==8 || n==4)
                    cont++;
               out[i] = n+'0';
          }
          out[i] = '\0';
          alloutValues += atoi(out);
     }
     printf("Cont: %d\n", cont);
     printf("AlloutValues: %d\n", alloutValues);

     entries = destroyEntries(entries);
}


//Not my cleanest code ik lol

display_t * getInput(char filename[], int sigpatterns, int outvalues){
     FILE *fp;
     display_t *h, *tmp, *prec;
     int i;
     h = NULL;
     fp = fopen(filename ,"r");
     if(fp){
          do{
               tmp = malloc(sizeof(display_t));
               if(tmp){
                    for(i=0;i<sigpatterns;i++){
                         fscanf(fp, "%s ", (tmp->entry.sigpatterns[i]));
                    }
                    fscanf(fp, "| ");
                    for(i=0;i<outvalues;i++){
                         fscanf(fp, "%s ", (tmp->entry.output[i]));
                    }
                    tmp->next = NULL;
                    if(!h){
                         h = tmp;
                    }else{
                         for(prec = h; prec->next; prec=prec->next);
                         prec->next = tmp;
                    }
                    fscanf(fp, "\n");
               }else{
                    printf("Problemi di allocazione di memoria");
               }
          }while(!feof(fp));
          fclose(fp);
     }else{
          printf("Trouble opening the file");
     }
     return h;
}

void displayInput(display_t *entry,  int sigpatterns, int outvalues){
     int i;
     for(entry; entry; entry=entry->next){
          for(i=0;i<sigpatterns;i++){
               printf("%s ", entry->entry.sigpatterns[i]);
          }
          printf("| ");
          for(i=0;i<outvalues;i++){
               printf("%s ", entry->entry.output[i]);
          }
          printf("\n");
     }
}

void *destroyEntries(display_t *entry){
     display_t *tmp;
     while(entry){
          tmp = entry;
          entry = entry->next;
          free(tmp);
     }
}

void compileMap(char map[], char patterns[SIGPATTERNS][DISP+1], int sigpatterns){
     char c; int i; int n;
     for(c='a';c<='g';c++){
          n = foundInSignalP(c, patterns, sigpatterns);
          switch (n) {
               case 4: map['e'-'a'] = c; break;
               case 6: map['b'-'a'] = c; break;
               case 9: map['f'-'a'] = c; break;
               case 8:
                    if(sumLenPatternsWhichContain(c, patterns, sigpatterns) == 43)
                         map['a'-'a'] = c;
                    else
                         map['c'-'a'] = c;
                    break;
               case 7:
                    if(sumLenPatternsWhichContain(c, patterns, sigpatterns) == 40)
                         map['g'-'a'] = c;
                    else
                         map['d'-'a'] = c;
                    break;
           }
     }
}

int isInPattern(char c, char str[]){
     int found, len;
     for(found=0, len =0; str[len]!='\0' && !found; len++)
          if(str[len]==c){
               found=1;
          }
     return found;
}

int foundInSignalP(char c, char patterns[SIGPATTERNS][DISP+1], int sigpatterns){
     int i, found;
     for(i=0, found=0;i<sigpatterns;i++){
          if(isInPattern(c, patterns[i]))
               found++;
     }
     return found;
}

int sumLenPatternsWhichContain(char c, char patterns[SIGPATTERNS][DISP+1], int sigpatterns){
     int i, len, sumLen, found;
     for(i=0, sumLen=0, found=0; i<sigpatterns;i++){
          if(isInPattern(c, patterns[i])){
               found++;
               for(len=0; patterns[i][len]!='\0';len++);
               sumLen += len;
          }
     }
     return sumLen;
}

//same letters
int sameSignals(char sigpattern[], char output[]){
     int i,j, f1, f2, ok;
     for(i=0, ok=1;output[i]!='\0' && ok;i++){
          for(j=0, f1=0, f2=0; output[j]!='\0' && sigpattern[j]!='\0';j++){
               if(output[i]==output[j])
                    f1++;
               if(output[i] == sigpattern[j])
                    f2++;
          }
          ok = output[j]=='\0' && sigpattern[j]=='\0' && f1 == f2;
     }
     return ok;
}

int computenumber(display_t *entry, char output[], char map[], int disp, int sigpatterns){
     int activatedSegments[DISP], len, i, j, w, num;
     for(i=0;i<disp;i++)
          activatedSegments[i]=0;

     for(i=0;i<sigpatterns;i++){
          if(sameSignals(entry->entry.sigpatterns[i], output)){
               for(j=0;output[j]!='\0';j++){
                    for(w=0;w<disp;w++){
                         if(output[j]==map[w])
                         activatedSegments[w] = 1;

                    }
               }

          }
     }

     if(activatedSegments[0] && activatedSegments[1] && activatedSegments[2] && !activatedSegments[3] && activatedSegments[4] && activatedSegments[5] && activatedSegments[6])
          num = 0;
     if(!activatedSegments[0] && !activatedSegments[1] && activatedSegments[2] && !activatedSegments[3] && !activatedSegments[4] && activatedSegments[5] && !activatedSegments[6])
          num = 1;
     if(activatedSegments[0] && !activatedSegments[1] && activatedSegments[2] && activatedSegments[3] && activatedSegments[4] && !activatedSegments[5] && activatedSegments[6])
          num = 2;
     if(activatedSegments[0] && !activatedSegments[1] && activatedSegments[2] && activatedSegments[3] && !activatedSegments[4] && activatedSegments[5] && activatedSegments[6])
          num = 3;
     if(!activatedSegments[0] && activatedSegments[1] && activatedSegments[2] && activatedSegments[3] && !activatedSegments[4] && activatedSegments[5] && !activatedSegments[6])
          num = 4;
     if(activatedSegments[0] && activatedSegments[1] && !activatedSegments[2] && activatedSegments[3] && !activatedSegments[4] && activatedSegments[5] && activatedSegments[6])
          num = 5;
     if(activatedSegments[0] && activatedSegments[1] && !activatedSegments[2] && activatedSegments[3] && activatedSegments[4] && activatedSegments[5] && activatedSegments[6])
          num = 6;
     if(activatedSegments[0] && !activatedSegments[1] && activatedSegments[2] && !activatedSegments[3] && !activatedSegments[4] && activatedSegments[5] && !activatedSegments[6])
          num = 7;
     if(activatedSegments[0] && activatedSegments[1] && activatedSegments[2] && activatedSegments[3] && activatedSegments[4] && activatedSegments[5] && activatedSegments[6])
          num = 8;
     if(activatedSegments[0] && activatedSegments[1] && activatedSegments[2] && activatedSegments[3] && !activatedSegments[4] && activatedSegments[5] && activatedSegments[6])
          num = 9;


     return num;
}

/*

*/
