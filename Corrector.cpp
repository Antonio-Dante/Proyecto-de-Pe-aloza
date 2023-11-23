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
void	Diccionario(char* szNombre, char szPalabras[][TAMTOKEN], int iEstadisticas[], int& iNumElementos)
{
    FILE* fpDicc;
    char linea[4000];
    char palabraDetectada[TAMTOKEN];
    int i, j;
    int indicePD, indice2=0;
    iNumElementos = 0;
    char auxiliar[TAMTOKEN];

    // abrir el achivo
    if (DEPURAR == 1)
        printf("%s", szNombre);

    fopen_s(&fpDicc, szNombre, "r");
    if (fpDicc != NULL)
    {
        if (DEPURAR == 1)
            printf("\nSi lo pude abrir");

        while (!feof(fpDicc))
        {
            fgets(linea, sizeof(linea), fpDicc);
            if (DEPURAR == 1)
                printf("\n%s\n", linea);

            // Aqui se inicializa el indice para la primera palabra
            indicePD = 0;
            for (i = 0; i < strlen(linea); i++)
            {
                // Se detecta una palabra
                if (linea[i] == ' ')
                {
                    // Verificar si la palabra detectada no está vacía
                    if (indicePD > 0) {
                        palabraDetectada[indicePD] = '\0';
                        _strlwr_s(palabraDetectada, TAMTOKEN); //se vuelven minusculas
                        strcpy_s(szPalabras[iNumElementos], TAMTOKEN, palabraDetectada);
                        iNumElementos++;
                        indicePD = 0; // Se reinicia el índice para la proxima palabra
                    }
                }
                else
                {
                    // Se quitan parentesis, comas, puntos y comas, salto de linea y tabuladores
                    if (linea[i] != '(' && linea[i] != ')' && linea[i] != ',' && linea[i] != ';' && linea[i] != '.' && linea[i] != '\t' && linea[i] != '\n') {
                        palabraDetectada[indicePD] = linea[i];
                        indicePD++;
                    }
                }
            }

            // 
            if (indicePD > 0) {
                palabraDetectada[indicePD] = '\0';
                _strlwr_s(palabraDetectada, TAMTOKEN);
                strcpy_s(szPalabras[iNumElementos], TAMTOKEN, palabraDetectada);
                iNumElementos++;
            }

            // burbujazo
            for (i = 0; i < iNumElementos; i++) {
                for (j = 0; j < iNumElementos; j++) {
                    if (strcmp(szPalabras[j], szPalabras[j + 1]) > 0) {
                        strcpy_s(auxiliar, TAMTOKEN, szPalabras[j]);
                        strcpy_s(szPalabras[j], TAMTOKEN, szPalabras[j + 1]);
                        strcpy_s(szPalabras[j + 1], TAMTOKEN, auxiliar);
                    }
                }
            }

            //Contador de palabras
            for (i = 0; i < iNumElementos; i++) {
                if (i == 0 || strcmp(szPalabras[i], szPalabras[i - 1]) != 0) {
                    // Se inicia en 1 si es la primera vez que aparece
                    int contador = 1;
                    // comparar si existe la palabra si no se aumenta el contador
                    for (j = i + 1; j < iNumElementos; j++) {
                        if (strcmp(szPalabras[i], szPalabras[j]) == 0) {
                            contador++;
                        }
                    }
                    iEstadisticas[indice2] = contador;
                    strcpy_s(szPalabras[indice2], TAMTOKEN, szPalabras[i]);

                    indice2++;
                }
            }
            iNumElementos = indice2;
        }

        fclose(fpDicc);
    }
    else
    {
        if (DEPURAR == 1)
            printf("\nNo lo pude abrir");
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
    int i, j, k;
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
    for (int i = 0; i < longitud - 1; i++) {
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
            strncpy(palabraModificada, szPalabraLeida, i);
            palabraModificada[i] = alfabeto[j];
            // caracteres despues del espacio
            strcpy(palabraModificada + i + 1, szPalabraLeida + i);
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