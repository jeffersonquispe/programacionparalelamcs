#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <stdio.h>
#include <omp.h>
#include <conio.h> 


using namespace std;

int nFilas, nCol;
int **output;
int **input;
int **kernel;


void generates(int ** input, int nFilas, int nColumn) {
	srand(time(0));//numeros Aleatorios en funcion del tiempo
	for (int i = 0; i<nFilas; i++)//para desplazarse por las columnas
	{
		for (int j = 0; j<nColumn; j++)//para desplazarse por las filas
		{
			//*(*(input + i) + j) = rand() % 2;//Agrega numero aleatorio a la posicion ij de la matriz
			*(*(input + i) + j) = 1;//Agrega numero aleatorio a la posicion ij de la matriz
											 //printf("\t%d",*(*(input+i)+j));//imprime elemento de la matriz en pantalla
											 //input[i][j]=rand()%2;
											 // printf("\t%d",input[i][j]);
		}
		 //printf("\n\n");//para dejar espacios entre filas.
	}
}

void generateker(int ** kernel, int nFilas, int nColumn) {
	for (int i = 0; i<nFilas; i++)//para desplazarse por las columnas
	{
		for (int j = 0; j<nColumn; j++)//para desplazarse por las filas
		{
			*(*(kernel + i) + j) = 1;//Agrega numero aleatorio a la posicion ij de la matriz
									 //printf("\t%d",*(*(kernel+i)+j));//imprime elemento de la matriz en pantalla
									 //input[i][j]=rand()%2;
									 // printf("\t%d",input[i][j]);
		}
		// printf("\n\n");//para dejar espacios entre filas.
	}
}

void convolucioniter(int ** output, int ** input, int ** kernel, int nInput, int nkernel) {
	int mitad, i, j, m, n, mm, nn, ii, jj, acumulador,matrizborde;
	mitad = nkernel / 2;
	matrizborde = nInput - nkernel;
	for (i = 0; i < nInput; ++i) // Filas
	{
		for (j = 0; j < nInput; ++j) // Columnas
		{
			if (i < mitad || i >= matrizborde || j < mitad || j >= matrizborde) {
				acumulador = 0;
				for (m = 0; m < nkernel; ++m) // Filas del Kernel
				{
					for (n = 0; n < nkernel; ++n) // Columnas del kernel
					{
						ii = i - mitad + m;
						jj = j - mitad + n;
						// validar limites de la imagen 00000
						if (ii >= 0 && ii < nFilas && jj >= 0 && jj < nFilas)
						{
							acumulador += input[ii][jj] * kernel[m][n];
						}
					}
				}
				output[i][j] = acumulador;
			}
			
			
		}
	}

	// Muestro imagen final aplicando la convolucion
	/*cout<<endl<<endl<<"Imagen Aplicando convolucion: "<<endl;
	for(int i = 0; i < nFilas; i++)
	{
	for(int j = 0; j < nFilas; j++)
	{
	cout<<" "<< output[i][j];
	}
	cout<<endl;
	}*/
}

