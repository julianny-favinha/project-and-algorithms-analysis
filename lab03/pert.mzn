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
array [1..2,1..nprec] of int: prec;

% tempo de inicio de cada tarefa
int: soma_tempos = sum(extended_djo);
array[1..njobs] of var 0..soma_tempos: inicio;

% variavel que queremos minimizar
var 0..soma_tempos: makespan;

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

% -------- FUNCAO OBJETIVO --------
solve minimize makespan;

% -------- SAIDA --------
output ["\(makespan)\n"];

output ["\(i) \(inicio[i])\n" | i in 1..njobs];
