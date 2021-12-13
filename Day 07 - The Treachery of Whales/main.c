#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FN "input.txt"

typedef struct pos_{
     int h;
     struct pos_ *next;
}pos_t;

pos_t  *getInitialPositions(char filename[]);
int getTotalFuelForPosition(pos_t *crabs, int h, int linear);
int getNonLinearFuel(int distance);
int getMinimalFuel(pos_t *crabs, int linear);

int main(){
     pos_t *crabs;
     int minFuel;
     crabs = getInitialPositions(FN);

     minFuel = getMinimalFuel(crabs,1);
     printf("MinFuel with Linear consumption: %d\n", minFuel);

     minFuel = getMinimalFuel(crabs,0);
     printf("MinFuel with NonLinear consumption: %d\n", minFuel);

}


int getMinimalFuel(pos_t *crabs, int linear){
     pos_t *curr;
     int fuelAtCurrCrab, minFuel;
     minFuel = getTotalFuelForPosition(crabs, crabs->h, linear);
     for(curr=crabs->next; curr;curr=curr->next){
          fuelAtCurrCrab = getTotalFuelForPosition(crabs, curr->h, 0);
          if(fuelAtCurrCrab<minFuel)
               minFuel = fuelAtCurrCrab;
     }
     return minFuel;
}

int getTotalFuelForPosition(pos_t *crabs, int h, int linear){
     int totFuel;
     for(totFuel = 0;crabs; crabs = crabs->next){
          if(linear)
               totFuel += abs(crabs->h - h);
          else{
               totFuel += getNonLinearFuel(abs(crabs->h - h -1));
          }
     }
     return totFuel;
}

int getNonLinearFuel(int distance){
     int step, fuel;
     for(step = 1, fuel =0; distance; step++, distance--)
     fuel += step;
     return fuel;
}

pos_t  *getInitialPositions(char filename[]){
     pos_t *crabs, *tmp;
     FILE *fp;
     int n;
     crabs = NULL;
     fp = fopen(filename, "r");
     if(fp){
          fscanf(fp, "%d,", &n);
          while(!feof(fp)){
               tmp = malloc(sizeof(pos_t));
               tmp->h = n;
               if(tmp){
                    if(!crabs){
                         tmp->next = NULL;
                         crabs = tmp;
                    }else{
                         tmp->next = crabs;
                         crabs = tmp;

                    }
               }else{
                    printf("Could not allocate memory");
               }
               fscanf(fp, "%d,", &n);
          }
          tmp = malloc(sizeof(pos_t));
          if(tmp){
               tmp->h = n;
               tmp->next = crabs;
               crabs = tmp->next;
          }
          fclose(fp);
     }else{
          printf("Could not open the file");
     }
     return crabs;
}
