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
#define DEPURAR 1

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
    int indicePD;
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

            // Resetear el índice para una nueva palabra
            indicePD = 0;
            for (i = 0; i < strlen(linea); i++)
            {
                // Detectar una palabra
                if (linea[i] == ' ')
                {
                    // Verificar si la palabra detectada no está vacía
                    if (indicePD > 0) {
                        palabraDetectada[indicePD] = '\0';
                        _strlwr_s(palabraDetectada, TAMTOKEN);
                        strcpy_s(szPalabras[iNumElementos], TAMTOKEN, palabraDetectada);
                        iNumElementos++;
                        indicePD = 0; // Reiniciar el índice para la próxima palabra
                    }
                }
                else
                {
                    // Si no es un carácter especial, añadirlo a la palabra detectada
                    if (linea[i] != '(' && linea[i] != ')' && linea[i] != ',' && linea[i] != ';' && linea[i] != '.' && linea[i] != '\t' && linea[i] != '\n') {
                        palabraDetectada[indicePD] = linea[i];
                        indicePD++;
                    }
                }
            }

            // Al finalizar una línea, verificar si queda una palabra por agregar
            if (indicePD > 0) {
                palabraDetectada[indicePD] = '\0';
                _strlwr_s(palabraDetectada, TAMTOKEN);
                strcpy_s(szPalabras[iNumElementos], TAMTOKEN, palabraDetectada);
                iNumElementos++;
            }

            // Resto del código...
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
            int indiceNuevo = 0;
            for (i = 0; i < iNumElementos; i++) {
                if (i == 0 || strcmp(szPalabras[i], szPalabras[i - 1]) != 0) {
                    // Si es la primera aparición o una palabra diferente a la anterior
                    int contador = 1;
                    // Verificar las apariciones siguientes para la misma palabra
                    for (j = i + 1; j < iNumElementos; j++) {
                        if (strcmp(szPalabras[i], szPalabras[j]) == 0) {
                            contador++;
                        }
                    }
                    iEstadisticas[indiceNuevo] = contador;
                    strcpy_s(szPalabras[indiceNuevo], TAMTOKEN, szPalabras[i]);

                    indiceNuevo++;
                }
            }

            // Actualizar el número de elementos en la matriz y los contadores
            iNumElementos = indiceNuevo;

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

    //Sustituya estas lineas por su código
    strcpy(szListaFinal[0], szPalabrasSugeridas[0]); //la palabra candidata
    iPeso[0] = iEstadisticas[0];			// el peso de la palabra candidata

    iNumLista = 1;							//Una sola palabra candidata
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
    char alfabeto[] = "abcdefghijklmnopqrstuvwxyzáéíóú";
    int longitud = strlen(szPalabraLeida);
    int i, j, k;
    iNumSugeridas = 0;
    char palabraModificada[TAMTOKEN]; // Para almacenar la palabra modificada

    // Almacenar la palabra original en szPalabrasSugeridas
    strcpy_s(szPalabrasSugeridas[iNumSugeridas],TAMTOKEN, szPalabraLeida);
    iNumSugeridas++;

    for (i = 0; i < strlen(szPalabraLeida); i++) {

        // Eliminar el i-ésimo carácter de la palabra original
        for (j = 0; j < longitud; j++) {
            if (j != i) {
                palabraModificada[j - (j > i)] = szPalabraLeida[j];
            }
        }
        palabraModificada[longitud - 1] = '\0'; // Agregar el terminador nulo al final de la nueva palabra

        // Copiar la palabra modificada a la matriz de palabras sugeridas
        strcpy_s(szPalabrasSugeridas[iNumSugeridas],TAMTOKEN, palabraModificada);
        iNumSugeridas++;
    }

    strcpy_s(palabraModificada,TAMTOKEN, szPalabraLeida);
    // Intercambia caracteres de par en par
    for (int i = 0; i < longitud - 1; i++) {
        char temp = palabraModificada[i];
        palabraModificada[i] = palabraModificada[i + 1];
        palabraModificada[i + 1] = temp;

        strcpy_s(szPalabrasSugeridas[iNumSugeridas],TAMTOKEN, palabraModificada);
        strcpy_s(palabraModificada,TAMTOKEN, szPalabraLeida);
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
    
    strcpy_s(palabraModificada, TAMTOKEN, szPalabraLeida);
    // Insertar el alfabeto completo en cada uno de los espacios
    for (i = 0; i < longitud; i++) {
        k = 0;
        for (j = 0; j < strlen(alfabeto); j++) {
            palabraModificada[i] = alfabeto[j];
            
            while (szPalabraLeida[i] != '\0' && k < longitud) {
                palabraModificada[i + 1] = szPalabraLeida[i];
            }
            strcpy_s(szPalabrasSugeridas[iNumSugeridas], TAMTOKEN, palabraModificada);
            iNumSugeridas++;
        }
    }

}