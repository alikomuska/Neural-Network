#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define M 9
#define N 1200

struct center_s{
	double x;
	double y;
	int team;
	int size;
	double new_x;
	double new_y;
};

typedef struct center_s center_t;

double** load_data(FILE *fp);
center_t *centers_init();
int create_random_values(int min_y, int max_y);
double** allocate_numOf_teams();
double* resize_teams(double *teams, center_t center);
double calculate_distance(double x, double y, double cen_x, double cen_y);
void find_min(double *values, double *array);
