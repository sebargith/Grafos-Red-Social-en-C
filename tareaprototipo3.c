#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PERSONAS 100
#define MAX_AMIGOS 50
#define MAX_INTERESES 20
#define MAX_PAISES 20

typedef struct {
    char nombre[20];
    char pais[50];
    int edad;
    int esInflucencer;
    int NumeroDeAmigos;
    char Amigos[MAX_AMIGOS][50];
    int NumeroDeIntereses;
    char Intereses[MAX_INTERESES][50];
    int Visitado;
} Persona;

typedef struct {
    char miembros[MAX_PERSONAS][50];
    int NumeroDeMiembros;
} Comunidad;

Persona personas[MAX_PERSONAS];
Comunidad comunidades[MAX_PERSONAS];
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

void BFS(int IndicePersonaInicial, int indicecomunidad) {
    int queue[MAX_PERSONAS];
    int frente = 0, atras = 0;

    AgregarMiembroAComunidad(indicecomunidad, personas[IndicePersonaInicial].nombre);
    personas[IndicePersonaInicial].Visitado = 1;
    queue[atras++] = IndicePersonaInicial;

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

void filtrarPersonas(const char* p, int edadminima,int edadmaxima, const char*hobby) {
    printf("Filtrando personas:\n");
    int usuariosEnComunidad = 0;
    int contadorinf = 0;
    int contadorNoinf = 0;

    for (int i = 0; i < NumeroDeComunidades; i++) {
        usuariosEnComunidad = 0; // Resetea los contadores para cada comunidad
        contadorinf = 0;
        contadorNoinf = 0;
        printf("\nComunidad: %d\n", i + 1);
        for (int j = 0; j < comunidades[i].NumeroDeMiembros; j++) {
            int indicemiembro = encontrarpersona(comunidades[i].miembros[j]);
            if (indicemiembro != -1) {

                //filtros
                int FiltroPorPais = (strcmp(p, "-1") == 0) || (strcmp(personas[indicemiembro].pais, p) == 0);   //filtra el pais
                int FiltroPorEdad = (personas[indicemiembro].edad > edadminima - 1 && personas[indicemiembro].edad < edadmaxima + 1);  //filtra la edad
                //si se elige un interes, corre esto
                if (strcmp(hobby, "-1") != 0){       
                    for (int k=0; k<personas[indicemiembro].NumeroDeIntereses; k++){
                        for (int l = 0; l < personas[k].NumeroDeIntereses; l++) {
                            if (strcmp(personas[indicemiembro].Intereses[k], hobby) == 0) {
                                if (FiltroPorPais && FiltroPorEdad){
                                    printf(" - %s\n",comunidades[i].miembros[j]);
                                    usuariosEnComunidad++;

                                    //Divide entre influencer y no influencer
                                    if (personas[indicemiembro].esInflucencer == 1){
                                        contadorinf++;
                                    }
                                    else {
                                        contadorNoinf++;
                                    }
                                }
                                break;
                            }
                        }
                    }
                }
                //si no elige interes, corre esto
                else {                               
                    if (FiltroPorPais && FiltroPorEdad){
                        printf(" - %s\n",comunidades[i].miembros[j]);               
                        usuariosEnComunidad++;

                        //Divide entre influencer y no influencer
                        if (personas[indicemiembro].esInflucencer == 1){
                            contadorinf++;
                        }
                        else {
                            contadorNoinf++;
                        }
                    }
                }
            }
        }  
        printf("La comunidad %d tiene %d cuentas de usuarios.\n", i+1, usuariosEnComunidad);
        printf("Esta comunidad tiene %d creadores de contenido y tiene %d usuarios regulares.\n", contadorinf, contadorNoinf);
    }
}


int main() {
    char f[100];
    char p[50];
    int edadminima, edadmaxima;
    char hobby[50];
    printf("Ingrese el nombre del archivo a leer: ");
    scanf("%s", f);

    int otra = 1; // Variable para el do loop

    do {
        //reinicia las instancias
        NumeroDePersonas = 0;
        NumeroDeComunidades = 0;
        memset(personas, 0, sizeof(personas));
        memset(comunidades, 0, sizeof(comunidades));


    FILE* file = fopen(f, "r");
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

    
    printf("\nDesea hacer otra consulta? (1=si, 0=no): ");
        scanf("%d", &otra);

    } while (otra);

    return 0;
}