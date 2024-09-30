#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Function to convert a number from a given base to decimal
long long int base_to_decimal(const char* value, int base) {
    long long int result = 0;
    int len = strlen(value);
    for (int i = 0; i < len; i++) {
        char c = value[len - 1 - i];
        int digit;
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        } else {
            digit = c - 'A' + 10; // Assume uppercase for hex
        }
        result += digit * pow(base, i);
    }
    return result;
}

// Function to calculate the constant term of the polynomial using Lagrange Interpolation
double lagrange_interpolation(long long int x[], long long int y[], int n, long long int x_value) {
    double result = 0.0;

    for (int i = 0; i < n; i++) {
        double term = y[i];
        for (int j = 0; j < n; j++) {
            if (j != i) {
                term *= (double)(x_value - x[j]) / (x[i] - x[j]);
            }
        }
        result += term;
    }
    return result;
}

// Function to read JSON data from a file and decode the Y values
void read_json_data(const char* filename, long long int x[], long long int y[], int* count) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }

    char line[256];
    
    // Reading the JSON data (simplified)
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "\"base\":") != NULL) {
            // Extract base
            char* base_str = strtok(line, ":");
            base_str = strtok(NULL, ",");
            int base = atoi(base_str);
            fgets(line, sizeof(line), file); // Read next line for value
            char* value_str = strtok(line, ":");
            value_str = strtok(NULL, ",");
            value_str++; // Skip the first quote
            value_str[strlen(value_str) - 2] = '\0'; // Remove trailing quote

            long long int decoded_value = base_to_decimal(value_str, base);
            x[*count] = atoi(strtok(line - 1, "\""));
            y[*count] = decoded_value;
            (*count)++;
        }
    }
    fclose(file);
}

// Function to identify wrong points
void find_wrong_points(long long int x[], long long int y[], int n, double secret) {
    printf("Wrong points (imposter points):\n");
    for (int i = 0; i < n; i++) {
        double expected_y = lagrange_interpolation(x, y, n, x[i]);
        if (y[i] != expected_y) {
            printf("Point (%lld, %lld) is wrong (expected: %.0f)\n", x[i], y[i], expected_y);
        }
    }
}

int main() {
    long long int x1[10], y1[10];
    int count1 = 0;
    
    // Read first test case
    read_json_data("input1.json", x1, y1, &count1);
    
    // Find the constant term c of the polynomial for the first test case
    double secret_c1 = lagrange_interpolation(x1, y1, count1, 0);
    printf("The secret (constant term c) for test case 1 is: %.0f\n", secret_c1);
    
    long long int x2[10], y2[10];
    int count2 = 0;

    // Read second test case
    read_json_data("input2.json", x2, y2, &count2);
    
    // Find the constant term c of the polynomial for the second test case
    double secret_c2 = lagrange_interpolation(x2, y2, count2, 0);
    printf("The secret (constant term c) for test case 2 is: %.0f\n", secret_c2);
    
    // Identify wrong points in the second test case
    find_wrong_points(x2, y2, count2, secret_c2);

    return 0;
}
