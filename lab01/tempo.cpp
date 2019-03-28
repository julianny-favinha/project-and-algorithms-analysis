#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <vector>

/*
 * Esse codigo apresenta duas maneiras de medir o tempo de computacao de um
 * trecho do seu codigo. Um dos metodos utiliza metodos da biblioteca time.h
 * disponivel para C/C++ e o segundo utilizando a biblioteca chrono disponivel
 * para C++. Ambas bibliotecas sao distribuidas junto dos compiladores (GCC, por
 * exemplo).
 *
 * O exemplo usado na medição e na ordenação de um vetor de 8000000 inteiros
 * gerados aleatoriamente.
 *
 * Compile:
 * g++ tempo.cpp -o tempo
 * Execute:
 * ./mede_tempo
 * 
 * Distribuido pelo professor Cid Carvalho de Souza
 * IC Unicamp
 */

using namespace std;

int main(int argc, char *argv[]) {

  // numero de elementos
  long int N = 8000000;

  // define a semente do gerador pseudo-aleatorio
  srand(unsigned(time(0)));
  // vetor de N elementos
  vector<int> V(N);
  // gera N inteiros aleatorios e armazena em V
  generate(V.begin(), V.end(), rand);
  // estruturas auxiliares para utilizar a biblioteca time.h
  clock_t tempo_inicio = 0, tempo_termino;
  // tempo de inÃ­cio usando a biblioteca chrono
  auto tempo_inicio_c = chrono::high_resolution_clock::now();
  // tempo de inÃ­cio usando a biblioteca time.h
  tempo_inicio = clock();
  sort(V.begin(), V.end());
  // tempo de termino usando a biblioteca time.h
  tempo_termino = clock();
  // tempo de termino usando a biblioteca chrono
  auto tempo_termino_c = chrono::high_resolution_clock::now();
  // calcula a diferença dos tempos de termino e inicio; imprime o resultado
  printf("[CLOCK] Tempo total: %.2f segundos\n",
         ((tempo_termino - tempo_inicio) / (float)CLOCKS_PER_SEC));
  printf("[CHRONO] Tempo total: %.2f segundos\n",
         chrono::duration<double>(tempo_termino_c - tempo_inicio_c).count());

  return 0;
}