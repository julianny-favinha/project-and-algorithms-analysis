#include "agm.hpp"


// calcula custo de uma lista de adjacencias
int calculate_cost(vector<NodeSource> adjacency) {
	int cost = 0;
	int nodes = adjacency.size();

	for (int i = 0; i < nodes; i++) {
		for (int j = 0; j < adjacency[i].adj.size(); j++) {
			if (i < adjacency[i].adj[j].id) {
				cost += adjacency[i].adj[j].cost;
			}
		}
	}

	return cost;
}

// cria um vector de arestas
vector< pair< pair<int, int>, int > > create_adjacency_edges(vector<NodeSource> adjacency) {
    vector< pair< pair<int, int>, int > > adjacency_edges;

    for (int u = 0; u < adjacency.size(); u++) {
        for (int v = 0; v < adjacency[u].adj.size(); v++) {
            if (u < adjacency[u].adj[v].id) {
                adjacency_edges.push_back(make_pair(make_pair(u, adjacency[u].adj[v].id), adjacency[u].adj[v].cost));
            }
        }
    }

    return adjacency_edges;
}

// transforma um vector de arestas em um vector de NodeSource
vector<NodeSource> transform(vector<NodeSource> adjacency, vector< pair< pair<int, int>, int > > agm_edges, int nodes) {
    vector<NodeSource> agm_adjacency(nodes);

    for (int i = 0; i < nodes; i++) {
        NodeSource u;
        u.max_degree = adjacency[i].max_degree;
        agm_adjacency[i] = u;
    }

    for (int i = 0; i < agm_edges.size(); i++) {
        if (agm_edges[i].first.first < agm_edges[i].first.second) {
            NodeDestiny v1;
            v1.id = agm_edges[i].first.second;
            v1.cost = agm_edges[i].second;

            agm_adjacency[agm_edges[i].first.first].adj.push_back(v1);

            NodeDestiny v2;
            v2.id = agm_edges[i].first.first;
            v2.cost = agm_edges[i].second;

            agm_adjacency[agm_edges[i].first.second].adj.push_back(v2);
        }
    }

    return agm_adjacency;
}

// ordena custo crescentemente
bool sort_cost_ascending(const pair< pair<int, int>, int > &rhs, const pair< pair<int, int>, int > &lhs) {
    return rhs.second < lhs.second;
}

void make_set(vector<int> *c) {
    for (int i = 0; i < c->size(); i++) {
        (*c)[i] =  i;
    }
}

int find_set(vector<int> c, int i) {
    if (i == c[i]) {
        return i;
    }

    return find_set(c, c[i]);
}

void union_set(vector<int> *c, int i, int j) {
    int i1 = find_set(*c, i);
    int j1 = find_set(*c, j);

    (*c)[j1] = i1;
}

vector<int> find_vertices(vector<int> c, int component) {
    vector<int> vertices;
    for (int i = 0; i < c.size(); i++) {
        if (c[i] == component) {
            vertices.push_back(i);
        }
    }

    return vertices;
}

int sum_degrees(vector<NodeSource> adjacency, vector<int> vertices_ci) {
    int sum = 0;

    for (int i = 0; i < vertices_ci.size(); i++) {
        sum += adjacency[vertices_ci[i]].max_degree;
    }

    return sum;
}

int calculate_edges_count(vector<NodeSource> adjacency) {
    int count = 0;

    for (int u = 0; u < adjacency.size(); u++) {
        for (int v = 0; v < adjacency[u].adj.size(); v++) {
            if (u < adjacency[u].adj[v].id) {
                count++;
            }
        }
    }

    return count;
}

// arvore geradora minima com restricao de grau dos vertices
vector<NodeSource> agm_with_degree_restriction(vector<NodeSource> adjacency) {
    int nodes_count = adjacency.size();
    int edges_count = calculate_edges_count(adjacency);
    vector< pair< pair<int, int>, int > > adjacency_edges = create_adjacency_edges(adjacency);
    vector<int> degree(nodes_count, 0);
    vector<int> component(nodes_count);
    vector<int> degrees_component(nodes_count, 0);
    vector< pair< pair<int, int>, int > > edges;

    make_set(&component);

    // ordena de acordo com o custo da aresta
    sort(adjacency_edges.begin(), adjacency_edges.end(), sort_cost_ascending);

    int k = 0;

    // uma arvore tem V-1 arestas, e k nao deve ultrapassar o maximo de arestas de adjacency
    while (edges.size() < nodes_count - 1 && k < edges_count) {
        int i = adjacency_edges[k].first.first;
        int j = adjacency_edges[k].first.second;

        if (degree[i] < adjacency[i].max_degree && degree[j] < adjacency[j].max_degree) {
            int ci = find_set(component, i);
            int cj = find_set(component, j);

            if (ci != cj) {
                if (edges.size() == nodes_count - 2) {
                    edges.push_back(adjacency_edges[k]);

                    union_set(&component, i, j);
                    int sum_union = degrees_component[i] + degrees_component[j] + 2;
                    degrees_component[i] = sum_union;
                    degrees_component[j] = sum_union; 
                } else {
                    vector<int> vertices_ci = find_vertices(component, ci);
                    vector<int> vertices_cj = find_vertices(component, cj);

                    int sum_degrees_components = sum_degrees(adjacency, vertices_ci) + sum_degrees(adjacency, vertices_cj);

                    if (degrees_component[i] + degrees_component[j] + 2 < sum_degrees_components) {
                        edges.push_back(adjacency_edges[k]);

                        union_set(&component, i, j);

                        int sum_union = degrees_component[i] + degrees_component[j] + 2;
                        degrees_component[i] = sum_union;
                        degrees_component[j] = sum_union; 

                        degree[i]++;
                        degree[j]++;
                    }
                }
            }
        }

        k++;
    }

    vector<NodeSource> agm_adjacency = transform(adjacency, edges, nodes_count);

    // print_graph(agm_adjacency);

    return agm_adjacency;
}

// arvore geradora minima
vector<NodeSource> agm(vector<NodeSource> adjacency) {
	int nodes = adjacency.size();
    vector< pair< pair<int, int>, int > > adjacency_edges = create_adjacency_edges(adjacency);
    vector<int> component(nodes);

    make_set(&component);
    
    // ordena de acordo com o custo da aresta
    sort(adjacency_edges.begin(), adjacency_edges.end(), sort_cost_ascending);

    int k = 0;
    vector< pair< pair<int, int>, int > > agm_edges;

    while (agm_edges.size() < nodes - 1) {
        int i = adjacency_edges[k].first.first;
        int j = adjacency_edges[k].first.second;

        int ci = find_set(component, i);
        int cj = find_set(component, j);

        if (ci != cj) {
            agm_edges.push_back(adjacency_edges[k]);
            union_set(&component, i, j);
        }

        k++;
    }

    vector<NodeSource> agm_adjacency = transform(adjacency, agm_edges, nodes);

    // print_graph(agm_adjacency);

    return agm_adjacency;
}