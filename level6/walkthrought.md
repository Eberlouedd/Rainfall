Il y a une fonction n() qui permet d'avoir le flague donc le but va etre de l'executer

on voit que notre argument va etre copier dans une variable allouer dynamiquement avec un malloc et au'il y a un pointeur sur une fonction m()
qui sera exectuer dans tout les cas. Le jeu va t'etre de passer par un buffer overflow pour passe dans le pointeur et y mettre l'adresse de n()

pour ca il va falloir trouver l'offset correct pour acceder au pointeur de la fonction m()

on voit que le malloc est de 64 pour la variable dans laquelle sera copier notre entré, donc on part de la en tatonnant et on trouve 72 pour offset

run $(python -c 'print "e" * 72')

l'adresse de n() est 0x08048454

a partir de la c'est simple on va juste reecrire l'adresse de n() un overflow :

./level6 $(python -c 'print "A"*72 + "\x54\x84\x04\x08"')

le flag apparait
