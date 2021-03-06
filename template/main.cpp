
#include "generalCuda.cuh"


// 8.4f,-3.2f,1000,0.0000001f,0.000000001f);

extern "C"  void callkernel(float* h_tableOfMinima, float end,float start, int noOfSamples,float incr, float h);

/*
float getLocalMinimum(float jump, float start,float incr,float h){
	float minimum=0.0f;
	float d = 0.0f;
	float dprev = 0.0f;
	float prevx = 0.0f;
	for(float i=0; i<jump;i+=incr){
		d=derivative(start+i,h);
		if((d==0.00000000f)||(dprev>0 &&d<0) || (dprev<0 && d>0)){
			printf("\npx = %.10f; x = %.10f; dp = %.10f; derivative = %.10f",prevx,start+i,dprev,d);
			getchar();
		}
		dprev=d;
		prevx = start+i;
		
	}
	//printf("\n%.10f",function(3.0f));
	
return minimum;
}
*/
//dodać dynamiczne tablice albo wrzucić do jednej pętli
int getMinimaHost(float * tableOfMinima, float end, float start, int noOfSamples,float incr, float h,int sizeOfTable,float MARKER,float divider){
	float jump = (end-start+incr)/(float)noOfSamples;
	float minimum=0.0f;
	double d = 0.0f;
	double dprev = 0.0f;
	double prevx = 0.0f;
	double dnext = 0.0f;
	//float i=0.0f;
	int prevIndex = 0;
	float x = 0.0f;
int index =0;

	for( int j=0; j<sizeOfTable;j++){
		//tableOfMinima[i] = getLocalMinimum(jump,start,incr,h);
		//for(float i=0; i<jump;i+=incr){
		x=start/divider +(incr/divider)*j;
			d=derivative((double)x,(double)h);
			if(d==0){
				printf("jjjajajjajajaa");
			}
			dnext=derivative((double)(x+incr/divider),(double)h);
			//if((d==0.00000000f)||(dprev>0 &&d<0) || (dprev<0 && d>0)){
				if( dprev<=0.0f && dnext>=0.0f &&(index>prevIndex+3)){// || ((d<threshold) && ( d>-threshold)) ){
				
					if(index>=sizeOfTable){
						printf("wtf");
					}
				
				tableOfMinima[index] = x;
				prevIndex=index;
				//getchar();
			}else{

				if(index>=sizeOfTable){
						printf("wtf");
				}
				tableOfMinima[index]=MARKER;
			}
			dprev=d;
			//prevx = start+i;
			//i+=incr;
			index++;
				
		//start+=jump;
	}

	return 0;
}

void printMinima(float * minima,int count,float MARKER){
	int prevI= -1;
	for(int i=0; i<count;i++){
		if(minima[i]>MARKER && prevI+3<i){
			printf("\nx = %.10f; f(x) = %.10f",minima[i],function(minima[i]));
			prevI=i;
		}
	}
	
}

int main(){
	//RANGE / INCR +1;
/*
#define RANGE  8.4f
#define INCR   0.00000010000f
#define START -3.20000000000f
#define END    6.20000000000f
#define NO_OF_SAMPLES 1000
#define H 0.000000001f
#define MARKER (START-10.00000000000f)*/
	
	int NO_OF_SAMPLES = 1000;
	float H =     0.00000000100f;
	float END =    62000000.0000f;
	float START = -32000000.0000f;
	
	float H_GPU =      0.010000f;
	float END_GPU =    6.200000f;
	float START_GPU = -3.200000f;

	float INCR =   10.0f;
	float suma = END-START+INCR;
	float dzielenie = suma/INCR;
	float MARKER = START/10000000.000000f - 10.0000000000f;
	int MAXMINIMA = (int)(dzielenie) +1;

	clock_t start_calc,end_calc;

	float * minimaCPU = (float*) malloc(sizeof(float) * MAXMINIMA);
	
	start_calc=clock();
	int result = getMinimaHost(minimaCPU, END,START,NO_OF_SAMPLES,INCR,H,MAXMINIMA,MARKER,10000000.000000f);
	end_calc = clock();

	printf("\nCPU Minima\n-----------------------------------------------------------\n");
	printMinima(minimaCPU,MAXMINIMA,MARKER);
	printf("\nComputation time = %d millisec\n", (end_calc-start_calc)*1000/(CLOCKS_PER_SEC));
	free(minimaCPU);
	
	float * minimaGPU = (float*) malloc(sizeof(float) * MAXMINIMA);
	
	start_calc=clock();
	callkernel(minimaGPU,END_GPU,START_GPU,NO_OF_SAMPLES,INCR,H_GPU);
	end_calc = clock();

//	callkernel(minimaGPU,6.2f,-3.2f,1000,0.1f,0.01f);

	printf("\nGPU Minima\n-----------------------------------------------------------\n");
	printMinima(minimaGPU,MAXMINIMA,MARKER);
	printf("\nComputation time = %d millisec\n", (end_calc-start_calc)*1000/(CLOCKS_PER_SEC));
	free(minimaGPU);
	//callkernel(0,0);
	getchar();
    return 0;
}