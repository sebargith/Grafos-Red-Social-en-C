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

void AgregarMiembroAComunidad(int indicecomunidad, const char* NombreDeMiembro, const char* country, int edMax, int edMin, const char* hobby){

    Comunidad* comunidad = &comunidades[indicecomunidad];

    if (strcmp(personas[indicepersona].pais, country) != 0) return;  // La persona no cumple con el filtro de país, se omite

    if (personas[indicepersona].edad > edMax || personas[indicepersona].edad < edMin) return;  // La persona no cumple con los filtros de edad, se omite

    int tieneInteres = 0;
    for (int k = 0; k < personas[indicepersona].NumeroDeIntereses; k++) {
        
        if (strcmp(personas[indicepersona].Intereses[k], hobby) == 0) {
        
            tieneInteres = 1;
            break;
        }
    }

    if (strcmp(hobby, "-1") != 0 && !tieneInteres) return;  // La persona no cumple con el filtro de interés, se omite

    for (int i = 0; i < comunidad->NumeroDeMiembros; i++) {

        if (strcmp(comunidad->miembros[i], NombreDeMiembro) == 0) return;  // evita duplicados
    }

    strcpy(comunidad->miembros[comunidad->NumeroDeMiembros], NombreDeMiembro);
    
    comunidad->NumeroDeMiembros++;
}

int encontrarpersona(const char* nombre) {
    
    for (int i = 0; i < NumeroDePersonas; i++) {
    
        if (strcmp(personas[i].nombre, nombre) == 0) return i;
    }
    
    return -1;
}

void BFS(int startPersonIndex, int indicecomunidad, const char* country, int edMax, int edMin, const char* hobby){
    
    int queue[MAX_PERSONS];
    int frente = 0, atras = 0;

    if (strcmp(personas[indiceamigo].pais, country) != 0) continue;  // La persona no cumple con el filtro de país, se omite

    if (personas[indiceamigo].edad > edMax || personas[indiceamigo].edad < edMin) continue;  // La persona no cumple con los filtros de edad, se omite

    int tieneInteres = 0;
    for (int k = 0; k < personas[indiceamigo].NumeroDeIntereses; k++) {
        
        if (strcmp(personas[indiceamigo].Intereses[k], hobby) == 0) {
        
            tieneInteres = 1;
            break;
        }
    }

    if (strcmp(hobby, "-1") != 0 && !tieneInteres) continue;  // La persona no cumple con el filtro de interés, se omite

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
    
            BFS(i, NumeroDeComunidades, country, edMax, edMin, hobby);
            NumeroDeComunidades++;
        }
    }
}

void ImprimirComunidades() {
    
    for (int i = 0; i < NumeroDeComunidades; i++) {
    
        printf("Comunidad %d:\n", i + 1);
    
        for (int j = 0; j < comunidades[i].NumeroDeMiembros; j++) {
    
            printf("- %s\n", comunidades[i].miembros[j]);
        }
    
        printf("\n");
    }
}

void losquecocinan(){

    printf("Los que cocinan son:\n");

    for (int i=0; i<NumeroDeComunidades; i++){

        printf("\nComunidad: %d\n", i+1);

        for (int j=0; j<comunidades[i].NumeroDeMiembros; j++){

            int memberIndex = encontrarpersona(comunidades[i].miembros[j]);

            if (memberIndex != -1){

                for (int k=0; k<personas[memberIndex].NumeroDeIntereses; k++){

                    if(strcmp(personas[memberIndex].Intereses[k], "Cocina") == 0){

                        printf(" -%s\n", comunidades[i].miembros[j]);
                        break;
                    }
                }
            }        
        }
    }
}

void edadmaxima(){

    printf("\nMayor a 25 y menor a 50\n");

    for (int i=0; i<NumeroDeComunidades; i++){

        printf("\nComunidad: %d\n", i+1);

        for (int j=0; j<comunidades[i].NumeroDeMiembros; j++){

            //recorriendo las comunidades de otra forma para encontrar personas

            for (int k=0; k<NumeroDePersonas; k++){

                if (strcmp(personas[k].nombre, comunidades[i].miembros[j]) == 0){  //recorre el array de personas y se fija si algun nombre coincide con el de miem[i]com[j], para ver si pertece a esa comunidad

                    if(personas[k].edad >24 && personas[k].edad<51){

                        printf(" -%s\n",comunidades[i].miembros[j]);
                    }
                }              
            }

        }
    }
}

int main(){

    char archivo[100], country[40], hobby[40];
    int edMin, edMax;

    printf("ingrese el nobre del archivo a leer: ");
    scanf("%s", archivo);

    FILE* file = fopen(archivo, "r");
    
    if (file == NULL) {
    
        printf("No se pudo abrir personas.txt\n");
        return 1;
    }

    printf("ingrese filtros de busqueda\n");
    
    printf("ingrese pais (ingresar -1 si no sabe): ");
    scanf("%s", country);
    
    printf("ingrese edad minima (ingresar 0 si es por defecto): ");
    scanf("%d", &edMin);
    
    
    printf("ingrese edad maxima (ingresar 120 si es por defecto): ");
    scanf("%d", &edMax);

    printf("ingrese interes (ingresar -1 si es por defecto): ");
    scanf("%s", hobby);
    printf("\n")

    printf("los filtros dados son: %s %d %d %s \n", country, edMin, edMax, hobby);
    
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
    ImprimirComunidades();
    losquecocinan();
    edadmaxima();

    return 0;
}