int __cdecl main(int argc, const char **argv, const char **envp)
{
  char s[5]; // [esp+20h] [ebp-88h] BYREF
  char v5[2]; // [esp+25h] [ebp-83h] BYREF
  char v6[129]; // [esp+27h] [ebp-81h] BYREF

  while ( 1 )
  {
    printf("%p, %p \n", auth, (const void *)service);
    if ( !fgets(s, 128, stdin) )
      break;
    if ( !memcmp(s, "auth ", 5u) )
    {
      auth = (char *)malloc(4u);
      *(_DWORD *)auth = 0;
      if ( strlen(v5) <= 0x1E )
        strcpy(auth, v5);
    }
    if ( !memcmp(s, "reset", 5u) )
      free(auth);
    if ( !memcmp(s, "service", 6u) )
      service = (int)strdup(v6);
    if ( !memcmp(s, "login", 5u) )
    {
      if ( *((_DWORD *)auth + 8) )
        system("/bin/sh");
      else
        fwrite("Password:\n", 1u, 0xAu, stdout);
    }
  }
  return 0;
}

decompiler avec hex-rays

j'ecris ici le processus de decompilation car il n'est pas aussi trivial que dans les autres cas

d'abord on peut voir que s, v5, v6 ont quelque octet de decalage par rapport a esp ce qui fait penser que ce n'est qu'une seul varible
qu'ont nommera buffer, fgets attend 128 octet pour s, on peut supposer : char buffer[128]

on peut aisement deviner que if ( !memcmp(s, "auth ", 5u) ) equivaut enfaite a if (!strncmp(buf, "auth ", 5)) c'est la meme chose
pour "service" "login" et "reset"

auth = (char *)malloc(4u);
signifie que auth est un char donc on peut declarer une global char *auth

*(_DWORD *)auth = 0;
ca peut etre auth[0] = 0

if ( strlen(v5) <= 0x1E )
    strcpy(auth, v5);

v5 est a la 5eme place du buffer comme on peur le voir avec esp 88 - 83 = 5 donc v5 veut dire buffer + 5
ce qui corrobore cette theorie c'est que "auth " fait 5 caracter le programme semble vouloir recuperer ce qu'il y a apres "auth " dans le buffer
ca devient donc


if ( strlen(buffer + 5) <= 30 )
    strcpy(auth, buffer + 5);

Meme logique pour ca : service = (int)strdup(v6); 88 - 81 = 7 donc on traduit service = strdup(buffer + 7) et service et un char* car il attend le retour
du strdup() le decompilateur s'est trompé car il a du voir 4 octets

if ( *((_DWORD *)auth + 8) )
    system("/bin/sh");

auth a ete caste encore avec le format D_WORD qui equivaut a 4 octets donc
on fait 8 * 4 = 32 ce qui donne auth + 32 : auth[32]

if (auth[32] != 0)
  system("/bin/sh");

Le resulat final est dans source.c
