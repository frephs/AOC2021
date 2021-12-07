#include <stdio.h>
#include <stdlib.h>

#define FN "input.txt"
#define MAXCHAREXT 289
#define NROWS 5
#define NCOLS 5


typedef struct bingo_{
     int card[NROWS][NCOLS];
     int won;
     struct bingo_ *next;
}bingo_t;

typedef struct extractions_{
     int ext;
     struct extractions_ *next;
}extractions_t;


FILE *getExtractions(char filename[], extractions_t **hExt);
bingo_t *getBingoCards(FILE *fp, int nrows, int ncols);

void printBingoCards(bingo_t *hCards,  int nrows, int ncols);
void printExtractions(extractions_t *ext);

int won(bingo_t **currcard, int nrows, int ncols);
void extractNumbers(extractions_t *ext, bingo_t *hCards, int nrows, int ncols);
int calcScore(bingo_t *curr, extractions_t *ext, int nrows, int ncols);

void checkFirstWinning(extractions_t *ext, bingo_t *hCards, int nrows, int ncols, int *score);
int checkLastWinning(extractions_t *ext, bingo_t *hCards, int nrows, int ncols);




bingo_t *duplicateCards(bingo_t *hCards, bingo_t *duplicate);

int main(){
     extractions_t *hExt, *currExt;
     bingo_t *hCards, *firstCheck, *secondCheck, *minWin, *currCard;
     FILE *fp;
     int score, allWon;

     fp = getExtractions(FN, &hExt);
     hCards = getBingoCards(fp, NROWS, NCOLS);

     //printBingoCards(hCards, NROWS, NCOLS);
     //removing the first blank card

     hCards = hCards->next;
     //printExtractions(hExt);

     //Duplicating the do the first extraction
     firstCheck = duplicateCards(hCards, firstCheck);
     for(currExt = hExt, score=0; currExt && !score; currExt = currExt->next){
          extractNumbers(currExt, firstCheck, NROWS, NCOLS);
          checkFirstWinning(currExt, firstCheck, NROWS, NCOLS, &score);
     }
     printf("Score: %d\n", score);

     secondCheck = duplicateCards(hCards, secondCheck);

     for(currExt = hExt, allWon =0 ; currExt && !allWon; currExt= currExt->next){
          extractNumbers(currExt, secondCheck, NROWS, NCOLS);
          allWon = checkLastWinning(currExt, secondCheck, NROWS, NCOLS);
          if(allWon){
               for(currCard = secondCheck, minWin= currCard; currCard; currCard = currCard->next){
                    if(currCard->won<=minWin->won){
                         minWin = currCard;
                    }
               }
               score = calcScore(minWin,currExt, NROWS,NCOLS);
          }
     }
     printf("Score: %d\n", score);

}

FILE *getExtractions(char filename[], extractions_t **hExt){
     FILE *fp;
     char exts[MAXCHAREXT+1];
     int n, pow, len, i;
     extractions_t *tmp, *curr;
     *hExt = NULL;
     fp = fopen(filename, "r");
     if(fp){
          fgets(exts, MAXCHAREXT+1, fp);
          for(len=0; exts[len]!='\0' && exts[len]!=' ' && exts[len]!=13 && exts[len]!=10 && exts[len]!='\n';len++);
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
                         if(!(*hExt)){
                              *hExt = tmp;
                              (*hExt)->next = NULL;
                         }else{
                              tmp->next = *hExt;
                              *hExt = tmp;
                         }
                    }else{
                         printf("Could not allocate memory\n");
                    }
                    n = 0;
               }

          }
          tmp = malloc(sizeof(extractions_t));
          if(tmp){
               tmp->ext = n;
               tmp->next = *hExt;
               *hExt = tmp;
          }

     }else{
          printf("Could not open the file");
     }
     return fp;
}

