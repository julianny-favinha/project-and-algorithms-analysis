% -------- ENTRADA --------
% numero de trabalhadores
int: L;

% numero de ordens
int: no;

% numero de jobs para cada ordem
array[1..no] of int: njo;

int: njobs = sum(i in 1..no)(njo[i]);

array[1..njobs] of int: extended_njo = [djo[i] | i in 1..no, j in 1..njo[i]];

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

% variavel que queremos minimizar
array[1..njobs] of int: soma = [inicio[job] + extended_njo[job] | job in 1..njobs];
int: makespan = max(job in 1..njobs) (soma[job]);

%solve minimize makespan;

% tempo de inicio de cada tarefa
% TODO: REMOVER MOCK!!!
array[1..njobs] of int: inicio = [0,5,4,6,9,0,6];

% -------- SAIDA --------
output ["\(makespan)\n"];

output ["\(i) \(inicio[i])\n" | i in 1..njobs];