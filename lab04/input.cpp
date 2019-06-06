#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>

using namespace std;

struct Node {
	int id;
	int max_degree;
};

struct Edge {
	int src;
	int dest;
	int cost;
};

struct NodeDestiny {
	int id;
	int cost;
};

struct NodeSource {
	int max_degree;
	vector<NodeDestiny> adj;
};

vector<NodeSource> read_file(char *name) {
	ifstream file; 
	file.open(name);

	int nodes;
	int edges;
	file >> nodes >> edges;

	vector<NodeSource> adjacency(nodes);

	vector<Edge> edges_list(edges);

	for (int i = 0; i < edges; i++) {
		Edge edge;
		int u, v, cost;
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

	// for (int i = 0; i < nodes; i++) {
	// 	cout << "id: " << i << " max_degree: " << adjacency[i].max_degree << endl;
	// 	cout << "lista de adjacentes: ";

	// 	for (int j = 0; j < adjacency[i].adj.size(); j++) {
	// 		cout << "(" << adjacency[i].adj[j].id << ", " << adjacency[i].adj[j].cost << ") ";
	// 	}
	// 	cout << endl << endl;
	// }
	
	return adjacency;
}