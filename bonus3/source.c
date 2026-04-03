#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv, char** envp)
{
    FILE* fp = fopen("/home/user/end/.pass", "r");
    char buf[0x41];
    __builtin_memset(&buf, 0, 0x84);
    
    if (!fp || argc != 2)
        return 0xffffffff;
    
    fread(&buf, 1, 66, fp);
    char var_57_1 = 0;
    buf[atoi(argv[1])] = 0;
    char var_56[0x42];
    fread(&var_56, 1, 65, fp);
    fclose(fp);
    
    if (strcmp(&buf, argv[1]))
        puts(&var_56);
    else
        execl("/bin/sh", "sh", 0);
    
    return 0;
}