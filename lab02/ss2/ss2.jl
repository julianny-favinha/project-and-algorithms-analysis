using JuMP, Gurobi, Printf

# read file

file_name = "ss2."*ARGS[1]*".instance"

f = open(file_name, "r")

let

lines = 1

line = readline(f)
n, s_count = [parse(Int, x) for x in split(line)]

T = Array{Int64}(undef, n)
D = Array{Int64}(undef, n)
S = Array{Tuple{Int64, Int64}}(undef, s_count)

i = 1
j = 1

while !eof(f)
	line = readline(f)

	if lines <= n
		t, d = [parse(Int64, x) for x in split(line)]
		T[i] = t
		D[i] = d 
		i += 1
	else
		s1, s2 = [parse(Int64, x) for x in split(line)]
		S[j] = (s1, s2)
		j += 1
	end
	
	lines += 1
end

close(f)

# model

time_limit = parse(Int64, ARGS[2])
SS2 = Model(solver=GurobiSolver(TimeLimit=time_limit))

# variables

@variable(SS2, s[i=1:n])

@variable(SS2, y[i=1:n], Bin)

@variable(SS2, x[i=1:n, j=1:n], Bin)

# objective function

@objective(SS2, Min, sum(y[i] for i in 1:n))

# constraints

M = sum(T[i] for i in 1:n)

# x_ij + x_ji = 1, para todo (i, j) em TxT, sendo i != j

for i in 1:n
	for j in 1:n
		if j != i
			@constraint(SS2, x[i, j] + x[j, i] == 1)
		end
	end
end

# s_j >= s_i + t_i - M*x_ji, para todo (i, j) em TxT, sendo i != j

for i in 1:n
	for j in 1:n
		if j != i
			@constraint(SS2, s[j] >= s[i] + T[i] - M * x[j, i])
		end
	end
end

# x_ij = 1, para todo (i, j) em S

for tuple in S
	@constraint(SS2, x[tuple[1], tuple[2]] == 1)
end

# s_j, t_j <= d_j + M*y_j

for i in 1:n
	@constraint(SS2, s[i] + T[i] <= D[i] + M * y[i])
end

# s_j >= 0

for i in 1:n
	@constraint(SS2, s[i] >= 0)
end

status = solve(SS2)

obj = getobjectivevalue(SS2)

open("ss2.out", "w") do f
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
	println("Número de nós explorados: ", getnodecount(SS2::Model))

D = getobjbound(SS2::Model)
P = getobjectivevalue(SS2::Model)
@printf("Melhor limitante dual: %.2f\n", D)
@printf("Melhor limitante primal: %.2f\n", P)
@printf("Gap de otimalidade: %.2f\n", (abs( D - P )/P)*100)
@printf("Tempo de execução: %.2f\n", getsolvetime(SS2::Model))

end