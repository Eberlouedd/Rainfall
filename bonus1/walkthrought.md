
### Bonus 1

#### Etape 01 :


v5 doit être égal à 1464814662 et nous avons le contrôle de `v5` via `argv[1]`. Mais il y a une garde : `v5` ne peut pas être supérieur à 9.

Il y a une faille apparente : on peut copier avec `memcpy` l'équivalent de 4 fois `v5` dans `dest`, qui a une taille de 40 octets.

Sauf que 9*4 = 36 : on peut copier au maximum 36 octets dans `dest`, ce qui ne permet pas d'overflower `dest` (taille 40). Mais il existe un moyen de contourner ceci :

On sait que le 3ᵉ argument de `memcpy` est un `size_t` : c'est un entier non signé, donc la valeur maximale est 4294967296.

On sait que lorsqu'on dépasse la plage de la taille permise, un modulo 4294967296 est appliqué.

Donc on prend le plus petit `int` possible -2147483637, on le multiplie par 4 ce qui donne -8589934548. Si on essaye de le mettre dans un `size_t`,
il y aura un modulo 4294967296 donc : -8589934548 modulo 4294967296 = 44.

C'est super : cela signifie qu'on peut overflower `dest`.


#### Etape 02 :

On pourrait calculer l'offset, mais en fait on n'a que 44, donc la variable `dest` est probablement directement collée à `v5` : `dest` fait 40 octets et `v5`, comme c'est un `int`, fait 4 octets. Du coup il n'est pas nécessaire de deviner l'offset — on peut overflower `dest` et écrire dans `v5` la valeur souhaitée pour exécuter le shell.

Il faut que `v5` soit égal à 1464814662 (hexadécimal 0x574f4c46).

```bash
./bonus1 -2147483637 $(python -c 'print "A" * 40 + "\x46\x4c\x4f\x57"')
```

Ça marche : on a un shell.

```bash
cat /home/user/bonus2/.pass
```



