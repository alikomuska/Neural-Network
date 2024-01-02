#include"kMeans.h"

int main(){
  FILE *fp;
  double **values;
  center_t *centers;
  double **teams;
  int seasons = 1;
 
  srand((unsigned int)time(NULL));

  fp = fopen("dataset2.txt", "r");
  if(fp == NULL){
    perror("Error opening the file");
    return -1;
  }
  
  values = load_data(fp);
  if(values == NULL)
    return -1;
  
  centers = centers_init(values);
  if(centers == NULL)
    return -1;
  
  teams = allocate_numOf_teams();
  if(teams == NULL)
      return -1;


  while(1){
    double error = 0;
    int count = 0;
    int i,j;
    double minTeam [2] ;
    
    for(i=0; i<N; i++){
      double min_dist[M];
      for(j=0; j<M; j++){
        min_dist[j] = calculate_distance(values[0][i],values[1][i],centers[j].x,centers[j].y);

      }
      
      find_min(min_dist,minTeam);
      centers[(int)minTeam[1]].size++;
      centers[(int)minTeam[1]].new_x += values[0][i];
      centers[(int)minTeam[1]].new_y += values[1][i];
      teams[(int)minTeam[1]][centers[(int)minTeam[1]].size] = minTeam[0];
    }

    for(i=0; i<M; i++){
      for(j=0; j<centers[i].size; j++){
        error += teams[i][j];  
      
      }
      if(centers[i].size>0){
        centers[i].new_x = centers[i].new_x / centers[i].size;
        centers[i].new_y = centers[i].new_y / centers[i].size;
      }
    }

    for(i=0; i<M; i++){
      if(centers[i].new_x == centers[i].x && centers[i].new_y == centers[i].y)
        count++ ;

      centers[i].x = centers[i].new_x;
      centers[i].y = centers[i].new_y;
      centers[i].new_x = 0;
      centers[i].new_y = 0;
      centers[i].size = 0;
    }

    printf("Season %d error : %lf\n",seasons,error);

    for(i=0;i<M;i++){
      printf("center[%d]-team : %d , (x = %lf ,y = %lf)\n",i+1,centers[i].team+1,centers[i].x,centers[i].y);
    }
    
    printf("\n");

    if(count == M)
      break;
    
    if(seasons == 1000){
      perror("Too many Seasons");
      break;
    }
    seasons++;
  }

  return 0;
  
}

void find_min(double *values, double *array){
  int i;
  array[0] = values[0];
  array[1] = 0;
  for(i=1; i<M; i++){
    if(array[0]>values[i]){
      array[0]= values[i];
      array[1] = i;

    }
  }
  
}

double calculate_distance(double x, double y, double cen_x, double cen_y){
  
  return sqrt(pow((cen_x-x), 2)+pow((cen_y-y), 2));
}

center_t *centers_init(double **values){
  center_t *centers;
  int i;

  centers = (center_t *)malloc(M*sizeof(center_t));
  if(centers ==NULL){
    perror("There was an error allocating memory for centers\n");
    return NULL;
  }
  
  for(i=0; i<M;i++){
    centers[i].y = create_random_values(0,1199);
    
    centers[i].x = (values[0][(int)(centers[i].y)]);
    centers[i].y = (values[1][(int)(centers[i].y)]);
    centers[i].team = i;
    centers[i].size = 0;
    centers[i].new_x = 0;
    centers[i].new_y = 0;
  }

  return centers;
}

int create_random_values(int min_y, int max_y){
  int value, y, range_y;

  range_y = (max_y - min_y);
      
  value = (double)(min_y + rand()%(range_y+1));

  return value;
}

double** allocate_numOf_teams(){
  double **oj;
  int i;

  oj = (double **)malloc(M*sizeof(double *));
  if(oj == NULL){
    perror("Error allocating memory for the teams\n");
    return NULL;
  }
  
  for(i=0; i<M; i++){
    oj[i] = (double *)malloc(N*sizeof(double));
    if(oj[i] == NULL){
      perror("Error allocating memory for the teams\n");
      return NULL;
    }
  }

  return oj;
}

double** load_data(FILE *fp){
  double **array;
  double x;
  double y;
  int i;
  int count = 0;
  int cols = 2;
  int rows = N;  

  array = (double **)malloc(cols*sizeof(double *));
  if(array==NULL){
    perror("Error allocating memory");
    return NULL;
  }

  for(i=0; i<cols; i++){
    array[i] = (double *) malloc(rows*sizeof(double));
    if(array[i] == NULL){
      perror("Error allocating memory");
      return NULL;
    }
  }

  while(fscanf(fp,"(x1,x2)=(%lf,%lf)\n",&x,&y)!= EOF){
    array[0][count] = x;
    array[1][count] = y;
    count++;
  }

  return array;
}
