#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 8000

int main(){

 double x, y;

 srand(time(NULL));
 FILE *file = fopen("dataset.txt", "w");

 if(file == NULL){
  printf("Something went wrong with the file");
  return -1;
 }

 for(int i = 0; i<N; i++){
  x = 2*(double)rand()/ (double)RAND_MAX -1;
  y = 2*(double)rand()/ (double)RAND_MAX -1;
 
  if(x<0){   // case 3,4,7,8,9
    if(pow(x + 0.5, 2) + pow(y - 0.5, 2) < 0.2 ||  (pow(x + 0.5, 2) + pow(y + 0.5, 2) < 0.2)){  // case 3,4,7,8
      if(x<-0.5){  //left inside the cycle left(green) C1 case 4,8
        fprintf(file, "%f %f 2\n", x , y);
      }else{        //left inside the cycle right (pink) C2 case 3,7
        fprintf(file, "%f %f 1\n", x, y);
      }
    }else{
      fprintf(file, "%f %f 4\n", x, y); // left outside of the cycles (blue) C4 case 10
    }
  }else{ //case 1,2,5,6,10
    if(pow(x - 0.5, 2) + pow(y - 0.5, 2) < 0.2 || pow(x - 0.5, 2) + pow(y + 0.5, 2) < 0.2){
      if(x>0.5){  //right inside the cycle right (pink) case 1,5
        fprintf(file, "%f %f 1\n", x, y);
      }else{
        fprintf(file, "%f %f 2\n", x, y); //right inside the cycle left (green) case 2, 6 
      }
    }else{ // right outside of the cycle (red) C3 case 9
      fprintf(file, "%f %f 3\n", x, y);
    }

  } //end else
 } //end for


}
