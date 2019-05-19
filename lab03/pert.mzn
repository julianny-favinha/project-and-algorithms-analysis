% -------- ENTRADA --------
% numero de trabalhadores
int: L;

% numero de ordens
int: no;

% numero de jobs para cada ordem
array[1..no] of int: njo;

int: njobs = sum(i in 1..no)(njo[i]);

array[1..njobs] of int: extended_djo = [djo[i] | i in 1..no, j in 1..njo[i]];

% duracao de cada ordem
array[1..no] of int: djo;

% numero de tarefas
int: ntarefas = sum(i in 1..no)(djo[i]);

% numero de trabalhadores necessarios para cada tarefa
array[1..ntarefas] of int: trab;

% numero de precedencias
int: nprec;

% matriz de precedencias. ex: (1, 3) significa que tarefa 3 pode ser executada somente depois da 1
array [1..2, 1..nprec] of int: prec;

% TODO: REMOVER MOCK!!!!
array[1..njobs] of int: z = [1,1,5,5,5,7,7];

% -------- VARIAVEIS --------
% tempos de inicio de cada job
int: soma_tempos = sum(extended_djo);
array[1..njobs] of var 0..soma_tempos: inicio;

% variavel que queremos minimizar
var 0..soma_tempos: makespan;

% x
array[1..njobs, 0..soma_tempos] of var 0..99: x;

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
% TODO: REMOVER MOCK!!!!
array[1..no] of int: index = [1,3,6];

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

% -------- FUNCAO OBJETIVO --------
solve minimize makespan;

% -------- SAIDA --------
output ["\(makespan)\n"];

output ["\(i) \(inicio[i])\n" | i in 1..njobs];
