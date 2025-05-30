#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265358979323846
#define MAX_VARIANTS 3

typedef struct 
{
    double T;   
    double dt;   
    double L0;   
    double k0;     
    double C0;      
    double m;       
    double n;       
} Variant;

double calculate_k(double t, double T, double k0, double m) 
{
    if (t <= T/4.0) 
    {
        return k0 * (1.0 + exp(-m * t));
    } 
    else 
    {
        return k0 * (1.0 + exp(-m * T/4.0));
    }
}

double calculate_L(double t, double T, double L0, double k) 
{
    if (t <= T/2.0) 
    {
        return L0 * (1.0 - exp(-k * t));
    } 
    else 
    {
        return L0 * (1.0 - exp(-k * T/2.0));
    }
}

double calculate_C(double t, double T, double C0, double n) 
{
    if (t <= T/3.0) 
    {
        return C0 * (1.0 - exp(-n * t));
    } 
    else 
    {
        return C0 * (1.0 - exp(-n * T/3.0));
    }
}

double calculate_Tk(double L, double C) 
{
    return 2.0 * PI * sqrt(L * C);
}

int read_input_data(const char* filename, Variant variants[], int max_variants) 
{
    FILE *input_file = fopen(filename, "r");
    if (input_file == NULL) {
        printf("Помилка відкриття вхідного файлу %s!\n", filename);
        return 0;
    }

    int count = 0;
    while (count < max_variants && 
           fscanf(input_file, "%lf %lf %lf %lf %lf %lf %lf", 
                 &variants[count].T, &variants[count].dt, &variants[count].L0,
                 &variants[count].k0, &variants[count].C0, &variants[count].m,
                 &variants[count].n) == 7) {
        count++;
    }

    fclose(input_file);
    return count;
}

int main() 
{
    system("chcp 65001");

    Variant variants[MAX_VARIANTS];

    int num_variants = read_input_data("inputdata.txt", variants, MAX_VARIANTS);
    if (num_variants == 0) 
    {
        printf("Не вдалося зчитати дані або файл порожній.\n");
        return 1;
    }

    FILE *output_file = fopen("outputdata.txt", "w");
    if (output_file == NULL) 
    {
        printf("Помилка відкриття файлу для запису результатів!\n");
        return 1;
    }
    
    fprintf(output_file, "Результати розрахунків періоду власних коливань контура:\n\n");

    for (int i = 0; i < num_variants; i += 1) 
    {
        Variant var = variants[i];
        fprintf(output_file, "Варіант %d:\n", i+1);
        fprintf(output_file, "T = %.2f c, Δt = %.2f c, L0 = %.2e Гн, k0 = %.1f, C0 = %.2e Ф, m = %.1f, n = %.1f\n",
                var.T, var.dt, var.L0, var.k0, var.C0, var.m, var.n);
        fprintf(output_file, "t (c)\tL (Гн)\t\tC (Ф)\t\tTk (c)\n");

        for (double t = 0.0; t <= var.T; t += var.dt) 
        {
            double k = calculate_k(t, var.T, var.k0, var.m);
            double L = calculate_L(t, var.T, var.L0, k);
            double C = calculate_C(t, var.T, var.C0, var.n);
            double Tk = calculate_Tk(L, C);
            
            fprintf(output_file, "%.2f\t%.3e\t%.3e\t%.3e\n", t, L, C, Tk);
        }
        fprintf(output_file, "\n");
    }
    
    fclose(output_file);
    printf("Розрахунки завершено. Результати записано у файл 'outputdata.txt'\n");
    
    return 0;
}