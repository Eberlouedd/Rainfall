#include <stdio.h>
#include <stdlib.h>

void m(void *param_1,int param_2,char *param_3,int param_4,int param_5)
{
  puts("Nope");
  return;
}

void n(void)
{
  system("/bin/cat /home/user/level7/.pass");
  return;
}

void main(int argc,char **argv)
{
  char *__dest;
  void(*puVar1)(void);
  
  __dest = malloc(64);
  void(*puVar1)(void) = malloc(4);
  void(*puVar1)(void) = (void (*)())m;
  strcpy(__dest,*(char **)(argv[1]));
  ((void (*)())*(void **)puVar1)();
  return;
}
