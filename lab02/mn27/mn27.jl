using JuMP, Gurobi, Printf

# read file

file_name = "mn27."*ARGS[1]*".instance"

f = open(file_name, "r")

let

lines = 1
edges = 0
n = 0
m = 0

head = zeros(Int64, (1,1))
tail=zeros(Int64,(1,1))

while !eof(f)
	line = readline(f)
	if lines == 1
		n, m = [parse(Int, x) for x in split(line)]
		head = zeros(Int64, (1, m))
		tail = zeros(Int64, (1, m))
		edges = 1
	else
		head[edges], tail[edges] = [parse(Int64, x) for x in split(line)]
		edges += 1
	end

	lines += 1
end
close(f)

Vertex = 1:n
Edges = Array{Tuple{Int64, Int64}}(undef, m)
for i in 1:m
  Edges[i] = (head[i], tail[i])
end

# model

time_limit = parse(Int64, ARGS[2])
MN = Model(solver=GurobiSolver(TimeLimit=time_limit))

# variables

@variable(MN, x[i in Vertex, j in Vertex], Bin)

@variable(MN, u[i in 1:n], Bin)

# objective function

@objective(MN, Min, sum(u[i] for i in 1:n))

# constraints

# Todo vértice deve receber exatamente uma cor

for v in Vertex
	@constraint(MN, sum(x[v, i] for i in Vertex) == 1)
end

# Se um vértice recebe uma cor k, esta cor tem que ser usada

for v in Vertex
	for i in Vertex
		@constraint(MN, x[v, i] <= u[i])
	end
end

# Uma cor só pode ser usada se algum vértice tiver aquela cor

for v in Vertex
	@constraint(MN, u[v] <= sum(x[i, v] for i in Vertex))
end

# Os vértices extremos de uma aresta não podem ter a mesma cor

for e in Edges
	for k in Vertex
		@constraint(MN, x[e[1], k] + x[e[2], k] <= 1)
	end
end

status = solve(MN)

obj = getobjectivevalue(MN)

open("mn27.out", "w") do f
	write(f, "$obj")
end

# relatorio 

println("===============================")
if status == :Optimal
	println("Solução ótima encontrada.")
elseif status == :Unbounded
	println("Problema é ilimitado.")
elseif status == :Infeasible
	println("Problema é inviável.")
elseif status == :UserLimit
	println("Parado por limite de tempo ou iterações.")
elseif status == :Error
	println("Erro do resolvedor.")
else
	println("Não resolvido.")
end
	println("Número de nós explorados: ", getnodecount(MN::Model))

D = getobjbound(MN::Model)
P = getobjectivevalue(MN::Model)
@printf("Melhor limitante dual: %.2f\n", D)
@printf("Melhor limitante primal: %.2f\n", P)
@printf("Gap de otimalidade: %.2f\n", (abs( D - P )/P)*100)
@printf("Tempo de execução: %.2f\n", getsolvetime(MN::Model))

end