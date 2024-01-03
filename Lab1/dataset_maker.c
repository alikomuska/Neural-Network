#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    // Seed the random number generator with the current time
    srand(time(NULL));

    // Define the file pointer and open the file in write mode
    FILE *file = fopen("dataset.txt", "w");

    // Check if the file was opened successfully
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    // Generate 8000 examples
    for (int i = 0; i < 8000; ++i) {
        // Generate random float values between -1 and 1
        float x1 = ((float)rand() / RAND_MAX) * 2 - 1;
        float x2 = ((float)rand() / RAND_MAX) * 2 - 1;

        // Write the values to the file
        fprintf(file, "%f %f\n", x1, x2);
    }

    // Close the file
    fclose(file);

    printf("File generated successfully!\n");

    return 0;
}
