#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FN "input.txt"
#define MAXBIN 12
#define BASE 2

typedef struct input_bin_{
     char bin[MAXBIN+1];
     struct input_bin_ *next;
}input_bin_t;

//Lists are redundant but were implemented to exercise for my FDI exam

void EGrate(input_bin_t *data, int gamma[], int epsilon[], int dim); //
void checksBitCriteria(input_bin_t *data, input_bin_t **oxy,  input_bin_t **cotwo, int dim);
void OCO2rate(input_bin_t *data, int gamma[], int epsilon[], int o[], int co2[], int dim);

input_bin_t *loadData(char filename[], input_bin_t *h);
input_bin_t *pushBin(input_bin_t *h, char bin[]);

int binToDec(int bin[], int dim);

int main(){
     int gamma[MAXBIN], epsilon[MAXBIN], o[MAXBIN], co2[MAXBIN];
     int gam, eps, oxy, cotwo;
     input_bin_t *data, *tmp;
     data = loadData(FN, data);
     EGrate(data, gamma, epsilon, MAXBIN);
     OCO2rate(data, gamma, epsilon, o, co2, MAXBIN);
     gam = binToDec(gamma, MAXBIN);
     eps = binToDec(epsilon, MAXBIN);
     oxy = binToDec(o, MAXBIN);
     cotwo = binToDec(co2, MAXBIN);
     printf("%d * %d = %d\n", gam, eps,  gam*eps);
     printf("%d * %d = %d\n", oxy, cotwo,  oxy*cotwo);

     /*Let's get cleaning*/
     while(data){
          tmp = data;
          data = data->next;
          free(tmp);
     }
}

void EGrate(input_bin_t *data, int gamma[], int epsilon[], int dim){ //
     input_bin_t *curr;
     int i,len;
     for(i=0;i<dim;i++)
     gamma[i] = epsilon[i] = 0;

     for(len=0, curr = data; curr; curr = curr->next, len++){
          for(i=0;curr->bin[i]!='\0';i++){
               gamma[i] += (curr->bin[i]) - '0';
          }
     }
     for(i=0;i<dim;i++){
          gamma[i] = (gamma[i]+(len/2))/len;
          epsilon[i] = !(gamma[i]);
     }
}


void OCO2rate(input_bin_t *data, int gamma[], int epsilon[], int o[], int co2[], int dim){
     //if two elements remain they are the only two valid
     input_bin_t *oxy, *cotwo;
     oxy = NULL;
     cotwo = NULL;
     int i;
     checksBitCriteria(data, &oxy, &cotwo, MAXBIN);
     for(i=0;i<dim;i++){
          o[i] = (oxy->bin[i]) -'0';
          co2[i] = (cotwo->bin[i]) -'0';
     }
}

void checksBitCriteria(input_bin_t *data, input_bin_t **oxy,  input_bin_t **cotwo, int dim){
     input_bin_t *tmp, *curr, *prec;
     int gamma[MAXBIN], epsilon[MAXBIN];
     int i, valid, len;
     for(; data; data=data->next){
          *oxy = pushBin(*oxy, data->bin);
          *cotwo = pushBin(*cotwo, data->bin);
     }

     for(i=0; i<dim && (*oxy)->next; i++){
          EGrate(*oxy, gamma, epsilon, MAXBIN);
          for(curr= *oxy, prec=NULL; curr;){
               valid = (curr->bin[i]) - '0' == gamma[i];
               if(!valid){
                    if(!prec){
                         *oxy = curr->next;
                         tmp = curr;
                         curr = curr->next;
                    }else{
                         prec->next = curr->next;
                         tmp = curr;
                         curr = curr->next;
                    }
                    free(tmp);
               }else{
                    prec=curr;
                     curr=curr->next;
               }
          }
          for(curr=*oxy; curr;curr=curr->next){
               printf("%s ->", curr->bin);
          }
     }
     for(i=0; i<dim && (*cotwo)->next; i++){
          EGrate(*cotwo, gamma, epsilon, MAXBIN);
          for(curr= *cotwo, prec=NULL; curr;){
               valid = (curr->bin[i]) - '0' == epsilon[i];
               if(!valid){
                    if(!prec){
                         *cotwo = curr->next;
                         tmp = curr;
                         curr = curr->next;
                    }else{
                         prec->next = curr->next;
                         tmp = curr;
                         curr = curr->next;
                    }
                    free(tmp);
               }else{
                    prec=curr;
                     curr=curr->next;
               }
          }
     }
}

input_bin_t *loadData(char filename[], input_bin_t *h){
     FILE *fp;
     input_bin_t *data;
     data = 0;
     char bin[MAXBIN+1];
     fp = fopen(filename, "r");
     if(fp){
          fscanf(fp, "%s", bin);
          while(!feof(fp)){
               data = pushBin(data, bin);
               fscanf(fp, "%s", bin);
          }
     }else{
          printf("Could not open the file\n");
     }
     return data;
}

input_bin_t *pushBin(input_bin_t *h, char bin[]){
     input_bin_t *tmp;
     tmp = malloc(sizeof(input_bin_t));

     if(tmp){
          strcpy(tmp->bin, bin);
          if(!h){
               h = tmp;
               h->next = NULL;
          }else{
               tmp->next = h;
               h = tmp;
          }
     }else{
          printf("Could not allocate memory");
          h = 0;
     }
     return h;
}

int binToDec(int bin[], int dim){
     int i, pow, dec;
     for(i=MAXBIN-1, pow=1, dec=0; i>=0; i--, pow *=2){
          dec += bin[i]*pow;
     }
     return dec;
}
