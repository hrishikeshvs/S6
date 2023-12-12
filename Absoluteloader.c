#include <stdio.h>
#include <stdlib.h>

FILE *obcode, *outfile;
char read[100], pname[100], plength[100], saddress[100], oblength[100], obstart[100];
int oprev = 0;

void fill(int osaddress, int oprev) {
    do {
        fprintf(outfile, "%d\tXX\n", oprev);
    } while (++oprev < osaddress);
}

void scantext() {
    int i = 1, j = 0, k = 0, cl = 0, osaddress, flag = 0;
    char code[3] = "";

    while (read[i] != '$') {
        if (read[i] == '^') {
            if (flag == 2) {
                osaddress = atoi(obstart);
                if (osaddress > oprev) {
                    fill(osaddress, oprev);
                    oprev = osaddress;
                }
            }
            i++;
            flag++;
            continue;
        }
        if (flag == 1)
            obstart[j++] = read[i++];
        else if (flag == 2)
            oblength[k++] = read[i++];
        else {
            code[cl++] = read[i++];
            if (cl == 2) {
                fprintf(outfile, "%d\t%s\n", osaddress, code);
                osaddress++;
                oprev++;
                cl = 0;
            }
        }
    }
}

void scanhead() {
    int i = 1, j = 0, k = 0, l = 0, flag = 0;

    while (read[i] != '$') {
        if (read[i] == '^') {
            i++;
            flag++;
            continue;
        }
        if (flag == 1)
            pname[j++] = read[i++];
        else if (flag == 2)
            saddress[k++] = read[i++];
        else
            plength[l++] = read[i++];
    }
}

int main() {
    obcode = fopen("inputab.txt", "r");
    outfile = fopen("outab.txt", "w");

    if (obcode == NULL)
        printf("INPUT FILE IS EMPTY\n");
    else {
        while (fscanf(obcode, "%s", read) != EOF) {
            switch (read[0]) {
                case 'H':
                    scanhead();
                    fprintf(outfile, "\nPROGRAM NAME:%s", pname);
                    fprintf(outfile, "\nSTARTING ADDRESS:%s", saddress);
                    fprintf(outfile, "\nPROGRAM LENGTH:%s\n", plength);
                    oprev = atoi(saddress);
                    break;
                case 'T':
                    scantext();
                    break;
            }
        }
        fclose(obcode);
        fclose(outfile);
    }

    return 0;
}
