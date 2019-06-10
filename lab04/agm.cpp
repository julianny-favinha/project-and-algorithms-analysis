#include "agm.hpp"

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

// arvore geradora minima com restricao de grau dos vertices
vector<NodeSource> agm_with_degree_restriction(vector<NodeSource> adjacency) {
    int nodes = adjacency.size();
    vector< pair< pair<int, int>, int > > adjacency_edges;
    vector<int> degree(nodes, 0);
    vector<int> component(nodes);
    vector<int> degrees_component(nodes, 0);
    vector< pair< pair<int, int>, int > > edges;

    make_set(&component);
    
    // cria vector de arestas
    for (int u = 0; u < adjacency.size(); u++) {
        for (int v = 0; v < adjacency[u].adj.size(); v++) {
            if (u < adjacency[u].adj[v].id) {
                adjacency_edges.push_back(make_pair(make_pair(u, adjacency[u].adj[v].id), adjacency[u].adj[v].cost));
            }
        }
    }

    // ordena de acordo com o custo da aresta
    sort(adjacency_edges.begin(), adjacency_edges.end(), sort_cost_ascending);

    int k = 0;
    vector< pair< pair<int, int>, int > > agm_edges;

    while (edges.size() < nodes - 1) {
        int i = adjacency_edges[k].first.first;
        int j = adjacency_edges[k].first.second;

        if (degree[i] < adjacency[i].max_degree && degree[j] < adjacency[j].max_degree) {
            int ci = find_set(component, i);
            int cj = find_set(component, j);

            agm_edges = edges;
            agm_edges.push_back(adjacency_edges[k]);

            if (ci != cj) {
                if (agm_edges.size() == nodes - 1) {
                    edges = agm_edges;

                    union_set(&component, i, j);
                    int sum_union = degrees_component[i] + degrees_component[j] + 2;
                    degrees_component[i] = sum_union;
                    degrees_component[j] = sum_union; 
                } else {
                    vector<int> vertices_ci = find_vertices(component, ci);
                    vector<int> vertices_cj = find_vertices(component, cj);

                    int sum_degrees_components = sum_degrees(adjacency, vertices_ci) + sum_degrees(adjacency, vertices_cj);

                    if (degrees_component[i] + degrees_component[j] + 2 < sum_degrees_components) {
                        edges = agm_edges;

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

    // print_graph(agm_adjacency);

    return agm_adjacency;
}

// busca o subconjunto de um elemento i
int buscar(int subset[], int i) {
    if (subset[i] == -1) {
        return i;
    }

    return buscar(subset, subset[i]);
}

// une dois subconjuntos em um único subconjunto
void unir(int subset[], int v1, int v2) {
    int v1_set = buscar(subset, v1);
    int v2_set = buscar(subset, v2);
    subset[v1_set] = v2_set;
}

// arvore geradora minima
vector<NodeSource> agm(vector<NodeSource> adjacency) {
	vector<pair<pair<int, int>, int > > arvore;
	map<pair<int, int>, int >::iterator it;
	map<pair<int, int>, int > edge_map;

	// for em todos os nos pra popular o mapa
	for (int u = 0; u < adjacency.size(); u++) {
		for (int v = 0; v < adjacency[u].adj.size(); v++) {
			if (edge_map.count(make_pair(adjacency[u].adj[v].id, u)) == 0) {
				edge_map.insert(make_pair(make_pair(u,adjacency[u].adj[v].id), adjacency[u].adj[v].cost)); 
			}
		}
	}

	// ordena os edges crescentemente
    auto cmp = [](const auto &rhs, const auto &lhs) {
        return rhs.second < lhs.second;
    };

    set < pair<pair<int, int>, int >, decltype( cmp )> s(edge_map.begin(), edge_map.end(), cmp);

	int *subset = new int[adjacency.size()];
	memset(subset, -1, sizeof(int) * adjacency.size());

	for (const auto& kv : s) {
		int v1 = buscar(subset, kv.first.first);
		int v2 = buscar(subset, kv.first.second);

		// se forem diferentes é porque NÃO forma ciclo, insere no vetor
		if (v1 != v2) {
            arvore.push_back(kv);
			unir(subset, v1, v2);
		}
	}

    vector<NodeSource> arvoreMinima(adjacency.size());

    for (int i = 0; i < adjacency.size(); i++) {
        NodeSource u;
        u.max_degree = -1;
        arvoreMinima[i] = u;
    }

    for (const auto& no : arvore) {
        if (arvoreMinima[no.first.first].max_degree == -1) {
            arvoreMinima[no.first.first].max_degree = adjacency[no.first.first].max_degree;
            NodeDestiny v;
            v.id = no.first.second;
            v.cost = no.second;
            arvoreMinima[no.first.first].adj.push_back(v);
        } else {
            NodeDestiny v;
            v.id = no.first.second;
            v.cost = no.second;
            arvoreMinima[no.first.first].adj.push_back(v);
        }
        
        if (arvoreMinima[no.first.second].max_degree == -1) {
            arvoreMinima[no.first.second].max_degree = adjacency[no.first.second].max_degree;
            NodeDestiny v;
            v.id = no.first.first;
            v.cost = no.second;
            arvoreMinima[no.first.second].adj.push_back(v);
        } else {
            NodeDestiny v;
            v.id = no.first.first;
            v.cost = no.second;
            arvoreMinima[no.first.second].adj.push_back(v);
        }
    }

    // for (const auto& no : arvoreMinima) { 
    //     cout << "Max degree desse nó: " << no.max_degree << endl;
    //     for (const auto& ad : no.adj) {
    //         cout << "Id: " << ad.id << endl;
    //         cout << "Custo: " << ad.cost << endl;
    //     }
    // }

	return arvoreMinima; 
}