#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef float              f32;
typedef double             f64;
typedef unsigned long long u64;

typedef struct particle_s {

  f64 x, y, z;
  
} particle_t;


// Function for reading positions from a file
particle_t *readPositionsFromFile(const char *filename, u64 *nbrParticles) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Erreur lors de l'ouverture du fichier %s\n", filename);
        return NULL;
    }

    // Count the number of lines to determine the number of particles
    u64 count = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }

    // Reset file cursor to initial position
    fseek(file, 0, SEEK_SET);

    // Allocate memory to store positions
    particle_t *positions = malloc(count * sizeof(particle_t));

    // Read positions from file
    for (u64 i = 0; i < count; i++) {
        fscanf(file, "%lf %lf %lf", &(positions[i].x), &(positions[i].y), &(positions[i].z));
    }

    fclose(file);

    *nbrParticles = count;
    return positions;
}


particle_t compute_delta(particle_t *p_ref, particle_t *p, u64 n) {
    particle_t delta;
    delta.x = 0.0;
    delta.y = 0.0;
    delta.z = 0.0;	
    for (u64 i = 0; i < n; i++) {
        delta.x += (p_ref[i].x - p[i].x);
        delta.y += (p_ref[i].y - p[i].y);
        delta.z += (p_ref[i].z - p[i].z);
    }

    delta.x /= (f64)n;
    delta.y /= (f64)n;
    delta.z /= (f64)n;

    return delta;
}


int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <fichier_positions_base> <fichier_positions_optimized>\n", argv[0]);
        return EXIT_FAILURE;
    }
 
    u64 nbrParticlesBase, nbrParticlesOptimized;

    // Read the positions from files
    particle_t *positionsBase = readPositionsFromFile(argv[1], &nbrParticlesBase);
    particle_t *positionsOptimized = readPositionsFromFile(argv[2], &nbrParticlesOptimized);

    if (positionsBase == NULL || positionsOptimized == NULL) {
        return EXIT_FAILURE;
    }
    
    u64 n = nbrParticlesBase < nbrParticlesOptimized ? nbrParticlesBase : nbrParticlesOptimized;
    
    // Calculate the average distance between reference and optimized positions
    particle_t distance = compute_delta(positionsBase, positionsOptimized, n);

    printf("Average distance between reference and optimized versions : x:%lf y:%lf z:%lf\n", distance.x, distance.y, distance.z);

    // Close files and free memory
    free(positionsBase);
    free(positionsOptimized);
    return 0;
}
