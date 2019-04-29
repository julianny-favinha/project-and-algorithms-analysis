using JuMP, Gurobi, Printf

# read file

file_name = "gt10."*ARGS[1]*".instance"

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

Edges = Array{Tuple{Int64, Int64}}(undef, m)
for i in 1:m
  Edges[i] = (head[i], tail[i])
end

# model

time_limit = parse(Int64, ARGS[2])
GT = Model(solver=GurobiSolver(TimeLimit=time_limit))

# variables

@variable(GT, x[i in Edges], Bin)

# objective function

@objective(GT, Max, sum(x[i] for i in Edges))

# constraints

Ivertices=1:n

for v in Ivertices
	flag = false
	sum = 0
	for e in Edges
		if v == e[1] || v == e[2]
			sum += x[e]
			flag = true
		end
	end

	if flag
		@constraint(GT, sum <= 1)
	end
end

status = solve(GT)
obj = getobjectivevalue(GT)
optimal_x = getvalue(x)

println("The optimal objective function value is = $obj")

println("The optimal_x = $optimal_x")

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
	println("Número de nós explorados: ", getnodecount(GT::Model))

D = getobjbound(GT::Model)
P = getobjectivevalue(GT::Model)
@printf("Melhor limitante dual: %.2f\n", D)
@printf("Melhor limitante primal: %.2f\n", P)
@printf("Gap de otimalidade: %.2f\n", (abs( D - P )/P)*100)
@printf("Tempo de execução: %.2f\n", getsolvetime(GT::Model))

end