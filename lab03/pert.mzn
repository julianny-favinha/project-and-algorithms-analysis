include "cumulative.mzn";

% ------------- ENTRADA -------------

% numero de recursos
int: L;

% numero de ordens
int: no;

% numero de jobs para cada ordem
array[1..no] of int: njo;

% duracao de cada ordem
array[1..no] of int: djo;

% quantidade de recursos necessarios para cada tarefa
array[1..ntasks] of int: trab;

% numero de precedencias
int: nprec;

% matriz de precedencias
array [1..nprec, 1..2] of int: prec;

% ------------ AUXILIARES ------------

% soma total de jobs
int: njobs = sum(i in 1..no)(njo[i]);

% duracao de cada job
array[1..njobs] of int: extended_djo = extendArray(djo);

% numero de tarefas
int: ntasks = sum(i in 1..no)(djo[i]);

% posicao da primeira tarefa no vetor trab para cada job
array[1..njobs] of int: z = extendArray([1 + sumOrderJobs(i, djo) | i in 1..no]);

% primeiro job de cada ordem
array[1..no] of int: index = [1 + sumOrderJobs(i, njo) | i in 1..no];

% soma da duracao de cada job
int: sum_times = sum(extended_djo);

% -------------- FUNCOES -------------

function int: sumOrderJobs(int: i, array[1..no] of int: target) = sum(j in 1..i-1)(target[j]);

function array[1..njobs] of int: extendArray(array[1..no] of int: target) = [target[i] | i in 1..no, j in 1..njo[i]];

% ------------- VARIAVEIS -------------

% tempo de inicio de cada job
array[1..njobs] of var 0..sum_times-min(djo): start;

% variavel que queremos minimizar
var max(djo)..sum_times: makespan;

% quantidade de recurso utilizada por um job j num tempo t
array[1..njobs, 0..sum_times] of var 0..max(trab): x;

% ------------ RESTRICOES ------------

% o makespan deve ser maior ou igual a inicio[job] + duracao[job] para todo job
constraint
  forall(job in 1..njobs) (
    makespan >= start[job] + extended_djo[job]
  );

% precedencia entre jobs de ordens diferentes dado pela matriz prec
constraint
  forall (j in 1..nprec) (
    start[prec[j, 1]] + extended_djo[prec[j, 1]] <= start[prec[j, 2]]
  );
  
% precedencia entre jobs de mesma ordem
constraint
  forall (ordem in 1..no) (
    forall (job1 in index[ordem]..(index[ordem] + njo[ordem] - 1)) (
      forall (job2 in job1+1..(index[ordem] + njo[ordem] - 1)) (
        start[job1] + djo[ordem] <= start[job2]
      )      
    )
  );

% sequencia das tarefas dentro de cada job
constraint
  forall (job in 1..njobs) (
    forall (indice in 1..extended_djo[job]) (
      x[job, start[job] + indice - 1] = trab[z[job] + indice - 1]
    )
  );

% restricao de recurso
constraint
  forall (t in 0..sum_times) (
    sum(job in 1..njobs)(x[job, t]) <= L
  );
  
% -------- RESTRICOES GLOBAIS --------

% toda tarefa tem duracao 1
array[1..sum_times] of int: duration = [1 | i in 1..sum_times];

array[1..sum_times] of int: extended_trab = [trab[k] | i in 1..no, j in 1..njo[i], k in 1..djo[i]];

% tempo de inicio de cada tarefa
array[1..sum_times] of var 0..sum_times: startTasks = [start[i] + j | i in 1..njobs, j in 0..extended_djo[i]-1];

% o numero maximo de recursos deve ser respeitado
constraint 
  cumulative(startTasks, duration, extended_trab, L);

% ---------- FUNCAO OBJETIVO ----------

solve minimize makespan;

% --------------- SAIDA ---------------

output ["\(makespan)\n"];

output ["\(i) \(start[i])\n" | i in 1..njobs];