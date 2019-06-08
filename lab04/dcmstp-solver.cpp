#include "input.hpp"

int iterations = 1;

int best_dual = INT_MAX;
int best_primal = INT_MAX;

vector<NodeSource> best_agm;

// retorna true quando ocorreram todas as iterações OU se o limitante dual é igual ao primal
bool stop_subgradient() {
	if (iterations == 0 || (best_dual == best_primal && best_dual != INT_MAX)) {
		return true;
	}

	iterations--;
	return false;
}

// calcula custo de uma lista de adjacencias
int calculate_cost(vector<NodeSource> adjacency) {
	int cost = 0;
	int nodes = adjacency.size();

	for (int i = 0; i < nodes; i++) {
		for (int j = 0; j < adjacency[i].adj.size(); j++) {
			cost += adjacency[i].adj[j].cost;
		}
	}

	return cost;
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

// busca o subconjunto de um elemento i
int buscar(int subset[], int i) {
	if (subset[i] == -1) {
		return i;
	}

	return buscar(subset, subset[i]);
}

// une dois subconjuntos em um único subconjunto
void unir(int subset[], int v1, int v2){
	int v1_set = buscar(subset, v1);
	int v2_set = buscar(subset, v2);
	subset[v1_set] = v2_set;
}

// arvore geradora minima
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

	// ordena os edges crescentemente

    auto cmp = [](const auto &p1, const auto &p2) {
        return p2.second < p1.second || !(p1.second < p2.second);
    };

    set < pair<pair<int, int>, int >, decltype( cmp )> s(edge_map.begin(), edge_map.end(), cmp);

	int * subset = new int[adjacency.size()];
	memset(subset, -1, sizeof(int) * adjacency.size());
	for (const auto& kv : edge_map) {
		cout << '{' << kv.first.first << "," << kv.first.second << '}' << '\n';
		int v1 = buscar(subset, kv.first.first);
		int v2 = buscar(subset, kv.first.second);

		// se forem diferentes é porque NÃO forma ciclo, insere no vetor
		if (v1 != v2) {
			arvore.push_back(kv);
			unir(subset, v1, v2);
		}
	}

	cout << arvore.size() << "Teste maluco" << endl;

	return 156; 
}

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