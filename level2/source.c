#include <stdio.h>


void p(void)

{
  unsigned int *unaff_retaddr;
  char local_50 [76];
  
  fflush(stdout);
  gets(local_50);
  unaff_retaddr = __builtin_return_address(0);
  if ((*unaff_retaddr & 0xb0000000) == 0xb0000000) {
    printf("(%p)\n", (void *)unaff_retaddr);
    _exit(1);
  }
  puts(local_50);
  strdup(local_50);
  return;
}



void main(void)

{
  p();
  return;
}
