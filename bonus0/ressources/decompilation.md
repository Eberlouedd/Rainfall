void p(char *param_1,char *param_2)

{
  char *pcVar1;
  char local_100c [4104];
  
  puts(param_2);
  read(0,local_100c,0x1000);
  pcVar1 = strchr(local_100c,10);
  *pcVar1 = '\0';
  strncpy(param_1,local_100c,0x14);
  return;
}

void pp(char *param_1)

{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  byte bVar4;
  char local_34 [20];
  char local_20 [20];
  
  bVar4 = 0;
  p(local_34,&DAT_080486a0);
  p(local_20,&DAT_080486a0);
  strcpy(param_1,local_34);
  uVar2 = 0xffffffff; // -1
  pcVar3 = param_1;
  do {
    if (uVar2 == 0) break;
    uVar2 = uVar2 - 1;
    cVar1 = *pcVar3;
    pcVar3 = pcVar3 + (uint)bVar4 * -2 + 1;
  } while (cVar1 != '\0');
  (param_1 + (~uVar2 - 1))[0] = ' ';
  (param_1 + (~uVar2 - 1))[1] = '\0';
  strcat(param_1,local_20);
  return;
}



undefined4 main(void)

{
  char local_3a [42];
  
  pp(local_3a);
  puts(local_3a);
  return 0;
}

dechiffrement de la fonction p :


void p(char *param_1, char *param_2)
{
    char buffer[4104];

    puts(param_2);

    read(0, buffer, 0x1000);

    strchr(buffer, '\n') = 0;

    strncpy(param_1, buffer, 20);
}

c'est assez intuitif j'ai juste rennome les variable pour que se soit plus lisible

traduction de pp :

p(local_34,&DAT_080486a0);
p(local_20,&DAT_080486a0);

dans d'autre decompiateur on a :

p(&bLoc34, " - ");
p(&bLoc20, " - ");

donc je deduis que &DAT_080486a0 =  ' - '

le do while ressemble a un strlen il fait un compteur jusqu'a cvar qui est egal a param_1 et stock le resulatat dans uvar2

donc le block do while equivaut a size_t len = strlen(param_1);

(param_1 + (~uVar2 - 1))[0] = ' ';
(param_1 + (~uVar2 - 1))[1] = '\0';

sachant que ~uvar2 - 1 est la len de param_1 (car uvar2 commence a -1 et se decremente a chaque iteration, ~ cest valeur absolue) on peut traduire que :

param_1[len] = ' ';
param_1[len + 1] = '\0';

c'est juste une differente notation en c

(param_1 + (len))[0] equivaut a param_1[len]

le main est clair pas besoin d'expliquer a part que la taille du buffer est de 42 car c'est une taille coherente pour le code 20 + 20 + espace + null = 42
ce qui corobore cette hypothese c'est que dans d'autre decompilateur je trouve 42




