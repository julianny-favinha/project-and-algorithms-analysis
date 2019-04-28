using JuMP, Gurobi, Printf

# open file

function read_file(args)
	@show args
end

read_file(ARGS)
file_name = "gt10."*ARGS[1]*".instance"
println(file_name)

f = open(file_name, "r")

# read file

let

lines = 1
edges = 0
n = 0
m = 0

head = zeros(Int64, (1,1))
tail=zeros(Int64,(1,1))

while !eof(f)
	println(">>> WHILE: lines=$lines edges=$edges")
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

println("n=$n")
println("m=$m")
println("head=$head")
println("tail=$tail")

end