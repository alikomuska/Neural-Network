#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void create_values(double min_x, double max_x, double min_y, double max_y, int reps, FILE *fp);

int main(){

	srand(time(NULL));
 	FILE *file = fopen("dataset2.txt", "w");

 	if(file == NULL){
  	printf("Something went wrong with the file\n");
  		return -1;
 	}	

 	create_values(0.8,1.2,0.8,1.2,150,file);						//1)
 	create_values(0,0.5,0,0.5,150,file);							//2)
 	create_values(1.5,2,0,0.5,150,file);							//3)
 	create_values(0,0.5,1.5,2,150,file);							//4)
 	create_values(1.5,2,1.5,2,150,file);							//5)
 	create_values(0,0.4,0.8,1.2,75,file);							//6)
 	create_values(1.6,2,0.8,1.2,75,file);							//7)
 	create_values(0.8,1.2,0.3,0.7,75,file);							//8)
 	create_values(0.8,1.2,1.3,1.7,75,file);							//9)
 	create_values(0,2,0,2,150,file);								//10)

	return 0;
}

 	void create_values(double min_x, double max_x, double min_y, double max_y,int reps, FILE *fp){
 		
 		double x,y,range_x, range_y,div_x, div_y;
 		int i;
 		
 		range_x = (max_x - min_x);
 		range_y = (max_y - min_y);

 		div_x = RAND_MAX /range_x;
 		div_y = RAND_MAX /range_y;

 		for(i=0; i<reps; i++){
 			
 			x = min_x + (rand()/div_x);
 			y = min_y + (rand()/div_y);

 			fprintf(fp,"(x1,x2)=(%f,%f)\n",x,y);
 		}
 	}



