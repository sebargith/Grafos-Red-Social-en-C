#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PERSONS 100
#define MAX_FRIENDS 10
#define MAX_INTERESTS 10

typedef struct {
    char nombre[50];
    char pais[50];
    int edad;
    int esInflucencer;
    int NumeroDeAmigos;
    char Amigos[MAX_FRIENDS][50];
    int NumeroDeIntereses;
    char Intereses[MAX_INTERESTS][50];
    int Visitado;
} Persona;

typedef struct {
    char miembros[MAX_PERSONS][50];
    int NumeroDeMiembros;
} Comunidad;

Persona personas[MAX_PERSONS];
Comunidad comunidades[MAX_PERSONS];
int NumeroDePersonas = 0;
int NumeroDeComunidades = 0;

void AgregarPersona(const char* nombre, const char* pais, int edad, int esInflucencer) {
    Persona persona;
    strcpy(persona.nombre, nombre);
    strcpy(persona.pais, pais);
    persona.edad = edad;
    persona.esInflucencer = esInflucencer;
    persona.NumeroDeAmigos = 0;
    persona.NumeroDeIntereses = 0;
    persona.Visitado = 0;
    personas[NumeroDePersonas++] = persona; 
}

void AgregarAmigos(int indicepersona, const char* NombreAmigo) {
    Persona* persona = &personas[indicepersona];
    strcpy(persona->Amigos[persona->NumeroDeAmigos], NombreAmigo);
    persona->NumeroDeAmigos++;
}

void AgregarIntereses(int indicepersona, const char* interest) {
    Persona* persona = &personas[indicepersona];
    strcpy(persona->Intereses[persona->NumeroDeIntereses], interest);
    persona->NumeroDeIntereses++;
}

void AgregarMiembroAComunidad(int indicecomunidad, const char* NombreDeMiembro) {
    Comunidad* comunidad = &comunidades[indicecomunidad];
    for (int i = 0; i < comunidad->NumeroDeMiembros; i++) {
        if (strcmp(comunidad->miembros[i], NombreDeMiembro) == 0) {
            return;  // evita duplicados
        }
    }
    strcpy(comunidad->miembros[comunidad->NumeroDeMiembros], NombreDeMiembro);
    comunidad->NumeroDeMiembros++;
}

int encontrarpersona(const char* nombre) {
    for (int i = 0; i < NumeroDePersonas; i++) {
        if (strcmp(personas[i].nombre, nombre) == 0) {
            return i;
        }
    }
    return -1;
}

void BFS(int startPersonIndex, int indicecomunidad) {
    int queue[MAX_PERSONS];
    int frente = 0, atras = 0;

    AgregarMiembroAComunidad(indicecomunidad, personas[startPersonIndex].nombre);
    personas[startPersonIndex].Visitado = 1;
    queue[atras++] = startPersonIndex;

    while (frente != atras) {
        int currentPersonIndex = queue[frente++];
        Persona* personaActual = &personas[currentPersonIndex];

        for (int i = 0; i < personaActual->NumeroDeAmigos; i++) {
            int indiceamigo = encontrarpersona(personaActual->Amigos[i]);
            if (indiceamigo != -1 && !personas[indiceamigo].Visitado) {
                AgregarMiembroAComunidad(indicecomunidad, personas[indiceamigo].nombre);
                personas[indiceamigo].Visitado = 1;
                queue[atras++] = indiceamigo;
            }
        }

        if (personaActual->esInflucencer) {
            for (int i = 0; i < NumeroDePersonas; i++) {
                if (!personas[i].Visitado) {
                    for (int j = 0; j < personas[i].NumeroDeAmigos; j++) {
                        if (strcmp(personas[i].Amigos[j], personaActual->nombre) == 0) {
                            AgregarMiembroAComunidad(indicecomunidad, personas[i].nombre);
                            personas[i].Visitado = 1;
                            queue[atras++] = i;
                            break;
                        }
                    }
                }
            }
        }
    }
}

void FormarComunidades() {
    for (int i = 0; i < NumeroDePersonas; i++) {
        if (!personas[i].Visitado) {
            AgregarMiembroAComunidad(NumeroDeComunidades, personas[i].nombre);
            personas[i].Visitado = 1;  // Se resetea el tag de Visitado para la persona del comienzo
            BFS(i, NumeroDeComunidades);
            NumeroDeComunidades++;
        }
    }
}

