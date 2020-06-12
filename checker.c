/* Return codes interpretation
 * 139 - Segmentation Fault
 *   0 - Success
 *  -1 - Checker Failure
 *  -2 - Part Two Task Two/Three Failed
 *  -3 - Part Two Task Five Failed
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/project.h"

#define INPUT_FILE_EXTENSION_SIZE 7
#define OUTPUT_FILE_EXTENSION_SIZE 8
#define OUTPUT_TASK_FILE_EXTENSION_SIZE 2

void freeMatrix(int **matrix, int matrixSize) {
    int i;
    for (i = 0; i < matrixSize; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int checkPartOneTaskOne(FILE *outputTextFile, FILE *inputTextFile) {
    int arrayLength, *array;

    fscanf(inputTextFile, "%d", &arrayLength);

    array = calloc(arrayLength, sizeof(int));
    if (!array) {
        return -1;
    }

    int i;
    for (i = 0; i < arrayLength; i++) {
        fscanf(inputTextFile, "%d", &array[i]);
    }

    int *products = arrayProduct(array, arrayLength);

    for (i = 0; i < arrayLength; i++) {
        fprintf(outputTextFile, "%d ", products[i]);
    }
    fprintf(outputTextFile, "\n");

    free(array);
    free(products);
    return 0;
}

int checkPartOneTaskTwo(FILE *outputTextFile, FILE *inputTextFile) {
    int matrixSize, **matrix;

    fscanf(inputTextFile, "%d", &matrixSize);

    matrix = calloc(matrixSize, sizeof(int *));
    if (!matrix) {
        return -1;
    }

    int i, j;
    for (i = 0; i < matrixSize; i++) {
        matrix[i] = calloc(matrixSize, sizeof(int));
        if (!matrix[i]) {
            freeMatrix(matrix, i);
            return -1;
        }
    }

    for (i = 0; i < matrixSize; i++) {
        for (j = 0; j < matrixSize; j++) {
            fscanf(inputTextFile, "%d", &matrix[i][j]);
        }
    }

    int **rotatedMatrix = rotateMatrix(matrix, matrixSize);

    for (i = 0; i < matrixSize; i++) {
        for (j = 0; j < matrixSize; j++) {
            fprintf(outputTextFile, "%d ", rotatedMatrix[i][j]);
        }
        fprintf(outputTextFile, "\n");
    }

    if (matrix != rotatedMatrix) {
        freeMatrix(rotatedMatrix, matrixSize);
    }
    freeMatrix(matrix, matrixSize);


    return 0;
}

int checkPartOneTaskThree(FILE *outputTextFile, FILE *inputTextFile) {
    int linesNo, colsNo;
    fscanf(inputTextFile, "%d %d", &linesNo, &colsNo);

    int **matrix = calloc(linesNo, sizeof(int *));
    if (!matrix) {
        return -1;
    }

    int i, j;
    for (i = 0; i < linesNo; i++) {
        matrix[i] = calloc(colsNo, sizeof(int));
        if (!matrix[i]) {
            freeMatrix(matrix, i);
            return -1;
        }
    }

    for (i = 0; i < linesNo; i++) {
        for (j = 0; j < colsNo; j++) {
            fscanf(inputTextFile, "%d", &matrix[i][j]);
        }
    }

    int queriesNo;
    fscanf(inputTextFile, "%d", &queriesNo);

    int *queries = calloc(queriesNo * 4, sizeof(int));
    if (!queries) {
        freeMatrix(matrix, linesNo);
        return -1;
    }

    for (i = 0; i < queriesNo; i++) {
        fscanf(inputTextFile, "%d %d %d %d", &queries[i * 4],
            &queries[i * 4 + 1], &queries[i * 4 + 2], &queries[i * 4 + 3]);
    }

    int *sums = subMatrixesSums(matrix, queriesNo, queries);

    for (i = 0; i < queriesNo; i++) {
        fprintf(outputTextFile, "%d ", sums[i]);
    }
    freeMatrix(matrix, linesNo);
    free(queries);
    free(sums);
    return 0;
}

int checkPartOne(char *outputTextFileName, char *inputTextFileName) {
    FILE *inputTextFile = fopen(inputTextFileName, "rt");
    if (!inputTextFile) {
        return -1;
    }
    FILE *outputTextFile = fopen(outputTextFileName, "wt");
    if (!outputTextFile) {
        fclose(inputTextFile);
        return -1;
    }

    if (checkPartOneTaskOne(outputTextFile, inputTextFile) == -1) {
        fclose(inputTextFile);
        fclose(outputTextFile);
        return -1;
    }

    fprintf(outputTextFile, "\n");

    if (checkPartOneTaskTwo(outputTextFile, inputTextFile) == -1) {
        fclose(inputTextFile);
        fclose(outputTextFile);
        return -1;
    }

    fprintf(outputTextFile, "\n");

    if (checkPartOneTaskThree(outputTextFile, inputTextFile) == -1) {
        fclose(inputTextFile);
        fclose(outputTextFile);
        return -1;
    }

    fprintf(outputTextFile, "\n");

    fclose(inputTextFile);
    fclose(outputTextFile);

    return 0;
}

int checkPartTwoTasksFourAndFive(char *outputBinaryFileName,
    char *inputBinaryFileName, TDriver ***drivers, int *driversNo) {
    FILE *inputBinaryFile = fopen(inputBinaryFileName, "rb");
    if (!inputBinaryFile) {
        return -1;
    }

    *drivers = readDrivers(inputBinaryFile, driversNo);

    fclose(inputBinaryFile);

    if (!(*drivers)) {
        return -2;
    }

    int i, j;
    for (i = 0; i < *driversNo; i++) {
        if (!((*drivers)[i])) {
            for (j = 0; j < *driversNo; j++) {
                if ((*drivers)[j]) {
                    free((*drivers)[j]);
                }
            }
            free(*drivers);
            return -2;
        }
    }

    char *outputBinaryTask5FileName= calloc(strlen(outputBinaryFileName)
        + OUTPUT_TASK_FILE_EXTENSION_SIZE + 1, sizeof(char));
    if (!outputBinaryTask5FileName) {
        return -1;
    }

    sprintf(outputBinaryTask5FileName, "%s.5", outputBinaryFileName);
    FILE *outputBinaryTask5File = fopen(outputBinaryTask5FileName, "wb");
    if (!outputBinaryTask5File) {
        free(outputBinaryTask5FileName);
        return -1;
    }

    printDrivers(outputBinaryTask5File, *drivers, *driversNo);

    fclose(outputBinaryTask5File);
    free(outputBinaryTask5FileName);

    return 0;
}

int checkPartTwoTaskSix(char *outputBinaryFileName, TDriver **drivers,
    int driversNo) {

    char *highestRatingDriver = maxRatingDriverName(drivers, driversNo);
    if (!highestRatingDriver) {
        return -3;
    }

    char *outputBinaryTask6FileName= calloc(strlen(outputBinaryFileName)
        + OUTPUT_TASK_FILE_EXTENSION_SIZE + 1, sizeof(char));
    if (!outputBinaryTask6FileName) {
        return -1;
    }

    sprintf(outputBinaryTask6FileName, "%s.6", outputBinaryFileName);
    FILE *outputBinaryTask6File = fopen(outputBinaryTask6FileName, "wb");
    if (!outputBinaryTask6File) {
        free(outputBinaryTask6FileName);
        return -1;
    }

    fwrite(highestRatingDriver, sizeof(char), strlen(highestRatingDriver),
        outputBinaryTask6File);

    fclose(outputBinaryTask6File);
    free(outputBinaryTask6FileName);

    return 0;
}

int checkPartTwoTaskSeven(char *outputBinaryFileName,
    char *checkerInputFileName, TDriver **drivers, int driversNo) {
    FILE *checkerInputFile = fopen(checkerInputFileName, "rt");
    if (!checkerInputFile) {
        return -1;
    }

    double desiredX, desiredY;
    int resultsNo;

    fscanf(checkerInputFile, "%lf %lf %d", &desiredX, &desiredY, &resultsNo);

    fclose(checkerInputFile);

    TDriver **closestDrivers = getClosestDrivers(drivers, driversNo, desiredX,
        desiredY, resultsNo);

    char *outputBinaryTask7FileName= calloc(strlen(outputBinaryFileName)
        + OUTPUT_TASK_FILE_EXTENSION_SIZE + 1, sizeof(char));
    if (!outputBinaryTask7FileName) {
        return -1;
    }

    sprintf(outputBinaryTask7FileName, "%s.7", outputBinaryFileName);
    FILE *outputBinaryTask7File = fopen(outputBinaryTask7FileName, "wb");
    if (!outputBinaryTask7File) {
        free(closestDrivers);
        free(outputBinaryTask7FileName);
        return -1;
    }

    printDrivers(outputBinaryTask7File, closestDrivers, resultsNo);

    fclose(outputBinaryTask7File);
    free(outputBinaryTask7FileName);
    free(closestDrivers);
    return 1;
}

int checkPartTwo(char *outputBinaryFileName, char *inputBinaryFileName,
    char *checkerInputFileName) {

    int errorCode, driversNo;
    TDriver **drivers;

    if ((errorCode = checkPartTwoTasksFourAndFive(outputBinaryFileName,
        inputBinaryFileName, &drivers, &driversNo)) < 0) {
        return errorCode;
    }

    if ((errorCode = checkPartTwoTaskSix(outputBinaryFileName, drivers,
        driversNo)) < 0) {
        freeDrivers(drivers, driversNo);
        return errorCode;
    }

    if ((errorCode = checkPartTwoTaskSeven(outputBinaryFileName,
        checkerInputFileName, drivers, driversNo)) < 0) {
        freeDrivers(drivers, driversNo);
        return errorCode;
    }

    freeDrivers(drivers, driversNo);

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Incorrect number of arguments. Please run program as "
            "./homework4 test_<test_number>\n");
        return 0;
    }

    char *fileName = strdup(argv[1]);
    if (!fileName) {
        printf("-1");
        return 0;
    }

    char *inputTextFileName =
        calloc(strlen(fileName) + INPUT_FILE_EXTENSION_SIZE + 1, sizeof(char));
    if (!inputTextFileName) {
        free(fileName);
        printf("-1");
        return 0;
    }

    char *inputBinaryFileName =
        calloc(strlen(fileName) + INPUT_FILE_EXTENSION_SIZE + 1, sizeof(char));
    if (!inputBinaryFileName) {
        free(fileName);
        free(inputTextFileName);
        printf("-1");
        return 0;
    }

    char *checkerInputFileName =
        calloc(strlen(fileName) + INPUT_FILE_EXTENSION_SIZE + 1, sizeof(char));
    if (!checkerInputFileName) {
        free(fileName);
        free(inputTextFileName);
        free(inputBinaryFileName);
        printf("-1");
        return 0;
    }

    char *outputTextFileName =
        calloc(strlen(fileName) + OUTPUT_FILE_EXTENSION_SIZE + 1, sizeof(char));
    if (!outputTextFileName) {
        free(fileName);
        free(inputTextFileName);
        free(inputBinaryFileName);
        free(checkerInputFileName);
        printf("-1");
        return 0;
    }

    char *outputBinaryFileName =
        calloc(strlen(fileName) + OUTPUT_FILE_EXTENSION_SIZE + 1, sizeof(char));
    if (!outputBinaryFileName) {
        free(fileName);
        free(inputTextFileName);
        free(inputBinaryFileName);
        free(checkerInputFileName);
        free(outputTextFileName);
        printf("-1");
        return 0;
    }

    sprintf(inputTextFileName, "%s.txt.in", fileName);
    sprintf(inputBinaryFileName, "%s.bin.in", fileName);
    sprintf(checkerInputFileName, "%s.ckr.in", fileName);
    sprintf(outputTextFileName, "%s.txt.out", fileName);
    sprintf(outputBinaryFileName, "%s.bin.out", fileName);

    free(fileName);

    if (checkPartOne(outputTextFileName, inputTextFileName) == -1) {
        free(inputTextFileName);
        free(inputBinaryFileName);
        free(checkerInputFileName);
        free(outputTextFileName);
        free(outputBinaryFileName);
        printf("-1");
        return 0;
    }

    int errorCode;
    if ((errorCode = checkPartTwo(outputBinaryFileName, inputBinaryFileName,
        checkerInputFileName)) < 0) {
        free(inputTextFileName);
        free(inputBinaryFileName);
        free(checkerInputFileName);
        free(outputTextFileName);
        free(outputBinaryFileName);
        printf("%d", errorCode);
        return errorCode;
    }

    free(inputTextFileName);
    free(inputBinaryFileName);
    free(checkerInputFileName);
    free(outputTextFileName);
    free(outputBinaryFileName);
    printf("0");
    return 0;
}
