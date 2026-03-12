#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

char c[68];

void m(void *param_1,int param_2,char *param_3,int param_4,int param_5)

{
  time_t tVar1;
  
  tVar1 = time((time_t *)0);
  printf("%s - %d\n", c, tVar1);
  return;
}


int main(int argc, char** argv)
{
    int* eax = malloc(8);
    *eax = 1;
    eax[1] = malloc(8);
    int* eax_4 = malloc(8);
    *eax_4 = 2;
    eax_4[1] = malloc(8);
    strcpy(eax[1], argv[1]);
    strcpy(eax_4[1], argv[2]);
    fgets(&c, 68, fopen("/home/user/level8/.pass", "r"));
    puts("~~");
    return 0;
}