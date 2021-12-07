#include <stdio.h>
#include <stdlib.h>

#define FN "input.txt"
#define RADAR 1000

typedef struct{
     int x,y;
}point_t;


void findVents(char filename[], int seafloor[RADAR][RADAR], int diagonalToo);
void drawLine(int seafloor[RADAR][RADAR], point_t a, point_t b, int diagonalToo);
int findDangerousSpots(int seafloor[RADAR][RADAR]);
void normalizePoints(point_t *a, point_t *b);

int main(){
     int seafloor[RADAR][RADAR];
     int spots;
     findVents(FN, seafloor, 0);
     spots = findDangerousSpots(seafloor);
     printf("Dangerous spots: %d\n", spots);

     findVents(FN, seafloor, 1);
     spots = findDangerousSpots(seafloor);
     printf("Dangerous spots: %d\n", spots);
}


void findVents(char filename[], int seafloor[RADAR][RADAR], int diagonalToo){
     FILE *fp;
     int i, j;
     point_t a,b;

     for(i=0;i<RADAR;i++)
          for(j=0;j<RADAR;j++)
               seafloor[i][j] = 0;

     fp = fopen(filename, "r");
     if(fp){
          fscanf(fp, "%d,%d -> %d,%d", &(a.x), &(a.y), &(b.x), &(b.y));
          while(!feof(fp)){
               //printf("(%d , %d) -> (%d , %d)\n", (a.x), (a.y), (b.x), (b.y));
               drawLine(seafloor, a, b, diagonalToo);
               /*
               for(i=0;i<RADAR;i++){
                    for(j=0;j<RADAR;j++){
                         printf("%2d", seafloor[i][j]);
                    }
                    printf("\n");
               }
               printf("\n\n");*/

               fscanf(fp, "%d,%d -> %d,%d", &(a.x), &(a.y), &(b.x), &(b.y));
          }
     }else{
          printf("Could not open the file\n");
     }




}

void drawLine(int seafloor[RADAR][RADAR], point_t a, point_t b, int diagonalToo){
     int i, j, pointsDrawn;
     pointsDrawn =0;
     int dirX, dirY;

     dirX = (a.x < b.x) ? 1:-1;
     dirY = (a.y < b.y) ? 1:-1;

     if(a.x == b.x || a.y==b.y){
          for(i=0 ; i<= dirY * (b.y -a.y) && (a.y==b.y || a.x==b.x); i++){
               for(j=0; j<= dirX * (b.x - a.x); j++){
                    seafloor[a.y + dirY*i][a.x + dirX*j]++; pointsDrawn++;
               }
          }
     }else if(diagonalToo){
          for(i=0, j=0; (i<= dirY * (b.y -a.y)) && (j<= dirX * (b.x - a.x)); i++, j++){
               seafloor[a.y + dirY*i][a.x + dirX*j]++; pointsDrawn++;
          }
     }


}

int findDangerousSpots(int seafloor[RADAR][RADAR]){
     int i, j, spots;
     for(i=0, spots=0; i<RADAR; i++){
          for(j=0; j<RADAR; j++){
               if(seafloor[i][j]>=2){
                    spots++;
               }
          }
     }
     return spots;
}
