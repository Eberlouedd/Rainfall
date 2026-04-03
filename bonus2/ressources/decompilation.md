int language;

int __cdecl greetuser(char src)
{
  __int128 dest; // [esp+10h] [ebp-48h] BYREF
  __int16 v3; // [esp+20h] [ebp-38h]
  char v4; // [esp+22h] [ebp-36h]

  switch ( language )
  {
    case 1:
      *(_QWORD *)&dest = 0x20A4C3A4C3767948LL;
      *((_QWORD *)&dest + 1) = 0xC3A4C37669A4C370LL;
      v3 = unk_8048727;
      v4 = unk_8048729;
      break;
    case 2:
      strcpy((char *)&dest, "Goedemiddag! ");
      break;
    case 0:
      strcpy((char *)&dest, "Hello ");
      break;
  }
  strcat((char *)&dest, &src);
  return puts((const char *)&dest);
}
// 8049988: using guessed type int language;

//----- (08048529) --------------------------------------------------------
int __cdecl main(int argc, const char **argv, const char **envp)
{
  char v4[76]; // [esp+0h] [ebp-ACh] BYREF
  char dest[76]; // [esp+50h] [ebp-5Ch] BYREF
  char *v6; // [esp+9Ch] [ebp-10h]

  if ( argc != 3 )
    return 1;
  memset(dest, 0, sizeof(dest));
  strncpy(dest, argv[1], 0x28u);
  strncpy(&dest[40], argv[2], 0x20u);
  v6 = getenv("LANG");
  if ( v6 )
  {
    if ( !memcmp(v6, "fi", 2u) )
    {
      language = 1;
    }
    else if ( !memcmp(v6, "nl", 2u) )
    {
      language = 2;
    }
  }
  qmemcpy(v4, dest, sizeof(v4));
  return greetuser(v4[0]);
}

Reconstruction de greetuser :

v3, v4 ne sont pas utilise dest semble etre un char, le decompilateur a surement fait une separation d'une variable en plusieur parti je vais les regrouper
en une seule variable char dest[72]

on peut reecrire 
*(_QWORD *)&dest = 0x20A4C3A4C3767948LL;
*((_QWORD *)&dest + 1) = 0xC3A4C37669A4C370LL;

en strcpy(dest, "Hyvää päivää ");

car si on remet 0x20A4C3A4C3767948LL et 0xC3A4C37669A4C370LL dans l'odre sachant que c'est en little edian et qu'on converti l'hexa en ascii ca donne Hyvää päivää 

et 'dest =' est de toute evidence l'equivalent d'un strcpy

Pour le main :

j'ai juste renomme les variable et on passe return greetuser(v4[0]); a return greetuser(buffer); 