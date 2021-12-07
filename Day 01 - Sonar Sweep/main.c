#include <stdio.h>
#include <stdlib.h>

#define FN "input.txt"

int countIncreases(char filename[]);
int countIncreasesSlidingWindow(char filename[], int size);

int main(){
    int increases;
    increases = countIncreases(FN);
    printf("Increases: %d\n", increases);
    increases = countIncreasesSlidingWindow(FN, 3);
    printf("Increases: %d\n", increases);

}

int countIncreases(char filename[]){
    FILE *fp;
    int currDepth, prevDepth, increases;
    fp = fopen(filename, "r");
    if(fp){
        increases = 0;
        fscanf(fp, "%d %d", &prevDepth, &currDepth);
        while(!feof(fp)){
            if(currDepth>prevDepth)
                increases++;
            prevDepth = currDepth;
            fscanf(fp, "%d", &currDepth);
        }
    }else{
        printf("Could not open the file");
        increases = -1;
    }
    return increases;
}

int countIncreasesSlidingWindow(char filename[], int size){
    FILE *fp;
    int *sw;
    int i, prevSum, currSum, increases;

    sw = malloc(sizeof(int)*size);
    if(sw){
         fp = fopen(filename, "r");
         if(fp){
              increases = 0;
              for(i=0;i<size && !feof(fp); i++)
                   fscanf(fp, "%d", sw+i);
              for(i=0, prevSum=0;i<size;i++)
                   prevSum += *(sw+i);

              while(!feof(fp)){
                   for(i=0, currSum=0;i<size;i++)
                        currSum += *(sw+i);
                   if(currSum>prevSum)
                        increases++;
                   for(i=0; i<size-1;i++)
                        *(sw+i) = *(sw+i+1);
                   prevSum = currSum;
                   fscanf(fp, "%d", sw+size-1);
              }
              free(sw);
              fclose(fp);
         }else{
              printf("Could not open the file");
              increases = -1;
         }

    }else{
         printf("Could not allocate the necessary resources\n");
    }
    return increases;
}
