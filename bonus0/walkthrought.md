

la faille se trouve dans le strncpy car on va pouvoir copier un chaine sans null terminator
donc le prochain stcrpy va copier plus loin que la chaine qu'il recoit
donc si on ecrit plus de 20 character lors du premier input le strcpy va aussi prendre les 20 autre caractere de input2
donc on peut atteindre un buffer de : 20 octet du premier input + 20 octets du second input grasse au strcpy qui copy trop + 1 octet d'espace + 20 octet de l'input 2 ce qui donne 61 octets au total
donc le buffer dans main de 42 octet peut etre deborde de 19 octets
On va chercher l'offset pour atteindre l'eip

0xbffffe57 em little edian "\x57\xfe\xff\xbf"