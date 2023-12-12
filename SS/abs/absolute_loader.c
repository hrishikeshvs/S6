#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

FILE *objectCodeFile;
FILE *outputFile;
char line[100];
char programName[100], programLength[10], startingAddress[10], objectCodeLength[5], objectCodeStart[10];
int previousAddress = 0;

void main() {
    objectCodeFile = fopen("input.txt", "r");
    outputFile = fopen("output1.txt", "w"); 

    if (objectCodeFile == NULL || outputFile == NULL) {
        printf("\nError in opening files");
    } else {
        while (!feof(objectCodeFile)) {
            fscanf(objectCodeFile, "%s", line);
            switch (line[0]) {
                case 'H':
                    {
                        int i = 1, j = 0, k = 0, l = 0, flag = 0;
                        while (line[i] != '$') {
                            if (line[i] == '^') {
                                i++;
                                flag++;
                                continue;
                            }
                            if (flag == 1)
                                programName[j++] = line[i++];
                            else if (flag == 2)
                                startingAddress[k++] = line[i++];
                            else
                                programLength[l++] = line[i++];
                        }
                        fprintf(outputFile, "\nName of Program=%s", programName);
                        fprintf(outputFile, "\nStarting Address=%s", startingAddress);
                        fprintf(outputFile, "\nLength of Program=%s\n", programLength);
                        previousAddress = atoi(startingAddress);
                        break;
                    }
                case 'T':
                    {
                        int i = 1, j = 0, k = 0, flag = 0, codeLength = 0, currentAddress;
                        char code[3] = "";
                        while (line[i] != '$') {
                            if (line[i] == '^') {
                                if (flag == 2) {
                                    currentAddress = atoi(objectCodeStart);
                                    if (currentAddress > previousAddress) {
                                        do {
                                            fprintf(outputFile, "%d\t%s\n", previousAddress, "XX");
                                        } while (++previousAddress < currentAddress);
                                    }
                                }
                                i++;
                                flag++;
                                continue;
                            }
                            if (flag == 1)
                                objectCodeStart[j++] = line[i++];
                            else if (flag == 2)
                                objectCodeLength[k++] = line[i++];
                            else {
                                code[codeLength++] = line[i++];
                                if (codeLength == 2) {
                                    fprintf(outputFile, "%d\t%s\n", currentAddress, code);
                                    currentAddress++;
                                    previousAddress++;
                                    codeLength = 0;
                                }
                            }
                        }
                        break;
                    }
            }
        }
    }

    fclose(outputFile);  
   
}

