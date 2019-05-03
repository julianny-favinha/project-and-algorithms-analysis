using JuMP, Gurobi, Printf

# read file

file_name = "gt54."*ARGS[1]*".instance"

f = open(file_name, "r")

let

nl=1; ne=0; n=0; m=0; nt=0; t=0; s=0
a=0; b=0; e1=0; e2=0; c=0; t1=0
touples=(Array{Int64, 1})[]
edges=(Array{Int64, 1})[]

while !eof(f)
	line=readline(f) 
	if (nl==1) 
	   n, t, m = [parse(Int, x) for x in split(line)]
	   ne=1
	elseif nl==2 
	   s, t1 = [parse(Int64, x) for x in split(line)]
	elseif nl <= t + 2
	   a, b = [parse(Int64, x) for x in split(line)]
	   push!(touples, [a, b])
	   ne += 1
	else
	   e1, e2, c = [parse(Int64, x) for x in split(line)]
	   push!(edges, [e1, e2, c])
	   nt += 1
	end 
	nl = nl+1
 end 
close(f)

Vertex = 1:n
edges_sum = zeros(JuMP.GenericAffExpr{Float64,Variable},(1,n))
vertex_sum = zeros(JuMP.GenericAffExpr{Float64,Variable},(1,n))

# model

time_limit = parse(Int64, ARGS[2])
GT = Model(solver = GurobiSolver(NodefileStart = 8, TimeLimit = time_limit))

# variables

@variable(GT, u[i in Vertex], Bin)
@variable(GT, x[i in edges], Bin)

# objective function

@objective(GT, Min, sum(i[3]*x[i] for i in edges))

for touple in touples
   @constraint(GT, u[touple[1]] + u[touple[2]] <= 1)
 end

# constraints

for j in edges
	edges_sum[j[1]] += x[j]
	edges_sum[j[2]] -= x[j]
	vertex_sum[j[1]] += x[j]
end

for i in Vertex
	if i == s
		@constraint(GT, edges_sum[i] == 1)
	elseif i == t1
		@constraint(GT, edges_sum[i] == -1)
	else i == s
		@constraint(GT, edges_sum[i] == 0)
	end
	@constraint(GT, u[i] == vertex_sum[i])
end

status = solve(GT)

obj = getobjectivevalue(GT)

open("gt54.out", "w") do f
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
	println("Número de nós explorados: ", getnodecount(GT::Model))

D = getobjbound(GT::Model)
P = getobjectivevalue(GT::Model)
@printf("Melhor limitante dual: %.2f\n", D)
@printf("Melhor limitante primal: %.2f\n", P)
@printf("Gap de otimalidade: %.2f\n", (abs( D - P )/P)*100)
@printf("Tempo de execução: %.2f\n", getsolvetime(GT::Model))

end