#include "input.hpp"
#include "agm.hpp"
#include "output.hpp"
#include "grasp.hpp"

// número de iterações
int iterations = 100;

float alpha = 0.2;

// melhores valores / solução encontrados
float best_dual = float(INT_MIN);
float best_primal = float(INT_MAX);
vector<NodeSource> best_agm;

// tempo de inicio de execucao
clock_t start_time;

// tempo maximo de execucao
int max_time;

// retorna true quando ocorreram todas as iterações OU se o limitante primal é menor que o dual
bool stop_subgradient() {
	if (iterations == 0 || best_primal < best_dual) {
		return true;
	}

	iterations--;
	return false;
}

// incrementa o custo das arestas (i,j): c_(i,j) += lambda(i) + lambda(j)
vector<NodeSource> increment_edges_cost(vector<NodeSource> adjacency, vector<float> lambdas) {
	vector<NodeSource> adjacency2 = adjacency;

	for (int u = 0; u < adjacency2.size(); u++) {
		for (int v = 0; v < adjacency2[u].adj.size(); v++) {
			adjacency2[u].adj[v].cost += lambdas[u] + lambdas[adjacency2[u].adj[v].id];
		}
	}

	return adjacency2;
}

// verifica se é uma solucao
// como é uma arvore, cada vertice deve estar conectado a pelo menos uma aresta
// não ultrapassar a restrição dos vertices
bool is_solution(vector<NodeSource> adjacency) {
	for (int i = 0; i < adjacency.size(); i++) {
		if (adjacency[i].adj.size() < 1 || adjacency[i].adj.size() > adjacency[i].max_degree) {
			return false;
		}
	}

	return true;
}

// remove os lambdas somados no custo de cada aresta
vector<NodeSource> remove_lambdas(vector<NodeSource> adjacency, vector<float> lambdas) {
	vector<NodeSource> result = adjacency;

	for (int i = 0; i < result.size(); i++) {
		for (int j = 0; j < result[i].adj.size(); j++) {
			result[i].adj[j].cost -= (lambdas[i] + lambdas[result[i].adj[j].id]);
		}
	}

	return result;
}

// inicializa lambdas
vector<float> first_lambda(int n) {
	vector<float> lambdas(n);

	for (int i = 0; i < lambdas.size(); i++) {
		lambdas[i] = 0.0;
	}

	return lambdas;
}

// atualiza lambdas
vector<float> update_lambdas(vector<float> lambdas, vector<NodeSource> dual, float cost_dual, float cost_primal) {
	int nodes = lambdas.size();

	vector<float> new_lambdas(nodes);

	float beta = 0.03;

	vector<int> s(nodes);
	int s_norm = 0;

	for (int i = 0; i < nodes; i++) {
		s[i] = dual[i].adj.size() - dual[i].max_degree;
		s_norm += s[i] * s[i];
	}

	float t = alpha*((1 + beta) * cost_primal - cost_dual) / s_norm;


	// atualiza lambdas
	for (int i = 0; i < nodes; i++) {
		new_lambdas[i] = max(0.0f, lambdas[i] + t * s[i]);
	}

	return new_lambdas;
}

// relaxação lagrangiana / algoritmo do subgradiente
void lagrangean_relaxation(Graph *g) {
	vector<NodeSource> adjacency = g->adjacency;
	vector<float> lambdas = first_lambda(g->V);
	int no_progress = 0;

	while (stop_subgradient() == false) {
		if (time_expired(start_time, max_time)) {
        	break;
    	}

		vector<NodeSource> adjacency_with_lambdas = increment_edges_cost(adjacency, lambdas);

		// calcula arvore geradora minima (limitante dual)
		pair< bool, vector<NodeSource> > result_dual = agm(adjacency_with_lambdas, start_time, max_time);

		if (result_dual.first == true) {
			vector<NodeSource> dual_agm_with_lambdas = result_dual.second;

			int cost_dual_with_lambdas = calculate_cost(dual_agm_with_lambdas);

			int cost_primal = best_primal;

			if (is_solution(dual_agm_with_lambdas)) {
				// se o dual é uma solução, então é um limitante primal também
				vector<NodeSource> primal_agm = remove_lambdas(dual_agm_with_lambdas, lambdas);
				cost_primal = calculate_cost(primal_agm);

				if (time_expired(start_time, max_time)) {
	        		break;
	    		}

				if (cost_primal < best_primal) {
					best_primal = cost_primal;
					best_agm = primal_agm;
				}
			} else {
				pair< bool, vector<NodeSource> > result_primal = agm_with_degree_restriction(g->V, g->E, adjacency_with_lambdas, start_time, max_time);

				if (result_primal.first == true) {
					vector<NodeSource> primal_agm_with_lambdas = result_primal.second;

					if (time_expired(start_time, max_time)) {
			        	break;
		    		}

					vector<NodeSource> primal_agm = remove_lambdas(primal_agm_with_lambdas, lambdas);
					cost_primal = calculate_cost(primal_agm);

					if (time_expired(start_time, max_time)) {
			        	break;
		    		}

					if (cost_primal < best_primal) {
						best_primal = cost_primal;
						best_agm = primal_agm;
						if(best_dual > best_primal) {
							best_dual = best_primal;
							no_progress = 0;
						}
					}
				} else {
					break;
				}
			}

			if (cost_dual_with_lambdas > best_dual && cost_dual_with_lambdas < best_primal) {
				best_dual = cost_dual_with_lambdas;
				no_progress = 0;
			} else {
				no_progress++;
			}

	    	if (no_progress > 0 && no_progress % 15 == 0) {
	    		alpha = alpha / 1.2;
	    	}

			vector<float> new_lambdas = update_lambdas(lambdas, dual_agm_with_lambdas, cost_dual_with_lambdas, best_primal);

			if (time_expired(start_time, max_time)) {
		       	break;
	    	}

			lambdas = new_lambdas;
		} else {
			break;
		}
	}
}

void grasp(Graph *g) {
	for (int i = 0; i < g->E * 10; i++) {
		if (time_expired(start_time, max_time)) {
        	break;
    	}

		pair< bool, vector<NodeSource> > result = agm_grasp(g->V, g->E, g->adjacency, 0.01, start_time, max_time);

		if (result.first == true) {
			float cost = calculate_cost(result.second);

			if (best_primal == 0 || cost < best_primal) {
           		best_primal = cost;
           		best_agm = result.second;
        	}
		} else {
			break;
		}
	}
}

int main(int argc, char *argv[]) {
	start_time = clock();

	char *file_name = argv[1];
	int time = atoi(argv[2]);
	char *method = argv[3];

	Graph g = read_file(file_name);
	max_time = time;

	if (string(method) == "l") {
		lagrangean_relaxation(&g);
		printf("%s,%.4f,%.4f\n", file_name, best_dual, best_primal);
	} else {
		grasp(&g);
		printf("%s,%.4f\n", file_name, best_primal);
	}

   // clock_t finish_time = clock();
   // double total_time_execution = ((finish_time - start_time) / (float)CLOCKS_PER_SEC);
   // printf("%.2f\n", total_time_execution);

	save_output(file_name, best_agm);

	return 0;
}