bingo_t *getBingoCards(FILE *fp, int nrows, int ncols){
     bingo_t *hCards, *tmp;
     int i, j, k=0;
     hCards = NULL;

     if(fp){
          //assuming all bingo cards in the input file are valid
          while(!feof(fp)){
               tmp = malloc(sizeof(bingo_t));
               tmp->won = 0;
               if(tmp){
                    for(i=0;i<nrows;i++){
                         for(j=0;j<ncols;j++){
                              fscanf(fp, "%d", &(tmp->card)[i][j]);
                         }
                    }
                    if(!hCards){
                         hCards = tmp;
                         hCards->next = NULL;
                    }else{
                         tmp->next = hCards;
                         hCards= tmp;
                    }
               }else{
                    printf("Could not allocate memory");
               }
          }
     }
     fclose(fp);
     return hCards;
}


void printBingoCards(bingo_t *hCards, int nrows, int ncols){
     int i,j;
     bingo_t *curr;
     for(curr = hCards; curr; curr=curr->next){
          for(i=0;i<nrows;i++){
               for(j=0;j<ncols;j++){
                    printf("%2d ", (curr->card)[i][j]);
               }
               printf("\n");
          }
          printf("Wins: %d\n", curr->won);
     }
}

void printExtractions(extractions_t *ext){
     for(ext; ext; ext=ext->next){
          printf("%d -> ", ext->ext);
     }
     printf("\n");
}


int won(bingo_t **currcard, int nrows, int ncols){
     int i, j, count;
     for(i=0;i<nrows && !((*currcard)->won);i++){
          for(j=0, count=0;j<ncols;j++){
               if((*currcard)->card[i][j] >= 100 )
                    count++;
          }
          if(count==ncols)
               (*currcard)->won = 1;
     }
     for(i=0;i<ncols && !((*currcard)->won);i++){
          for(j=0, count=0;j<nrows;j++){
               if((*currcard)->card[j][i] >=100 ){
                    count++;

               }
          }
          if(count==ncols)
               (*currcard)->won = 1;
     }
     return (*currcard)->won;
}


void extractNumbers(extractions_t *ext, bingo_t *hCards, int nrows, int ncols){
     bingo_t * curr;
     int i, j;
     for(curr=hCards; curr; curr = curr->next){
          for(i=0;i<nrows;i++){
               for(j=0;j<ncols;j++){
                    if(curr->card[i][j]==ext->ext){
                         curr->card[i][j] +=100;
                    }
               }
          }

     }
}
void checkFirstWinning(extractions_t *ext, bingo_t *hCards, int nrows, int ncols, int *score){
     bingo_t *curr;
     int i, j, foundWinner;
     for(curr=hCards, foundWinner=0; curr && !foundWinner; curr = curr->next){
          if(won(&curr, nrows, ncols)){
               foundWinner=1;
               *score = calcScore(curr, ext, nrows, ncols);
          }
     }
}


bingo_t *duplicateCards(bingo_t *hCards, bingo_t *duplicate){
     bingo_t *currCard, *tmp;
     duplicate = NULL;
     for(currCard = hCards; currCard; currCard = currCard->next){
          tmp = malloc(sizeof(bingo_t));
          if(tmp){
               *tmp = *currCard; //copying all the struct
               if(!duplicate){
                    duplicate = tmp;
                    duplicate->next = NULL;
               }else{
                    tmp->next = duplicate;
                    duplicate = tmp;
               }
          }else{
               printf("COuld not allocate memory");
          }
     }
     return duplicate;
}

int checkLastWinning(extractions_t *ext, bingo_t *hCards, int nrows, int ncols){
     bingo_t *curr;
     int i, j, allWon;
     for(curr=hCards, allWon=1; curr ; curr = curr->next){
          if(won(&curr, nrows, ncols)){
               (curr->won)++;
          }else{
               allWon = 0;
          }
     }
     return allWon;
}

int calcScore(bingo_t *curr, extractions_t *ext, int nrows, int ncols){
     int i, j, score;

     for(i=0, score=0; i<nrows; i++){
          for(j=0;j<ncols;j++){
               if(curr->card[i][j]<100)
               score += curr->card[i][j];
          }
     }
     score *= ext->ext;
     return score;
}
