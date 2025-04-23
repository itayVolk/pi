/* 
   Author: Itay Volk
   Date: 04/23/2025
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main() {
    FILE * fp;
    fopen_s(&fp, "one-million.txt", "r");
    FILE * csv;
    fopen_s(&csv, "out.csv", "w");
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
                double sum = 0;
                double max = 0;
                int exact = 0;
                for(int j = 0; j < 10; j++) {
                    sum += (counts[j]-num/10.0)*(counts[j]-num/10.0);
                    if (abs(counts[j]-num/10) > max) {
                        max = abs(counts[j]-num/10);
                    }
                    exact += counts[j] == num/10;
                }
                max /= num/10.0;
                sum = sqrt(sum*10)/num;
                fprintf(csv, "%d,%lf,%lf,%d\n", num, sum, max, exact);
            }
        }
    }
    return 0;
}