/*
void ImprimirComunidades() {
    for (int i = 0; i < NumeroDeComunidades; i++) {
        printf("Comunidad %d:\n", i + 1);
        for (int j = 0; j < comunidades[i].NumeroDeMiembros; j++) {
            printf("- %s\n", comunidades[i].miembros[j]);
        }
        printf("\n");
    }
}

/*
void losquecocinan(){
    printf("Los que cocinan son:\n");
    for (int i=0; i<NumeroDeComunidades; i++){
        printf("\nComunidad: %d\n", i+1);
        for (int j=0; j<comunidades[i].NumeroDeMiembros; j++){
            int indicemiembro = encontrarpersona(comunidades[i].miembros[j]);
            if (indicemiembro != -1){
                for (int k=0; k<personas[indicemiembro].NumeroDeIntereses; k++){
                    if(strcmp(personas[indicemiembro].Intereses[k], "Cocina") == 0){
                        printf(" -%s\n", comunidades[i].miembros[j]);
                        break;
                    }
                }
            }        
        }
    }
}

void filtroPorEdad(int edadminima, int edadmaxima){
    printf("\nMayor a %d y menor a %d\n", edadminima, edadmaxima);
    for (int i=0; i<NumeroDeComunidades; i++){
        printf("\nComunidad: %d\n", i+1);
        for (int j=0; j<comunidades[i].NumeroDeMiembros; j++){
            //recorriendo las comunidades de otra forma para encontrar personas
            for (int k=0; k<NumeroDePersonas; k++){
                if (strcmp(personas[k].nombre, comunidades[i].miembros[j]) == 0){  //recorre el array de personas y se fija si algun nombre coincide con el de miem[i]com[j], para ver si pertece a esa comunidad
                    if(personas[k].edad > edadminima -1 && personas[k].edad< edadmaxima +1){
                        printf(" -%s\n",comunidades[i].miembros[j]);
                    }
                }              
            }

        }
    }
}

void filtroPorPais(const char* p) {
    printf("\nDe: %s\n", p);
    for (int i = 0; i < NumeroDeComunidades; i++) {
        printf("\nComunidad: %d\n", i + 1);
        for (int j = 0; j < comunidades[i].NumeroDeMiembros; j++) {
            int indicemiembro = encontrarpersona(comunidades[i].miembros[j]);
            if (indicemiembro != -1) {
                if (strcmp(personas[indicemiembro].pais, p) == 0) {
                    printf(" - %s\n", comunidades[i].miembros[j]);
                }
            }
        }
    }
}
*/



void filtrarPersonas(const char* p, int edadminima,int edadmaxima, const char*hobby) {
    printf("Filtrando personas:\n");

    for (int i = 0; i < NumeroDeComunidades; i++) {
        printf("\nComunidad: %d\n", i + 1);
        for (int j = 0; j < comunidades[i].NumeroDeMiembros; j++) {
            int indicemiembro = encontrarpersona(comunidades[i].miembros[j]);
            if (indicemiembro != -1) {
                //filtros



                int FiltroPorPais = (strcmp(p, "-1") == 0) || (strcmp(personas[indicemiembro].pais, p) == 0);   //filtra el pais
                int FiltroPorEdad = (personas[indicemiembro].edad > edadminima - 1 && personas[indicemiembro].edad < edadmaxima + 1);  //filtra la edad
                if (strcmp(hobby, "-1") != 0){       //si se elige un interes, corre esto
                    for (int k=0; k<personas[indicemiembro].NumeroDeIntereses; k++){
                        for (int l = 0; l < personas[k].NumeroDeIntereses; l++) {
                            if (strcmp(personas[k].Intereses[l], hobby) == 0) {
                                if (FiltroPorPais && FiltroPorEdad){
                                    printf(" - %s\n",comunidades[i].miembros[j]);
                                }
                                break;
                            }
                        }
                    }
                }
                else {        //sino no
                    if (FiltroPorPais && FiltroPorEdad){
                        printf(" - %s\n",comunidades[i].miembros[j]);
                    }
                }
            }
        }  
    }
}


int main() {
    char f[100];
    char p[50];
    int edadminima, edadmaxima;
    char hobby[50];
    //printf("Ingrese el nombre del archivo a leer: ");
    //scanf("%s", f);
    FILE* file = fopen("personas.txt", "r");
    if (file == NULL) {
        printf("No se pudo abrir el archivo\n");
        return 1;
    }

    int NumeroTotalPersonas;
    fscanf(file, "%d", &NumeroTotalPersonas);

    for (int i = 0; i < NumeroTotalPersonas; i++) {
        char nombre[50], pais[50];
        int edad, esInflucencer, NumeroDeAmigos, NumeroDeIntereses;
        fscanf(file, "%s %s %d %d", nombre, pais, &edad, &esInflucencer);
        AgregarPersona(nombre, pais, edad, esInflucencer);

        fscanf(file, "%d", &NumeroDeAmigos);
        for (int j = 0; j < NumeroDeAmigos; j++) {
            char NombreAmigo[50];
            fscanf(file, "%s", NombreAmigo);
            AgregarAmigos(i, NombreAmigo);
        }

        fscanf(file, "%d", &NumeroDeIntereses);
        for (int j = 0; j < NumeroDeIntereses; j++) {
            char interest[50];
            fscanf(file, "%s", interest);
            AgregarIntereses(i, interest);
        }
    }

    fclose(file);

    FormarComunidades();


    printf("\nIngrese los criterios de busqueda\n");
    printf("Ingrese el pais, (si no es importante ingrese -1): ");
    scanf("%s", p);

    printf("\nIngrese la edad minima (si no es importante ingrese 0): ");
    scanf("%d",&edadminima);
    printf("\nIngrese la edad maxima (si no es importante ingrese 120): ");
    scanf("%d",&edadmaxima);
    printf("Ingrese el interes, (si no es importante ingrese -1): ");
    scanf("%s", hobby);
    filtrarPersonas(p, edadminima, edadmaxima, hobby);

    








    /*
    FormarComunidades();
    ImprimirComunidades();
    losquecocinan();
    filtroPorEdad();
    */

    return 0;
}



















