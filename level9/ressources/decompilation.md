void __cdecl N::N(N *this, int a2)
{
  *(_DWORD *)this = off_8048848;
  *((_DWORD *)this + 26) = a2;
}

void *__cdecl N::setAnnotation(N *this, char *s)
{
  size_t v2; // eax

  v2 = strlen(s);
  return memcpy((char *)this + 4, s, v2);
}

int __cdecl N::operator+(int a1, int a2)
{
  return *(_DWORD *)(a1 + 104) + *(_DWORD *)(a2 + 104);
}

int __cdecl N::operator-(int a1, int a2)
{
  return *(_DWORD *)(a1 + 104) - *(_DWORD *)(a2 + 104);
}

int __cdecl main(int argc, const char **argv, const char **envp)
{
  N *v3; // ebx
  N *v4; // ebx
  N *v6; // [esp+1Ch] [ebp-8h]

  if ( argc <= 1 )
    _exit(1);
  v3 = (N *)operator new(0x6Cu);
  N::N(v3, 5);
  v6 = v3;
  v4 = (N *)operator new(0x6Cu);
  N::N(v4, 6);
  N::setAnnotation(v6, (char *)argv[1]);
  return (**(int (__cdecl ***)(N *, N *))v4)(v4, v6);
}

decompiler avec hex-ray

Ici aussi il va falloir detailler la decompilation :

on peut facilement comprendre que le programme original est en cpp car
il y a donc visiblement une classe N()

le constructeur definit que this (l'objet) comment a l'adresse 0x8048848
et que il alloue a un parametre int que l'on nomera value un attribu situe au 104eme octet (26*4=104)

la methode setAnnotation alloue a this + 4 le char  char en parametre, le this + 4 est tres probablement
l'attribut annotation, il se trouve donc a la 4eme place et que l'attribut value se situe a la 104eme place
annotation fait donc 100 octet dans on peut definir clairement l'attribut char annotation[100]

les operateur :

sachant l'utilisation courante des operateurs, et le fait qu'il modifie les des variable avec un offset 104 ca doit etre probablement :

int operator+(N &other)
{
    return this->value + other.value;
}

int operator-(N &other)
{
    return this->value - other.value;
}

ce qui corrobore cette hypothese c'est que dans le decompilateur binaryNinja les orperateur sont represente ainsi :
int32_t N::operator+(N& arg1)
int32_t N::operator-(N& arg1)

le main :

v3 = (N *)operator new(0x6Cu);
N::N(v3, 5);
v6 = v3;
v4 = (N *)operator new(0x6Cu);
N::N(v4, 6);
N::setAnnotation(v6, (char *)argv[1]);

0x6C = 108 c'est la taille de notre object
donc on inititialise un premier objet
N *a = new N(5)
pareil pour la suite
N *b = new N(6)
le set annotation est fait sur a car le registe v6 enregiste ce qu'il y a dans v3
donc
a->setAnnotation(argv[1]);


