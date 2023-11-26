/*****************************************************************************************************************
    UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
    FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

    Computadoras y programacion.
    (c) Ponga su nombre y numero de cuenta aqui.

    Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.

******************************************************************************************************************/

#include "stdafx.h"
#include <string.h>
#include "corrector.h"
#define DEPURAR 0

//Funciones publicas del proyecto
/*****************************************************************************************************************
    DICCIONARIO: Esta funcion crea el diccionario completo
    char *	szNombre				:	Nombre del archivo de donde se sacaran las palabras del diccionario
    char	szPalabras[][TAMTOKEN]	:	Arreglo con las palabras completas del diccionario
    int		iEstadisticas[]			:	Arreglo con el numero de veces que aparecen las palabras en el diccionario
    int &	iNumElementos			:	Numero de elementos en el diccionario
******************************************************************************************************************/
void Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos) {
    FILE* fpDicc;
    char linea[4000];
    char palabraDetectada[TAMTOKEN];
    int i, j;
    int indicePD;
    char temp[TAMTOKEN];
    iNumElementos = 0;
    char prevChar = '\0';

    fopen_s(&fpDicc, szNombre, "r");
    if (fpDicc != NULL) {
        while (fgets(linea, sizeof(linea), fpDicc)) {
            indicePD = 0;

            for (i = 0; i < strlen(linea); i++) {
                if (linea[i] == ' ' || linea[i] == '\t' || linea[i] == '\n' || linea[i] == ',' || linea[i] == ';' || linea[i] == '(' || linea[i] == ')' || linea[i] == '$' || linea[i] == '/' || linea[i] == '+' || linea[i] == '-') {
                    if (prevChar != ' ') {
                        palabraDetectada[indicePD] = '\0';
                        _strlwr_s(palabraDetectada, TAMTOKEN);

                        // Verificar si la palabra detectada es un espacio en blanco
                        if (palabraDetectada[0] != ' ' && strlen(palabraDetectada) > 0) {
                            // Verificar si la palabra ya está en szPalabras
                            int encontrado = -1;
                            for (int k = 0; k < iNumElementos; k++) {
                                if (strcmp(szPalabras[k], palabraDetectada) == 0) {
                                    encontrado = k;
                                    break;
                                }
                            }

                            // Si la palabra ya existe, incrementar su contador
                            if (encontrado != -1) {
                                iEstadisticas[encontrado]++;
                            }
                            else {
                                strcpy_s(szPalabras[iNumElementos], TAMTOKEN, palabraDetectada);
                                iEstadisticas[iNumElementos] = 1;
                                iNumElementos++;
                            }
                        }

                        indicePD = 0;
                    }
                }
                else {
                    if (linea[i] != '.' || (i > 0 && linea[i - 1] == '/' && linea[i + 1] == '/')) {
                        palabraDetectada[indicePD] = linea[i];
                        indicePD++;
                    }
                }
                prevChar = linea[i];
            }

            if (indicePD > 0) {
                palabraDetectada[indicePD] = '\0';
                _strlwr_s(palabraDetectada, TAMTOKEN);

                int encontrado = -1;
                for (int k = 0; k < iNumElementos; k++) {
                    if (strcmp(szPalabras[k], palabraDetectada) == 0) {
                        encontrado = k;
                        break;
                    }
                }

                if (encontrado != -1) {
                    iEstadisticas[encontrado]++;
                }
                else {
                    strcpy_s(szPalabras[iNumElementos], TAMTOKEN, palabraDetectada);
                    iEstadisticas[iNumElementos] = 1;
                    iNumElementos++;
                }
            }
        }

        fclose(fpDicc);

        // Ordenar szPalabras al final del proceso
        for (i = 0; i < iNumElementos - 1; i++) {
            for (j = 0; j < iNumElementos - i - 1; j++) {
                if (strcmp(szPalabras[j], szPalabras[j + 1]) > 0) {
                    strcpy_s(temp, TAMTOKEN, szPalabras[j]);
                    strcpy_s(szPalabras[j], TAMTOKEN, szPalabras[j + 1]);
                    strcpy_s(szPalabras[j + 1], TAMTOKEN, temp);

                    // Ajustar también los valores correspondientes en iEstadisticas
                    int tempCount = iEstadisticas[j];
                    iEstadisticas[j] = iEstadisticas[j + 1];
                    iEstadisticas[j + 1] = tempCount;
                }
            }
        }
    }
}


