using JuMP, Gurobi, Printf

# read file

file_name = "sr2."*ARGS[1]*".instance"

f = open(file_name, "r")

let

n = parse(Int, readline(f))

R = Array{Int64}(undef, n)
D = Array{Int64}(undef, n)
S = Array{Int64}(undef, n)

i = 1

while !eof(f)
	line = readline(f)

	println(split(line))

	r, d, s = [parse(Int, x) for x in split(line)]
	R[i] = r
	D[i] = d
	S[i] = s

	i += 1
end

close(f)

# model

time_limit = parse(Int64, ARGS[2])
SR2 = Model(solver = GurobiSolver(TimeLimit = time_limit))

# variables

T = 0

@variable(SR2, x[i=1:n])

@variable(SR2, k[i=1:n, j=1:n], Bin)

@variable(SR2, z[i=1:n, j=1:n], Bin)

@variable(SR2, y[i=1:n, j=1:n], Bin)

# objective function

@objective(SR2, Min, T)

# constraints

M = sum(R[i] for i in 1:n) + sum(D[i] for i in 1:n)

# x_i <= T

for i in 1:n
	@constraint(SR2, x[i] <= T)
end

# x_i <= sum(s_j * j_ij)

for i in 1:n
	soma = 0

	for j in 1:n
		soma += S[j] * k[i, j]
	end

	@constraint(SR2, x[i] == soma)
end

# i >= r_j - M*(1 - z_ij)
# i < r_j + M*z_ij

for i in 1:n
	for j in 1:n
		@constraint(SR2, i >= R[j] - M*(1 - z[i, j]))
		@constraint(SR2, i <= R[j] + M*z[i, j] + 1)
	end
end

# i < d_j + M*(1 - y_ij)
# i >= d_j - M*y_ij

for i in 1:n
	for j in 1:n
		@constraint(SR2, i <= D[j] + M*(1 - y[i, j]) + 1)
		@constraint(SR2, i >= D[j] - M*y[i, j])
	end
end

# k_ij >= y_ij + z_ij - 1

for i in 1:n
	for j in 1:n
		@constraint(SR2, k[i, j] >= y[i, j] + z[i, j] - 1)
		@constraint(SR2, k[i, j] <= y[i, j])
		@constraint(SR2, k[i, j] <= z[i, j])
		@constraint(SR2, k[i, j] >= 0)
		@constraint(SR2, k[i, j] <= 1)
	end
end

status = solve(SR2)

obj = getobjectivevalue(SR2)

println("The optimal objective function value is = $obj")

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
	println("Número de nós explorados: ", getnodecount(SR2::Model))

D = getobjbound(SR2::Model)
P = getobjectivevalue(SR2::Model)
@printf("Melhor limitante dual: %.2f\n", D)
@printf("Melhor limitante primal: %.2f\n", P)
@printf("Gap de otimalidade: %.2f\n", (abs( D - P )/P)*100)
@printf("Tempo de execução: %.2f\n", getsolvetime(SR2::Model))

end