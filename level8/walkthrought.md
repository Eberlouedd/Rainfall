### Level 8

#### Etape 01 :

On identifie la condition critique dans `login` :

- le programme lance `/bin/sh` si `auth[32]` est different de `0`,
- sinon il affiche `Password:`.

Le but est donc de controler la valeur lue a l'offset `32` depuis `auth`.

#### Etape 02 :

On regarde les commandes disponibles :

- `auth <data>` : alloue `auth` avec `malloc(4)`, puis copie `<data>` dedans,
- `service <data>` : fait `service = strdup(<data>)`,
- `login` : teste `auth[32]`.

Le probleme est que `auth` n'a que 4 octets alloues, mais le programme lit quand meme `auth[32]`.
C'est un acces hors limites (out-of-bounds read).

#### Etape 03 :

Au lancement, le binaire affiche les adresses de `auth` et `service`.
En pratique, apres `auth` puis `service`, les zones sont proches en memoire.

Du coup, avec une chaine suffisamment longue dans `service`, l'octet lu par `auth[32]` peut tomber dans la zone de `service` et devenir non nul.

#### Etape 04 :

On exploite avec la sequence suivante :

```text
auth a
service eeeeeeeeeeeeeeeeeeee
login
```

- `auth a` initialise `auth`,
- `service ...` place des `e` (non nuls) dans la zone voisine,
- `login` verifie `auth[32]`, lit une valeur non nulle, et execute `/bin/sh`.

#### Etape 05 :

Une fois le shell obtenu, on recupere le mot de passe du niveau suivant :

```bash
cat /home/user/level9/.pass
```

On obtient alors le flag du level9.