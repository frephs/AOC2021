#include <stdio.h>
#include <stdlib.h>
#define FN "input.txt"

#define OCTOGRID 10
#define STEPS 100

typedef struct {
     int energy_level;
     int hasFlashedThisStep;
}octopus_t;

void get_octos(char filename[], octopus_t o[OCTOGRID][OCTOGRID], int nr, int nc);
int one_step(octopus_t o[OCTOGRID][OCTOGRID], int nr, int nc, int radius, int *flashedAll);
void first_things_first(octopus_t o[OCTOGRID][OCTOGRID], int nr, int nc);
int last_things_last(octopus_t o[OCTOGRID][OCTOGRID], int nr, int nc);
void check_if_flashes(octopus_t o[OCTOGRID][OCTOGRID], int nr, int nc, int radius, int i, int j);
void flash(octopus_t o[OCTOGRID][OCTOGRID], int nr, int nc, int radius, int i, int j);
int isInScope(int x, int y, int nr, int nc);
void print_octos(octopus_t o[OCTOGRID][OCTOGRID], int nr, int nc);
int flashedAllTogether(octopus_t o[OCTOGRID][OCTOGRID], int nr, int nc);

int main(){
     octopus_t o[OCTOGRID][OCTOGRID];
     int step, totalFlashes, flashedAllTogether, alreadyFlashedtogether;
     get_octos(FN, o, OCTOGRID, OCTOGRID);
     for(step=0, totalFlashes=0, alreadyFlashedtogether=0, flashedAllTogether=0; step<STEPS;step++){
          totalFlashes += one_step(o, OCTOGRID, OCTOGRID, 1, &flashedAllTogether);
          //print_octos(o,OCTOGRID,OCTOGRID);
     }
     printf("Part 1: Total flashes: %d\n", totalFlashes);
     get_octos(FN, o, OCTOGRID, OCTOGRID);
     for(step=1, totalFlashes=0, alreadyFlashedtogether=0, flashedAllTogether=0; !alreadyFlashedtogether;step++){
          totalFlashes += one_step(o, OCTOGRID, OCTOGRID, 1, &flashedAllTogether);
          //print_octos(o,OCTOGRID,OCTOGRID);
          if(!alreadyFlashedtogether && flashedAllTogether){
               printf("Part 2: Flashed all together at step %d\n", step);
               //print_octos(o, OCTOGRID, OCTOGRID);
               alreadyFlashedtogether=1;
          }
     }
}

void get_octos(char filename[], octopus_t o[OCTOGRID][OCTOGRID], int nr, int nc){
     int i,j; char e;
     FILE *fp;
     fp = fopen(filename, "r");
     if(fp){
          for(i=0;i<nr;i++){
               for(j=0;j<nc;j++){
                    fscanf(fp, "%c", &e);
                    o[i][j].energy_level = e- '0';
                    o[i][j].hasFlashedThisStep=0;
               }
               fscanf(fp, " \n");
          }
     }else{
          printf("Could not open the file ");
     }
}

int one_step(octopus_t o[OCTOGRID][OCTOGRID], int nr, int nc, int radius, int *flashedAll){
     int i,j, flashes;
     first_things_first(o, nr,nc);
     for(i=0;i<nr;i++){
          for(j=0;j<nc;j++){
               check_if_flashes(o, nr, nc, radius, i, j);
          }
     }
     if(flashedAllTogether(o,nr,nc)){
          *flashedAll=1;
     }
     flashes = last_things_last(o, nr, nc);
     return flashes;
}

void first_things_first(octopus_t o[OCTOGRID][OCTOGRID], int nr, int nc){
     int i, j;
     for(i=0;i<nr;i++){
          for(j=0;j<nc;j++)
               o[i][j].energy_level++;
     }
}

int last_things_last(octopus_t o[OCTOGRID][OCTOGRID], int nr, int nc){
     int i, j, flashes;
     for(i=0, flashes=0;i<nr;i++){
          for(j=0;j<nc;j++)
               if(o[i][j].hasFlashedThisStep){
                    o[i][j].hasFlashedThisStep=0;
                    o[i][j].energy_level=0;
                    flashes++;
               }
     }
     return flashes;
}

void check_if_flashes(octopus_t o[OCTOGRID][OCTOGRID], int nr, int nc, int radius, int i, int j){
     if(!(o[i][j].hasFlashedThisStep) && o[i][j].energy_level>9){
          o[i][j].hasFlashedThisStep=1;
          flash(o,nr,nc,radius,i,j);
     }
}

void flash(octopus_t o[OCTOGRID][OCTOGRID], int nr, int nc, int radius, int i, int j){
     int x,y;
     o[i][j].hasFlashedThisStep=1;
     for(y=-radius;y<=radius;y++){
          for(x=-radius;x<=radius;x++){
               if(isInScope(i+y,x+j, nr, nc)){
                    o[i+y][j+x].energy_level++;
                    check_if_flashes(o,nr,nc,radius,i+y,j+x);
               }
          }
     }
}

int isInScope(int x, int y, int nr, int nc){
     return x>=0 && y>=0 && x<nc && y<nr;
}


void print_octos(octopus_t o[OCTOGRID][OCTOGRID], int nr, int nc){
     int i,j;
     for(i=0;i<nr;i++){
          for(j=0;j<nc;j++)
               fprintf(stderr, "%d", o[i][j].energy_level);
               fprintf(stderr, "\n");
     }
     fprintf(stderr, "\n");
}

int flashedAllTogether(octopus_t o[OCTOGRID][OCTOGRID], int nr, int nc){
     int flashingAllTogether, i,j;
     for(i=0, flashingAllTogether=1; i<nr && flashingAllTogether; i++){
          for(j=0;j<nc && flashingAllTogether;j++){
               flashingAllTogether= o[i][j].hasFlashedThisStep ==1;
          }
     }
     return flashingAllTogether;
}
