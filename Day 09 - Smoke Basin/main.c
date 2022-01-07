#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FN "input.txt"
#define SIZEY 100
#define SIZEX 100
#define SIZETOCHECK 1

typedef struct lowPoints_{
     struct lowPoints_ *next;
     int x;
     int y;
     int basinSize;
}lowPoints_t;

typedef struct basins_ {
     struct basins_ *next;
     int size;
}basins_t;

void getPoints(char filename[], int alreadyChecked[SIZEY][SIZEX], int points[SIZEY][SIZEX], int sizeY, int sizeX);
int risk(int height);
int isLowPoint(int x,int y, int points[SIZEY][SIZEX], int sizeY, int sizeX);
int isInScope(int i,int j, int sizeY, int sizeX);
lowPoints_t * findLowPoints(int points[SIZEY][SIZEX], int sizeY, int sizeX);
int basinSize(int points[SIZEY][SIZEX], int alreadyChecked[SIZEY][SIZEX],  int precPoint, int y, int x, int sizeY, int sizeX);
basins_t * insertBasinsInOrder(basins_t *b, int size);

//i should cleanup the memory ik

int main(){
     int points[SIZEY][SIZEX], alreadyChecked[SIZEY][SIZEX];
     int riskLowPoints, topBasins, i;
     lowPoints_t *h, *curr;
     basins_t *b, *currB;
     b = NULL;
     getPoints(FN, alreadyChecked, points, SIZEY, SIZEX);
     h = findLowPoints(points, SIZEY, SIZEX);
     for(curr=h, riskLowPoints=0;curr;curr=curr->next){
          riskLowPoints += risk(points[curr->y][curr->x]);
          curr->basinSize = basinSize(points, alreadyChecked, -1,curr->y, curr->x, SIZEY, SIZEX);
          b = insertBasinsInOrder(b, curr->basinSize);
     }
     for(i=0, topBasins=1, currB = b; i<3 && currB;i++, currB= currB->next){
          topBasins *= currB->size;
     }
     printf("Total Risk: %d\n", riskLowPoints);
     printf("Higher basins: %d", topBasins);
}

void getPoints(char filename[], int alreadyChecked[SIZEY][SIZEX], int points[][SIZEX], int sizeY, int sizeX){
     FILE *fp;
     int n;
     char line[SIZEX+2];
     int l, i;


     fp = fopen(filename, "r");
     if(fp){
          l = 0;
          fscanf(fp, "%s\n", line);
          while(!feof(fp)){
               for(i=0;i<sizeX;i++){
                    points[l][i] = line[i]-'0';
                    alreadyChecked[l][i] = 0;
               }
               l++;
               fscanf(fp, "%s\n", line);
          }
          for(i=0;i<sizeX;i++){
               points[l][i] = line[i]-'0';
               alreadyChecked[l][i] = 0;

          }
          fclose(fp);
     }else{
          printf("Trouble opening the file");
     }
}


int risk(int height){
     return 1+height;
}

int isLowPoint(int x,int y, int points[SIZEY][SIZEX], int sizeY, int sizeX){
     int i,j, min, isLow;
     for(i=-SIZETOCHECK, min = points[y][x], isLow=1; i<=SIZETOCHECK && isLow; i++){
          for(j=-SIZETOCHECK;j<=SIZETOCHECK && isLow;j++){
               if(isInScope(y+i,x+j, sizeY,sizeX)){
                    if(abs(i)+abs(j)==1){
                         //printf("[%d][%d] -> [%d][%d] %d<%d \n",y,x,i,j, points[y+i][x+j], min);
                         if((points[y+i][x+j]<min)){
                              isLow = 0;
                         }
                    }
               }
          }
     }
     return isLow;
}

int isInScope(int i,int j, int sizeY, int sizeX){
     return i>=0 && i<sizeY && j>=0 && j<sizeX;
}


lowPoints_t * findLowPoints(int points[SIZEY][SIZEX], int sizeY, int sizeX){
     lowPoints_t *h, *tmp, *prec;
     h = NULL;
     int i,j, x,y;
     for(i=0;i<sizeY;i++){
          for(j=0;j<sizeX;j++){
               //printf("Checking: %d %d\n ", j,i);
               if(isLowPoint(j,i,points, sizeY, sizeX) && points[i][j]!=9){
                    tmp = malloc(sizeof(lowPoints_t));
                    if(tmp){
                         tmp->x = j;
                         tmp->y = i;
                         tmp->next = NULL;
                         if(!h){
                              h = tmp;
                         }else{
                              for(prec=h; prec->next; prec = prec->next);
                              prec->next = tmp;
                         }
                    }else{
                         printf("Problems allocating memory");
                    }
               }
          }
     }
     return h;
}

int basinSize(int points[SIZEY][SIZEX], int alreadyChecked[SIZEY][SIZEX],  int precPoint, int y, int x, int sizeY, int sizeX){
     int i,j;
     if(!isInScope(y, x, sizeY, sizeX) || points[y][x]==9 || alreadyChecked[y][x])
          return 0;
     if(precPoint < points[y][x] ){
          alreadyChecked[y][x] = points[y][x];
          /*for(i=0;i<SIZEY;i++){
               for(j=0;j<SIZEX;j++){
                    printf("%2d", alreadyChecked[i][j] );
               }
               printf("\n");
          }
          printf("\n");*/
          return 1 +
               basinSize(points,alreadyChecked, points[y][x], y+1,x, sizeY, sizeX) +
               basinSize(points,alreadyChecked, points[y][x], y-1,x, sizeY, sizeX) +
               basinSize(points,alreadyChecked, points[y][x], y,x+1, sizeY, sizeX) +
               basinSize(points,alreadyChecked, points[y][x], y,x-1, sizeY, sizeX);
     }
     return 0;
}


basins_t * insertBasinsInOrder(basins_t *b, int size){
     basins_t *tmp, *prec, *curr;
     tmp = malloc(sizeof(basins_t));
     tmp->size = size;
     if(tmp){
          tmp->size = size;
          tmp->next = NULL;
          if(b && size < b->size){
               for(prec = b, curr = b->next; curr && size < curr->size;curr = curr->next, prec = prec->next);
               tmp->next = curr;
               prec->next = tmp;
          } else {
               /*caso b vuota o il primo elemento è maggiore del nuovo*/
               tmp->next = b;
               b = tmp;
          }
     }
     return b;
}
