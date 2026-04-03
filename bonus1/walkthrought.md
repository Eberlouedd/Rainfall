v5 doit etre egale a 1464814662 et nous avons le controle sur v5 avec argv[1]
mais il y a une garde v5 ne peut pas etre superieur a 9

il y a une faille apparente on peut copier avec memcpy l'equivalent de 4 fois v5 dans dest qui a
une taille de de 40 octets

sauf que 9*4 = 36 on peut copier au maximum 36 octets dans dest ce qui ne nous permet pas de faire un overflow par dest qui a une taille de 40
mais il y a un moyen de contourner ceci :

On sait que le 3eme argument de memcpy est un size_t, c'est donc un entier non signe donc le plus grand entier possible est 4294967295

on sait que lorsqu'on depasse la range de la taille permise il y a un modulo 2^32 qui est fait

donc on prend le plus petit int possible -2147483637, on le multiplie par 4 ce qui donne -8589934548, si on essaye de le mettre dans un size_t
il y aura un modulo 2^32 donc : -8589934548 modulo 2^32 = 44

c'est super cela veut dire quon peut overflow dans dest.

on pourrait calcule l'offset mais enfaite on a que 44 donc la varible dest est probablement directement colle a v5 car elle fait 40 octet et v5 comme c'est un int fait 4 octet ducoup ca sert a rien de deviner l'offset. donc on peut overflow par dest et y ecrire dans v5 la valeur que nous voulons pour executer le shell

il faut que v5 soit egal a 1464814662 en hexadecimal 0x574f4c46

./bonus1 -2147483637 $(python -c 'print "A" * 40 + "\x46\x4c\x4f\x57"')

ca marche on a un shell

cat /home/user/bonus2/.pass


