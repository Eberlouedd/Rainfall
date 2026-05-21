### Level 0

#### Etape 01 :

On commence par ouvrir le binaire dans Ghidra  pour inspecter la fonction `main`.
En lisant le pseudo-code décompilé, on remarque la logique suivante :
- le programme attend un argument en ligne de commande ;
- cet argument est converti en entier et eststocke dans `password` ;
- si la valeur correspond à `423`, le programme lance un shell via `system`/`exec*` avec les droits du propriétaire du binaire.

#### Etape 02 :
On exécute simplement le programme avec la bonne valeur :

```bash
./level0 423
```

Si l'analyse est correcte, on obtient un shell avec les privilèges de `level1`.

#### Etape 03 :
Depuis ce shell, on lit le fichier de mot de passe du niveau suivant :

```bash
cat /home/user/level1/.pass
```

La sortie de cette commande est le mot de passe à utiliser pour se connecter à `level1`.
