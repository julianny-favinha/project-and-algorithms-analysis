% -------- ENTRADA --------
% numero de trabalhadores
int: L;

% numero de ordens
int: no;

% numero de jobs para cada ordem
array[1..no] of int: njo;

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

% -------- SAIDA --------
% tempo final para executar todas as tarefas
% TODO: REMOVER MOCK!!!
int: makespan = 80;

% tempo de inicio de cada tarefa
% TODO: REMOVER MOCK!!!
array[1..ntarefas] of int: inicio = [9,8,7,6,5,4,3,0];

output ["\(makespan)\n"];

output ["\(i) \(inicio[i])\n" | i in 1..ntarefas];