#include "input.hpp"

// verifica se tempo expirou
bool time_expired(clock_t start_time, int max_time) {
    clock_t finish_time = clock();
    int total_time_execution = (int) ((finish_time - start_time) / (float)CLOCKS_PER_SEC);

    return total_time_execution > max_time;
}

// retorna um vetor de nodes posições com o grau de cada vértice
vector<int> calculate_degrees(vector<NodeSource> adjacency) {
	vector<int> degrees(adjacency.size());

	for (int v = 0; v < degrees.size(); v++) {
		degrees[v] = adjacency[v].max_degree;
	}

	return degrees;
}

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

Graph read_file(char *name) {
	ifstream file; 
	file.open(name);

	int nodes;
	int edges;
	file >> nodes >> edges;

	vector<NodeSource> adjacency(nodes);

	vector<Edge> edges_list(edges);

	for (int i = 0; i < edges; i++) {
		Edge edge;
		int u, v;
		float cost;
		file >> u >> v >> cost;
		edge.src = u-1;
		edge.dest = v-1;
		edge.cost = cost;
		edges_list[i] = edge;
	}

	vector<Node> nodes_list(nodes);

	for (int i = 0; i < nodes; i++) {
		Node node;
		int v;
		file >> v >> node.max_degree;
		node.id = v - 1;
		nodes_list[i] = node;
	}

	for (int i = 0; i < nodes; i++) {
		NodeSource u;
		u.max_degree = nodes_list[i].max_degree;

		for (int j = 0; j < edges; j++) {
			NodeDestiny v;

			if (edges_list[j].src == i) {
				v.id = edges_list[j].dest;
				v.cost = edges_list[j].cost;
				u.adj.push_back(v);
			} else {
				if (edges_list[j].dest == i) {
					v.id = edges_list[j].src;
					v.cost = edges_list[j].cost;
					u.adj.push_back(v);
				}
			}
		}

		adjacency[i] = u;
	}

	// print_graph(adjacency);
	
	Graph g;
	g.V = nodes;
	g.E = edges;
	g.adjacency = adjacency;

	return g;
}