#include <stdio.h>
#include <string.h>

#define FN "input.txt"
#define MAXS 7

int getFinalPosition(char filename[]);
int getCorrectFinalPosition(char filename[]);


int main(){
     int finalPos;
     finalPos = getFinalPosition(FN);
     printf("%d\n", finalPos);
     finalPos = getCorrectFinalPosition(FN);
     printf("%d\n", finalPos);
}

int getFinalPosition(char filename[]){
     FILE *fp;
     int x_pos, y_depth, n;
     char direction[MAXS+1];
     fp = fopen(filename, "r");
     x_pos = y_depth = 0;
     if(fp){
          fscanf(fp, "%s %d", direction, &n);
          while(!feof(fp)){
               if(!strcmp(direction, "forward")){
                    x_pos += n;
               }else if(!strcmp(direction, "down")){
                    y_depth += n;
               }else{
                    y_depth -= n;
                    if(y_depth<0)
                         printf("Sotto 0\n");
               }
               fscanf(fp, "%s %d", direction, &n);
          }
     }else{
          printf("Could not open the input file");
          x_pos = y_depth = 0;
     }
     return x_pos*y_depth;
}

int getCorrectFinalPosition(char filename[]){
     FILE *fp;
     int x_pos, y_depth, aim, n;
     char direction[MAXS+1];
     fp = fopen(filename, "r");
     x_pos = y_depth = aim = 0;
     if(fp){
          fscanf(fp, "%s %d", direction, &n);
          while(!feof(fp)){
               if(!strcmp(direction, "forward")){
                    x_pos += n;
                    y_depth += aim*n;
                    if(y_depth<0)
                         printf("Sotto 0\n");
               }else if(!strcmp(direction, "down")){
                    aim += n;
               }else{
                    aim -= n;
               }
               fscanf(fp, "%s %d", direction, &n);
          }
     }else{
          printf("Could not open the input file");
          x_pos = y_depth = 0;
     }
     return x_pos*y_depth;
}
