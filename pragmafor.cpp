#include <omp.h>
#include <iostream>
 
#define N 15
#define nthreads 5
 
int main (){
  int thread;
  omp_set_num_threads(nthreads);
  #pragma omp parallel private(thread)
  {
      thread = omp_get_thread_num();

      #pragma omp for schedule(auto) /* - -el sistema lo ordena de manera que quiera*/
      /*#pragma omp for schedule( guided,1) - -  semejante al dynamic pero empieza de grande y disminuye*/
     /*#pragma omp for schedule(dynamic,4) - - agarra segmetos de hilos y los distribuye a n iteraciones continuas*/
     /* #pragma omp for schedule(static,4) - - agarra hebras y los distribuye en bloque de n iteraciones */
      for (int i = 0 ; i < N; i++){
      printf("soy el proceso %d ejecuto la iteración %d\n",thread,i );
      }
 
  }
}