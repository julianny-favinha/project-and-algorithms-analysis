include "disjunctive.mzn";

% -------- ENTRADA --------

% numero de trabalhadores
int: L;

% numero de ordens
int: no;

% numero de jobs para cada ordem
array[1..no] of int: njo;

% soma total de jobs
int: njobs = sum(i in 1..no)(njo[i]);

% duracao de cada ordem
array[1..no] of int: djo;

% duracao de cada job
array[1..njobs] of int: extended_djo = extendVector(djo);

% numero de tarefas
int: ntasks = sum(i in 1..no)(djo[i]);

% numero de trabalhadores necessarios para cada tarefa
array[1..ntasks] of int: trab;

array[1..soma_tempos] of int: extended_trab = [trab[k] | i in 1..no, j in 1..njo[i], k in 1..djo[i]];

% numero de precedencias
int: nprec;

% matriz de precedencias. ex: (1, 3) significa que tarefa 3 pode ser executada somente depois da 1
array [1..nprec, 1..2] of int: prec;

% vetor auxiliar para montar o vetor z
array[1..no] of int: aux = [1 + sumOrderJobs(i,djo) | i in 1..no];

% posicao da primeira task no vetor trab para cada job
array[1..njobs] of int: z = extendVector(aux);

% primeiro job de cada ordem
array[1..no] of int: index = [1 + sumOrderJobs(i,njo) | i in 1..no];

% soma da duracao de cada job
int: soma_tempos = sum(extended_djo);

% -------- FUNCOES --------

function int: sumOrderJobs(int: i, array[1..no] of int: target) = sum(j in 1..i-1)(target[j]);

function array[1..njobs] of int: extendVector(array[1..no] of int: target) = [target[i] | i in 1..no, j in 1..njo[i]];

% -------- VARIAVEIS --------

% tempo de inicio de cada job
array[1..njobs] of var 0..soma_tempos: inicio;

% variavel que queremos minimizar
var 0..soma_tempos: makespan;

% quantidade de recurso utilizada por um job j num tempo t
array[1..njobs, 0..soma_tempos] of var 0..max(trab): x;

array[1..soma_tempos] of var 0..soma_tempos: inicioTasks = [inicio[i] + j | i in 1..njobs, j in 0..extended_djo[i]-1];

% -------- RESTRICOES --------

% o makespan deve ser maior ou igual a inicio[job] + duracao[job] para todo job
constraint
  forall(job in 1..njobs) (
    makespan >= inicio[job] + extended_djo[job]
  );

% precedencia entre jobs de ordens diferentes dado pela matriz prec
constraint
  forall (j in 1..nprec) (
    inicio[prec[j, 1]] + extended_djo[prec[j, 1]] <= inicio[prec[j, 2]]
  ); 
  
% precedencia entre jobs de mesma ordem
constraint
  forall (ordem in 1..no) (
    forall (job in index[ordem]..index[ordem] + njo[ordem] - 2) (
      inicio[job] + djo[ordem] <= inicio[job+1]
    )
  );

% sequencia das tarefas dentro de cada job
constraint
  forall (job in 1..njobs) (
    forall (indice in 1..extended_djo[job]) (
      x[job, inicio[job] + indice - 1] = trab[z[job] + indice - 1]
      
    )
  );

% restricao de recurso
constraint
  forall (t in 0..soma_tempos) (
    sum(job in 1..njobs)(x[job, t]) <= L
  );
  
% -------- RESTRICOES GLOBAIS --------

% o numero maximo de recursos deve ser respeitado
array[1..soma_tempos] of int: duracao = [1 | i in 1..soma_tempos];
constraint cumulative(inicioTasks, duracao, extended_trab, L);

% -------- FUNCAO OBJETIVO --------

solve minimize makespan;

% -------- SAIDA --------

output ["\(makespan)\n"];

output ["\(i) \(inicio[i])\n" | i in 1..njobs];