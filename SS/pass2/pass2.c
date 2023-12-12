#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void main() {
    char a[10], ad[10], label[10], opcode[10], operand[10], loc[10], pgname[10];
    char mnemonic[10], symbol[10], add[10], code[10];
    int i, locctr, len, actual_len;
    int start, final, length;
    FILE *fp1, *fp2, *fp3, *fp4, *fp5;

    fp1 = fopen("inter.c", "r");
    fscanf(fp1, "%s%s%s", pgname, opcode, operand);
    start = atoi(operand);

    while(strcmp(opcode, "END") != 0) {
        fscanf(fp1, "%s%s%s%s", loc, label, opcode, operand);
    }

    final = atoi(loc);
    length = final - start;
    printf("%d", length);
    fclose(fp1);

    fp1 = fopen("pass2out.c", "w");
    fp2 = fopen("outsym.c", "r");
    fp3 = fopen("inter.c", "r");
    fp4 = fopen("optab.c", "r");
    fp5 = fopen("objectphm.c", "w");

    fscanf(fp3, "%s%s%s", label, opcode, operand);

    if(strcmp(opcode, "START") == 0) {
        fprintf(fp1, "\t%s\t%s\t%s\n", label, opcode, operand);
        fscanf(fp3, "%d%s%s%s", &locctr, label, opcode, operand);
        fprintf(fp5, "H^%s^00%d^0000%d\n", pgname, start, length);
        fprintf(fp5, "T^00%d^%d^", start, length);
    }

    while(strcmp(opcode, "END") != 0) {
        if(strcmp(opcode, "BYTE") == 0) {
            fprintf(fp1, "%d\t%s\t%s\t%s\t", locctr, label, opcode, operand);
            len = strlen(operand);
            printf("%d\n", len);
            actual_len = len - 3;
            
            for(i = 2; i < (actual_len + 2); i++) {
                printf("%c", operand[i]);
                fprintf(fp1, "%x", operand[i]);
                fprintf(fp5, "%x", operand[i]);
            }
            
            fprintf(fp5, "^");
            fprintf(fp1, "\n");
        } else if(strcmp(opcode, "WORD") == 0) {
            len = strlen(operand);
            fprintf(fp1, "%d\t%s\t%s\t%s\t00000%d\n", locctr, label, opcode, operand, atoi(operand));
            fprintf(fp5, "00000%s^", operand);
        } else if((strcmp(opcode, "RESB") == 0) || (strcmp(opcode, "RESW") == 0)) {
            fprintf(fp1, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);
        } else {
            rewind(fp4);
            fscanf(fp4, "%s%s", mnemonic, code);

            while(strcmp(opcode, mnemonic) != 0)
                fscanf(fp4, "%s%s", mnemonic, code);

            if(strcmp(operand, " ") == 0) {
                fprintf(fp1, "%d\t%s\t%s\t%s\t%s0000\n", locctr, label, opcode, operand, code);
            } else {
                rewind(fp2);
                fscanf(fp2, "%s%s", symbol, add);

                while(strcmp(operand, symbol) != 0)
                    fscanf(fp2, "%s%s", symbol, add);

                fprintf(fp1, "%d\t%s\t%s\t%s\t%s%s\n", locctr, label, opcode, operand, code, add);
                fprintf(fp5, "%s%s^", code, add);
            }
        }
        fscanf(fp3, "%d%s%s%s\n", &locctr, label, opcode, operand);
    }

    fprintf(fp1, "%d\t%s\t%s\t%s\n", locctr, label, opcode, operand);
    fprintf(fp5, "\nE^00%d^", start);
    printf("FINISHED\n\n");
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    fclose(fp4);
    fclose(fp5);
}