/*****************************************************************************************************************
    ListaCandidatas: Esta funcion recupera desde el diccionario las palabras validas y su peso
    Regresa las palabras ordenadas por su peso
    char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
    int		iNumSugeridas,						//Lista de palabras clonadas
    char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
    int		iEstadisticas[],					//Lista de las frecuencias de las palabras
    int		iNumElementos,						//Numero de elementos en el diccionario
    char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
    int		iPeso[],							//Peso de las palabras en la lista final
    int &	iNumLista)							//Numero de elementos en la szListaFinal
******************************************************************************************************************/
void	ListaCandidatas(
    char	szPalabrasSugeridas[][TAMTOKEN],	//Lista de palabras clonadas
    int		iNumSugeridas,						//Lista de palabras clonadas
    char	szPalabras[][TAMTOKEN],				//Lista de palabras del diccionario
    int		iEstadisticas[],					//Lista de las frecuencias de las palabras
    int		iNumElementos,						//Numero de elementos en el diccionario
    char	szListaFinal[][TAMTOKEN],			//Lista final de palabras a sugerir
    int		iPeso[],							//Peso de las palabras en la lista final
    int& iNumLista)							//Numero de elementos en la szListaFinal
{
    int i, j, k;
    int temporal;
    iNumLista = 0;
    bool palabraExistente;//para saber si ya existe una palabra

    // Comparar szPalabras con szPalabrasSugeridas
    for (i = 0; i < iNumSugeridas; i++) {
        for (j = 0; j < iNumElementos; j++) {
            if (strcmp(szPalabrasSugeridas[i], szPalabras[j]) == 0) {
                // Verificar si la palabra ya está en szListaFinal
                palabraExistente = false;
                for (k = 0; k < iNumLista; k++) {
                    if (strcmp(szListaFinal[k], szPalabrasSugeridas[i]) == 0) {
                        palabraExistente = true;
                        break;
                    }
                }
                if (!palabraExistente) {
                    // Almacenar la palabra coincidente en szListaFinal
                    strcpy_s(szListaFinal[iNumLista], TAMTOKEN, szPalabrasSugeridas[i]);
                    // Almacenar la frecuencia correspondiente en iPeso
                    iPeso[iNumLista] = iEstadisticas[j];
                    iNumLista++;
                }
            }
        }
    }

    // Ordenar szListaFinal en orden de mayor a menor dependiendo el peso
    for (i = 0; i < iNumLista - 1; i++) {
        for (j = 0; j < iNumLista - i - 1; j++) {
            if (iPeso[j] < iPeso[j + 1]) {
                // Intercambio de las listas con metodo busbuja
                char szTemp[TAMTOKEN];
                strcpy_s(szTemp, TAMTOKEN, szListaFinal[j]);
                strcpy_s(szListaFinal[j], TAMTOKEN, szListaFinal[j + 1]);
                strcpy_s(szListaFinal[j + 1], TAMTOKEN, szTemp);
                // Se intercamnian los pesos
                temporal = iPeso[j];
                iPeso[j] = iPeso[j + 1];
                iPeso[j + 1] = temporal;
            }
        }
    }
}

/*****************************************************************************************************************
    ClonaPalabras: toma una palabra y obtiene todas las combinaciones y permutaciones requeridas por el metodo
    char *	szPalabraLeida,						// Palabra a clonar
    char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
    int &	iNumSugeridas)						//Numero de elementos en la lista
******************************************************************************************************************/
void	ClonaPalabras(
    char* szPalabraLeida,						// Palabra a clonar
    char	szPalabrasSugeridas[][TAMTOKEN], 	//Lista de palabras clonadas
    int& iNumSugeridas)						//Numero de elementos en la lista
{
    char alfabeto[] = "abcdefghijklmnñopqrstuvwxyzáéíóú"; //aqui estan todas las palabras que se insertaran y se cambiaran
    int longitud = strlen(szPalabraLeida);
    int i, j;
    iNumSugeridas = 0;
    char palabraModificada[TAMTOKEN]; // Para almacenar la palabra que se ira modificando y luego se ira guardando en la final
    char palabraAux[TAMTOKEN];

    // Almacenar la palabra original en szPalabrasSugeridas
    strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, szPalabraLeida);
    iNumSugeridas++;

    for (i = 0; i < strlen(szPalabraLeida); i++) {

        // Eliminar uno por uno de los caracteres de la palabra original
        for (j = 0; j < longitud; j++) {
            if (j != i) {
                palabraModificada[j - (j > i)] = szPalabraLeida[j];
            }
        }
        palabraModificada[longitud - 1] = '\0'; // Se agrega un nulo para terminar la palabra 

        // Copiar la palabra modificada a la matriz de palabras sugeridas
        strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, palabraModificada);
        iNumSugeridas++;
    }

    strcpy_s(palabraModificada, TAMTOKEN, szPalabraLeida);
    // Intercambia caracteres de par en par
    for (i = 0; i < longitud - 1; i++) {
        char temp = palabraModificada[i];
        palabraModificada[i] = palabraModificada[i + 1];
        palabraModificada[i + 1] = temp;

        strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, palabraModificada);
        strcpy_s(palabraModificada, TAMTOKEN, szPalabraLeida);
        iNumSugeridas++;
    }

    strcpy_s(palabraModificada, TAMTOKEN, szPalabraLeida);
    //sustituir el alfabeto en cada caracter
    for (i = 0; i < longitud; i++) {
        strcpy_s(palabraModificada, TAMTOKEN, szPalabraLeida);
        for (j = 0; j < strlen(alfabeto); j++) {
            palabraModificada[i] = alfabeto[j];
            strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, palabraModificada);
            iNumSugeridas++;
        }

    }

    //Ciclo que inserte el alfabeto entre cada espacio
    for (i = 0; i <= longitud; i++) {
        for (j = 0; j < strlen(alfabeto); j++) {
            // Se copian los caracteres
            char temp = szPalabraLeida[i];  // Guardar el carácter original en la posición i
            szPalabraLeida[i] = '\0';  // Colocar un nulo
            strcpy_s(palabraModificada, TAMTOKEN, szPalabraLeida);  
            szPalabraLeida[i] = temp;  // se restaura el caracter
            palabraModificada[i] = alfabeto[j];
            // caracteres despues del espacio
            strcpy_s(palabraModificada + i + 1, TAMTOKEN - i - 1, szPalabraLeida + i);
            strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, palabraModificada);
            iNumSugeridas++;
        }
    }


    // burbuja
    for (i = 0; i < iNumSugeridas - 1; i++) {
        for (j = 0; j < iNumSugeridas - i - 1; j++) {
            if (strcmp(szPalabrasSugeridas[j], szPalabrasSugeridas[j + 1]) > 0) {
                strcpy_s(palabraAux, TAMTOKEN, szPalabrasSugeridas[j]);
                strcpy_s(szPalabrasSugeridas[j], TAMTOKEN, szPalabrasSugeridas[j + 1]);
                strcpy_s(szPalabrasSugeridas[j + 1], TAMTOKEN, palabraAux);
            }
        }
    }

}