void convolucionPar(int ** output, int ** input, int ** kernel, int nInput, int nkernel, int thread_count) {
	int mitad, i, j, m, n, mm, nn, ii, jj, acumulador,matrizborde,k;
	mitad = nkernel / 2;
	matrizborde = nInput - nkernel;
	#pragma omp parallel num_threads(thread_count) shared(output,input) private (i,j,n,m,k,acumulador)
		#pragma omp for schedule(dynamic)
		for (i = mitad; i < matrizborde; i++)
		{
			for (j = mitad; j < matrizborde; j++)
			{
				acumulador = 0;
				for (m = 0; m < nkernel; m++)
				{
					for (n = 0; n < nkernel; n++)
					{
						acumulador += kernel[m][n] * input[i - mitad + m][j - mitad + n];				
					}
				}
				output[i][j] = acumulador / 25;
			}
		}


	#pragma omp for schedule(dynamic)
		for (i = 0; i < mitad; ++i) // Filas
		{
			for (j = matrizborde; j < nInput; ++j) // Columnas
			{
					acumulador = 0; k = 0;
					for (m = 0; m < nkernel; ++m) // Filas del Kernel
					{
						for (n = 0; n < nkernel; ++n) // Columnas del kernel
						{
							// validar limites de la imagen 00000
							if (i - mitad + m >= 0 && i - mitad + m < nFilas &&  j - mitad + n >= 0 && j - mitad + n < nFilas)
							{
								k++;
								acumulador += input[i - mitad + m][j - mitad + n] * kernel[m][n];
							}
						}
					}
					output[i][j] = acumulador / k;				
			}
		}
	
	#pragma omp for schedule(dynamic)
		for (i = 0; i < mitad; i++)
		{
			for (j = 0; j < mitad; j++)
			{
				acumulador = 0; k = 0;
				for (m = 0; m < nkernel; m++)
				{
					for (n = 0; n < nkernel; n++)
					{
						if (i - mitad + m >= 0 && i - mitad + m < nFilas &&  j - mitad + n >= 0 && j - mitad + n < nFilas) {
							k++;
							acumulador += kernel[m][n] * input[i - mitad + m][j - mitad + n];
						}
						
					}
				}
				output[i][j] = acumulador / k;
			}
		}
	
	#pragma omp for schedule(dynamic)
	for (i = matrizborde; i < nInput; ++i) // Filas
	{
		for (j = matrizborde; j < nInput; ++j) // Columnas
		{
				acumulador = 0; k = 0;
				for (m = 0; m < nkernel; ++m) // Filas del Kernel
				{
					for (n = 0; n < nkernel; ++n) // Columnas del kernel
					{
						// validar limites de la imagen 00000
						if (i - mitad + m >= 0 && i - mitad + m < nFilas &&  j - mitad + n >= 0 && j - mitad + n < nFilas)
						{
							k++;
							acumulador += input[i - mitad + m][j - mitad + n] * kernel[m][n];
						}
					}
				}
				output[i][j] = acumulador / k;
			}
		}
	
	#pragma omp for schedule(dynamic)
		for (i = matrizborde; i < nInput; ++i) // Filas
		{
			for (j = 0; j < mitad; ++j) // Columnas
			{
				acumulador = 0; k = 0;
				for (m = 0; m < nkernel; ++m) // Filas del Kernel
				{
					for (n = 0; n < nkernel; ++n) // Columnas del kernel
					{
						// validar limites de la imagen 00000
						if (i - mitad + m >= 0 && i - mitad + m < nFilas &&  j - mitad + n >= 0 && j - mitad + n < nFilas)
						{
							k++;
							acumulador += input[i - mitad + m][j - mitad + n] * kernel[m][n];
						}
					}
				}
				output[i][j] = acumulador / k;
			}
		}


	
	
}
	
int main(int argc, char* argv[]) {
	double end, start;
	int nkernel, a, b;
	int thread_count, thread_count1;
	//thread_count = strtol(argv[1], NULL, 10);
	thread_count = 4;
	thread_count1 = 1;
	cout << "Digite el numero de filas: ";
	cin >> nFilas;
	//nFilas = 2000;
	nCol = nFilas;

	output = new int*[nFilas];//reservando memoria para las filas
	for (int i = 0; i<nFilas; i++) {
		output[i] = new int[nCol];//Reserevando memeoria para las columnas
	}
	input = new int*[nFilas];//reservando memoeria para las filas
	for (int i = 0; i<nFilas; i++) {
		input[i] = new int[nCol];//Reserevando memeoria para las columnas
	}
	kernel = new int*[nFilas];//reservando memoeria para las filas
	for (int i = 0; i<nFilas; i++) {
		kernel[i] = new int[nCol];//Reserevando memoria para las columnas
	}

	nkernel = 5;
	generates(input, nFilas, nCol);
	generateker(kernel, nkernel, nkernel);

	//convolute(output,input,kernel,nFilas,nCol);
	start = omp_get_wtime();
	convolucionPar(output, input, kernel, nFilas, nkernel, thread_count1);
	//b=clock();
	end = omp_get_wtime();
	double timeiter = end - start;
	cout << "tiempo iterativo : " << timeiter << endl;
	// a=clock();


	start = omp_get_wtime();
	convolucionPar(output, input, kernel, nFilas, nkernel, thread_count);
	//b=clock();
	end = omp_get_wtime();
	cout << "tiempo paralelo : " << end - start << endl;
	float par = end - start;
	//start = omp_get_wtime();
	// Conv(input,kernel,output,nFilas,nFilas,nFilas,nkernel,thread_count);
	//end = omp_get_wtime();
	//cout << "tiempo paralelo 2: " << end - start << endl;
	cout << "speedup " << (timeiter) / par<<endl;
	cout << "eficiencia " <<  ((timeiter) / par )/ thread_count;
	getch();
	return 0;
	//return(argc);
}
