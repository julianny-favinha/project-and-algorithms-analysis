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

	r, d, s = [parse(Int, x) for x in split(line)]
	R[i] = r
	D[i] = d
	S[i] = s

	i += 1
end

close(f)

# model

time_limit = parse(Int64, ARGS[2])
SR2 = Model(solver = GurobiSolver(NodefileStart = 8, TimeLimit = time_limit))

# variables

W = sum(S[i] for i in 1:n)

T = maximum(D)

@variable(SR2, x[i=1:n, j=1:W], Bin)

@variable(SR2, y[i=1:n, j=1:W, t=1:T], Bin)

@variable(SR2, k[j=1:W], Bin)

# objective function

@objective(SR2, Min, sum(k[j] for j in 1:W))

# constraints

# x_ij <= k_j

for i in 1:n
	for j in 1:W
		@constraint(SR2, x[i, j] <= k[j])
	end
end

# (soma em i) x_ij <= 1
# slot j só pode ser utilizado uma vez

for j in 1:W
	@constraint(SR2, sum(x[i, j] for i in 1:n) <= 1)
end

# x_ij >= y_ijt

for i in 1:n
	for j in 1:W
		for t in 1:T
			@constraint(SR2, x[i, j] >= y[i, j, t])
		end
	end
end

# (soma em j) x_ij == S_i

for i in 1:n
	@constraint(SR2, sum(x[i, j] for j in 1:W) == S[i])
end

# solve

status = solve(SR2)

obj = getobjectivevalue(SR2)

open("sr2.out", "w") do f
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
	println("Número de nós explorados: ", getnodecount(SR2::Model))

D = getobjbound(SR2::Model)
P = getobjectivevalue(SR2::Model)
@printf("Melhor limitante dual: %.2f\n", D)
@printf("Melhor limitante primal: %.2f\n", P)
@printf("Gap de otimalidade: %.2f\n", (abs( D - P )/P)*100)
@printf("Tempo de execução: %.2f\n", getsolvetime(SR2::Model))

end