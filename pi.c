/* 
   Author: Itay Volk
   Date: 04/23/2025
*/

#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int hexIndex(char hex) {
    return isdigit(hex) ? hex-'0' : hex-'A'+10;
}

int* hexQuad(char hex) {
    int * out = malloc(2*sizeof(int));
    int dec = hexIndex(hex);
    out[0] = dec/4;
    out[1] = dec%4;
    return out;
}

int* hexBin(char hex) {
    int * out = malloc(4*sizeof(int));
    int dec = hexIndex(hex);
    out[0] = dec/8;
    out[1] = dec%8/4;
    out[2] = dec%4/2;
    out[3] = dec%2;
    return out;
}

void write(int * counts, int count, int num, FILE * csv) {
    double sum = 0;
    double max = 0;
    int exact = 0;
    for(int j = 0; j < count; j++) {
        sum += (counts[j]-num/(double)(count))*(counts[j]-num/(double)(count));
        if (abs(counts[j]-num/count) > max) {
            max = abs(counts[j]-num/count);
        }
        exact += counts[j] == num/count;
    }
    max /= num/(double)(count);
    sum = sqrt(sum*count)/num;
    fprintf(csv, "%d,%lf,%lf,%d\n", num, sum, max, exact);
}

int main() {
    FILE * fp10;
    fopen_s(&fp10, "one-million.txt", "r");
    FILE * csv10;
    fopen_s(&csv10, "out_10.csv", "w");
    fprintf(csv10, "num,div,max,exact\n");
    int * counts = calloc(10, sizeof(int));
    int num = 0;
    char line [100];
    while(fgets(line, sizeof(line), fp10)) {
        int i = 0;
        while(line[i] != '\n') {
            counts[line[i]-'0']++;
            i++;
            num++;
            if (num%10 == 0) {
                write(counts, 10, num,  csv10);
            }
        }
    }
    fclose(fp10);
    fclose(csv10);
    FILE * fp16;
    fopen_s(&fp16, "hex.txt", "r");
    FILE * csv16;
    FILE * csv4;
    FILE * csv2;
    fopen_s(&csv16, "out_16.csv", "w");
    fopen_s(&csv4, "out_4.csv", "w");
    fopen_s(&csv2, "out_2.csv", "w");
    fprintf(csv16, "num,div,max,exact\n");
    fprintf(csv4, "num,div,max,exact\n");
    fprintf(csv2, "num,div,max,exact\n");
    counts = calloc(16, sizeof(int));
    int * counts4 = calloc(4, sizeof(int));
    int * counts2 = calloc(2, sizeof(int));
    num = 0;
    while(fgets(line, sizeof(line), fp16)) {
        int i = 0;
        while(line[i] != '\n') {
            counts[hexIndex(line[i])]++;
            int * conv = hexQuad(line[i]);
            for (int j = 0; j < 2; j++) {
                counts4[conv[j]]++;
                if ((num*2+j+1)%4 == 0) {
                    write(counts4, 4, num*2+j+3, csv4);
                }
            }
            conv = hexBin(line[i]);
            for (int j = 0; j < 4; j++) {
                counts2[conv[j]]++;
                if (j%2) {
                    write(counts2, 2, num*4+j+1, csv2);
                }
            }
            i++;
            num++;
            if (num%16 == 0) {
                write(counts, 16, num, csv16);
            }
        }
    }
    fclose(fp16);
    fclose(csv16);
    fclose(csv4);
    fclose(csv2);
    return 0;
}