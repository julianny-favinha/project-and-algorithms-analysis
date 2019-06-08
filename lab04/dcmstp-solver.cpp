#include "input.hpp"

using namespace std;

int iterations = 1;

int best_dual = 9999;
int best_primal = 9999;

vector<NodeSource> best_agm;

// imprime grafo
void print_graph(vector<NodeSource> adjacency) {
	int nodes = adjacency.size();

	for (int i = 0; i < nodes; i++) {
		cout << "id: " << i << " max_degree: " << adjacency[i].max_degree << endl;
		cout << "adjacency list: ";

		for (int j = 0; j < adjacency[i].adj.size(); j++) {
			cout << "(" << adjacency[i].adj[j].id << ", " << adjacency[i].adj[j].cost << ") ";
		}
		cout << endl << endl;
	}
}

// retorna true quando ocorreram todas as iterações OU se o limitante dual é igual ao primal
bool stop_subgradient() {
	if (iterations == 0 || (best_dual == best_primal)) {
		return true;
	}

	iterations--;
	return false;
}

// incrementa o custo das arestas (i,j): c_(i,j) = c_(i,j) + lambda(i) + lambda(j)
vector<NodeSource> increment_edges_cost(vector<NodeSource> adjacency, vector<int> lambdas) {
	vector<NodeSource> adjacency2 = adjacency;

	for (int u = 0; u < adjacency2.size(); u++) {
		for (int v = 0; v < adjacency2[u].adj.size(); v++) {
			adjacency2[u].adj[v].cost += lambdas[u] + lambdas[adjacency2[u].adj[v].id];
		}
	}

	return adjacency2;
}

// função que busca o subconjunto de um elemento "i"
int buscar(int subset[], int i) {
	if(subset[i] == -1)
		return i;
	return buscar(subset, subset[i]);
}

// função para unir dois subconjuntos em um único subconjunto
void unir(int subset[], int v1, int v2){
	int v1_set = buscar(subset, v1);
	int v2_set = buscar(subset, v2);
	subset[v1_set] = v2_set;
}

// kruskal ou prim
int agm(vector<NodeSource> adjacency) {

	vector<pair<pair<int, int>, int > > arvore;

	map<pair<int, int>, int >::iterator it;
	map<pair<int, int>, int > edge_map;

	cout << "Teste 1" << endl;
	// for em todos os nos pra popular o mapa
	for (int u = 0; u < adjacency.size(); u++) {
		for (int v = 0; v < adjacency[u].adj.size(); v++) {
			if (!edge_map.count(make_pair(adjacency[u].adj[v].id,u))) {
				edge_map[make_pair(u,adjacency[u].adj[v].id)]=adjacency[u].adj[v].cost; 
			}
		}
	}

	cout << edge_map.size() << "Teste 1" << endl;

	// sort nos edges

    auto cmp = [](const auto &p1, const auto &p2) {
        return p2.second < p1.second || !(p1.second < p2.second);
    };

    set < pair<pair<int, int>, int >, decltype( cmp )> s(edge_map.begin(), edge_map.end(), cmp);

	// rodo o krusal

	int * subset = new int[adjacency.size()];
	memset(subset, -1, sizeof(int) * adjacency.size());
	for (const auto& kv : edge_map) {
		std::cout << '{' << kv.first.first << "," << kv.first.second << '}' << '\n';
		int v1 = buscar(subset, kv.first.first);
		int v2 = buscar(subset, kv.first.second);

		if(v1 != v2)
		{
			// se forem diferentes é porque NÃO forma ciclo, insere no vetor
			arvore.push_back(kv);
			unir(subset, v1, v2); // faz a união
		}

	}

	cout << arvore.size() << "Teste maluco" << endl;

	// supondo uma solução que envolve as arestas (0,1) (1,2) (1,3)
	// (10 + 1*2 + 20 + 1*2 + 80 + 1*2)
	return 156; 
}

// retorna um vetor de nodes posições com o grau de cada vértice
// vector<int> calculate_degrees(vector<NodeSource> adjacency) {
// 	vector<int> degrees(adjacency.size());

// 	for (int v = 0; v < degrees.size(); v++) {
// 		degrees[v] = adjacency[v].max_degree;
// 	}

// 	return degrees;
// }

// retorna o valor real de custo subtraindo somatorio lambda[i]*d[i]
// int calculate_real_cost(int cost_agm, vector<int> lambdas, vector<int> degrees) {
// 	int sum = 0;

// 	for (int v = 0; v < lambdas.size(); v ++) {
// 		sum += lambdas[v] * degrees[v];
// 	}

// 	return cost_agm - sum;
// }

// inicializa lambda
vector<int> first_lambda(int n) {
	vector<int> lambdas(n);

	for (int i = 0; i < lambdas.size(); i++) {
		lambdas[i] = 1;
	}

	return lambdas;
}

// atualiza lambda com o passo u_k
// TODO qual o tamanho do passo?
void update_lambdas(vector<int> *lambdas) {
	for (int i = 0; i < lambdas->size(); i++) {
		(*lambdas)[i] += 1;
	}
}

// grava no arquivo file_name.out a lista de arestas
void save_output(char *file_name) {
	string buf(file_name);
	buf.append(".out");

	ofstream file;
	file.open(buf);

	// TODO salvar arestas
	// vertices rotulados de 1 a n
	// ordem crescente
	// aresta 3 1 deve ser salva como 1 3

	file.close();
}

// relaxação lagrangiana / algoritmo do subgradiente
void lagrangean_relaxation(vector<NodeSource> adjacency) {
	int nodes = adjacency.size();
	
	vector<int> lambdas = first_lambda(nodes);

	while (stop_subgradient() == false) {
		vector<NodeSource> adjacency2 = increment_edges_cost(adjacency, lambdas);
		
		// calcula limitante dual. a agm retornada pode ser uma solucao viavel
		int dual_agm = agm(adjacency2);

		if (dual_agm < best_dual) {
			best_dual = dual_agm;
		}

		// TODO verifica se é uma solucao viavel
		// TODO se sim, é um limitante primal

		int primal_agm = dual_agm;

		// TODO se não, rodar alguma heuristica lagrangiana pra transformar em viavel

		if (primal_agm < best_primal) {
			best_primal = primal_agm;
			// TODO atualizar best_agm
		}

		update_lambdas(&lambdas);
	}
}

int main(int argc, char *argv[]) {
	char *file_name = argv[1];
	int time = atoi(argv[2]);
	char *method = argv[3];

	vector<NodeSource> adjacency = read_file(file_name);

	if (string(method) == "l") {
		lagrangean_relaxation(adjacency);
		save_output(file_name);
		cout << file_name << "," << best_dual << "," << best_primal << endl;
	} else {
		cout << "IMPLEMENTAR META-HEURÍSTICA" << endl;
		save_output(file_name);
		cout << file_name << "," << best_primal << endl;
	}
	
	return 0;
}