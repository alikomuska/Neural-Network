#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define inputNum 2
#define numNeuronsH1 1
#define numNeuronsH2 1
#define numNeuronsH3 1
#define groupNum 4

void weightMatrixInit(int dim1, int dim2, float matrix[dim1][dim2]);

int main(){

  float inputs[inputNum];
  float outputsH1[numNeuronsH1];
  float outputsH2[numNeuronsH2];
  float outputsH3[numNeuronsH3];
  float outputs[groupNum];

  float weights01[inputNum][numNeuronsH1];
  float weights12[numNeuronsH1][numNeuronsH2];
  float weights23[numNeuronsH2][numNeuronsH3];
  float weights30[numNeuronsH3][groupNum];

  srand(time(NULL));
	  
  //weight matrices initialization
  weightMatrixInit(inputNum, numNeuronsH1, weights01);     //weights01 
  weightMatrixInit(numNeuronsH1, numNeuronsH2, weights12); //weigths12
  weightMatrixInit(numNeuronsH2, numNeuronsH3, weights23); //weights23
  weightMatrixInit(numNeuronsH3, groupNum, weights30);     //weights30


  return 0;
}



void weightMatrixInit(int dim1, int dim2, float matrix[dim1][dim2]){
  for(int i=0; i<dim1; i++){
    for(int j=0; j<dim2; j++){
      matrix[i][j] = rand();
      printf("value %f\n", matrix[i][j]);
    }  
  }
}
