#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *auth;
char *service;

int main(void)
{
    char buffer[128];

    while (1)
    {
        printf("%p, %p \n", auth, service);

        if (!fgets(buffer, sizeof(buffer), stdin))
            break;

        if (!strncmp(buffer, "auth ", 5))
        {
            auth = malloc(4);
            *(int *)auth = 0;

            if (strlen(buffer + 5) <= 30)
                strcpy(auth, buffer + 5);
        }

        if (!strncmp(buffer, "reset", 5))
            free(auth);

        if (!strncmp(buffer, "service", 6))
            service = strdup(buffer + 7);

        if (!strncmp(buffer, "login", 5))
        {
            if (auth[32])
                system("/bin/sh");
            else
                fwrite("Password:\n", 1, 10, stdout);
        }
    }

    return 0;
}