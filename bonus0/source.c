#include <stdio.h>
#include <string.h>
#include <unistd.h>

void p(char *param_1, char *param_2)
{
    char buffer[4104];

    puts(param_2);

    read(0, buffer, 0x1000);

    char *newline = strchr(buffer, '\n');

    strncpy(param_1, buffer, 20);
}

void pp(char *param_1)
{
    char local_34[20];
    char local_20[20];

    p(local_34, " - ");
    p(local_20, " - ");

    strcpy(param_1, local_34);

    size_t len = strlen(param_1);

    param_1[len] = ' ';
    param_1[len + 1] = '\0';

    strcat(param_1, local_20);
}

int main(void)
{
    char local_3a[42];

    pp(local_3a);

    puts(local_3a);

    return 0;
}