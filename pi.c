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

int* hexOcta(char hex) {
    int * out = malloc(2*sizeof(int));
    int dec = hexIndex(hex);
    out[0] = dec/8;
    out[1] = dec%8;
    return out;
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
    out[1] = dec/4;
    out[2] = dec/2;
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
    FILE * fp;
    fopen_s(&fp, "one-million.txt", "r");
    FILE * csv;
    fopen_s(&csv, "out_count.csv", "w");
    fprintf(csv, "num,div,max,exact\n");
    int * counts = calloc(10, sizeof(int));
    int num = 0;
    char line [60];
    while(fgets(line, sizeof(line), fp)) {
        int i = 0;
        while(line[i] != '\n') {
            counts[line[i]-'0']++;
            i++;
            num++;
            if (num%10 == 0) {
                write(counts, 10, num,  csv);
            }
        }
    }
    fclose(fp);
    fclose(csv);
    fopen_s(&fp, "hex.txt", "r");
    FILE * csv8;
    FILE * csv4;
    FILE * csv2;
    fopen_s(&csv, "out_16.csv", "w");
    fopen_s(&csv8, "out_8.csv", "w");
    fopen_s(&csv4, "out_4.csv", "w");
    fopen_s(&csv2, "out_2.csv", "w");
    fprintf(csv, "num,div,max,exact\n");
    counts = calloc(16, sizeof(int));
    int * counts8 = calloc(8, sizeof(int));
    int * counts4 = calloc(4, sizeof(int));
    int * counts2 = calloc(2, sizeof(int));
    num = 0;
    while(fgets(line, sizeof(line), fp)) {
        int i = 0;
        while(line[i] != '\n') {
            counts[hexIndex(line[i])]++;
            int * conv = hexOcta(line[i]);
            for (int j = 1; j <= 2; j++) {
                counts8[conv[j-1]]++;
                if ((num+j)%8 == 0) {
                    write(counts8, 8, num*2+j, csv8);
                }
            }
            conv = hexQuad(line[i]);
            for (int j = 1; j <= 2; j++) {
                counts4[conv[j-1]]++;
                if ((num+j)%4 == 0) {
                    write(counts4, 4, num*4+j, csv4);
                }
            }
            conv = hexBin(line[i]);
            for (int j = 1; j <= 4; j++) {
                counts2[conv[j-1]]++;
                if ((num+j)%2 == 0) {
                    write(counts2, 2, num*2+j, csv2);
                }
            }
            i++;
            num++;
            if (num%16 == 0) {
                write(counts, 16, num, csv);
            }
        }
    }
    return 0;
}