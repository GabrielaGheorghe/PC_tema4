#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "include/project.h"

int *arrayProduct(int *array, int arrayLength) {


    int *g; // vectorul ce trebuie returnat
    g = (int*)malloc(arrayLength*sizeof(int)); // alocarea dinamica a vectorului returnat de functie
    int i; 
    int P = 1;

    for(i=0; i<arrayLength; i++){
        P = P*array[i]; // calculul produsului tuturor elementelor din functie
    }

    for(i=0; i<arrayLength; i++){
        g[i] = P/array[i]; /* impartirea fiecarui element al vectorului de returnat la elementul 
                            de pe pozitia respectiva din vectorul initial */
    }

    return g;
}

int **rotateMatrix(int **matrix, int matrixSize) {
    
  int **b; // declararea matricei ce trebuie returnata 
  int i, j;
  b = malloc(matrixSize * sizeof(int *)); 
  for (i = 0; i < matrixSize; i++) {          // alocarea dinamica a matricei de returnat
    b[i] = calloc(matrixSize, sizeof(int)); 
  }

  for(i = 0; i < matrixSize; i++)   // parcurgerea matricei initiale 
    for(j = 0; j < matrixSize; j++)
        b[i][j] = matrix[j][matrixSize - 1 - i]; /* conditia de intoarcere a fiecarui element al matricei
                                                    pe o pozitie cu 90 grade la stanga */

  return b;

}

int *subMatrixesSums(int **matrix, int queriesNo, int *queries) {

    int i, j, k; // declararea contorilor
    int p = 0; // declararea variabilei ce reprezinta pozitia din vectorul returnat a fiecarei sume
    int *S; // vectorul in care se stocheaza sumele elementelor fiecarei submatrice
    int e = 0; /* variabilele ce reprezinta pozitiile 
                coordonatelor submatricei din vectorul queries */
    int f = 2;
    int g = 1;
    int h = 3;


    S = malloc(queriesNo*sizeof(int*)); //alocarea dinamica a vectorului unde sunt stocate sumele 

    for(k = 0; k < queriesNo; k++) { //parcurgerea fiecarui query ce contine cate 4 coordonate
        S[p] = 0; // indentarea cu zero a fiecarei sume a elementelor submatricelor
        for(i = queries[e]; i <= queries[f]; i++){    // parcurgerea matricei initiale pana la coordonatele din queries    
            for(j = queries[g]; j <= queries[h]; j++){   
                S[p] = S[p] + matrix[i][j];  // calcului sumei elementelor fiecarei submatricei
            }      
        }
    e = e + 4; /* adaugarea a 4 pozitii pentru fiecare variabila pentru a se trece la al 
                doilea set de coordonate */
    f = f + 4;
    g = g + 4;
    h = h + 4;  
    p = p + 1;
    }
    return S;
}

TDriver *allocDriver(int ridesNo) {
    return NULL;
}

TDriver **allocDrivers(int driversNo, int *driversRidesNo) {
    return NULL;
}

TDriver **readDrivers(FILE *inputFile, int *driversNo) {
    return NULL;
}

void printDrivers(FILE *outputFile, TDriver **drivers, int driversNo) {
    return;
}

char *maxRatingDriverName(TDriver **drivers, int driversNo) {
    return NULL;
}

TDriver **getClosestDrivers(TDriver **drivers, int driversNo, double desiredX,
    double desiredY, int resultsNo) {
    return NULL;
}

void freeDriver(TDriver *driver) {
    return;
}

void freeDrivers(TDriver **drivers, int driversNo) {
    return;
}

// student GHEORGHE LUMINITA-GABRIELA, 312 CB