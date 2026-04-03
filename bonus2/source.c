#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int language = 0;

int greetuser(char *name)
{
    char dest[72];

    switch (language)
    {
        case 1:
            strcpy(dest, "Hyvää päivää "); 
            break;

        case 2:
            strcpy(dest, "Goedemiddag! ");
            break;

        case 0:
        default:
            strcpy(dest, "Hello ");
            break;
    }

    strcat(dest, name);
    return puts(dest);
}

int main(int argc, char **argv)
{
    char buffer[76];
    char dest[76];
    char *lang;

    if (argc != 3)
        return 1;

    memset(dest, 0, sizeof(dest));

    strncpy(dest, argv[1], 40);
    strncpy(dest + 40, argv[2], 32);

    lang = getenv("LANG");

    if (lang)
    {
        if (!memcmp(lang, "fi", 2))
            language = 1;
        else if (!memcmp(lang, "nl", 2))
            language = 2;
    }

    memcpy(buffer, dest, sizeof(buffer));

    return greetuser(buffer);
}