#include <stdio.h>
#include <stdlib.h>

#define FN "input.txt"
#define DAYS 9

typedef struct{
     long int numberAtDay[DAYS];
     long int totFish;
     //the index is the number of days left for a new lanternfish
     //the number in that position is the number of lanternfish in the same state
}lanternfish_t;

void getInitialLanternFish(char filename[], lanternfish_t *lfs);
void printCurrentLanternFish(lanternfish_t *lfs, int days, int currDay);
void afterOneDay(lanternfish_t *lfs, int days);


int main(){
     int i, day;
     lanternfish_t lfs;

     for(i=0;i<DAYS;i++)
          lfs.numberAtDay[i] = 0;
     lfs.totFish = 0;

     getInitialLanternFish(FN, &lfs);
     for(day = 0; day < 80; day++){
          afterOneDay(&lfs, DAYS);
     }
     printCurrentLanternFish(&lfs, DAYS, 80);

     for(day = 0; day < 256-80; day++){
          afterOneDay(&lfs, DAYS);
     }
     printCurrentLanternFish(&lfs, DAYS, day+80);


}

void getInitialLanternFish(char filename[], lanternfish_t *lfs){
     FILE *fp;
     int day;
     fp = fopen(filename, "r");
     if(fp){
          fscanf(fp, "%d,", &day);
          while(!feof(fp)){
               lfs->numberAtDay[day]++;
               lfs->totFish++;
               fscanf(fp, "%d,", &day);
          }
     }else{
          printf("Could not open the file\n");
     }
}

void printCurrentLanternFish(lanternfish_t *lfs, int days, int currDay){
     int i;
     printf("CurrDay: %d\n", currDay);
     /*for(i=0;i<days;i++){
          printf("%9d ", i);
     }
     printf("\nNumber:");
     for(i=0;i<days;i++){
          printf("%9d ", lfs->numberAtDay[i]);
     }*/
     printf("Total:    %ld", lfs->totFish);
     printf("\n");
}

void afterOneDay(lanternfish_t *lfs, int days){
     long int atDayZero; int i;
     atDayZero = lfs->numberAtDay[0];
     for(i=0;i<days-1;i++){
          lfs->numberAtDay[i] = lfs->numberAtDay[i+1];
     }
     lfs->numberAtDay[6] += atDayZero;
     lfs->numberAtDay[8] = atDayZero;
     for(i=0, lfs->totFish = 0; i<days;i++){
          lfs->totFish += lfs->numberAtDay[i];
     }
}
