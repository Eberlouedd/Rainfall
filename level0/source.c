#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(int argc,char **argv) { 
    int password;
    char *commande;
    __uid_t user_id;
    __gid_t group_id; 
    password = atoi(argv[1]); 
    if (password == 423) {
        commande = strdup("/bin/sh");
        group_id = getegid();
        user_id = geteuid();
        setresgid(group_id,group_id,group_id);
        setresuid(user_id,user_id,user_id);
        execv("/bin/sh",&commande);
    } else {
       fwrite("No !\n",1,5, stderr); 
    }
    return 0; 
}