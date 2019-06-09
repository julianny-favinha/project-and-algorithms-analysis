#include "agm.hpp"

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
vector<NodeSource> agm(vector<NodeSource> adjacency) {

	vector<pair<pair<int, int>, int > > arvore;

	map<pair<int, int>, int >::iterator it;
	map<pair<int, int>, int > edge_map;

	// for em todos os nos pra popular o mapa
	for (int u = 0; u < adjacency.size(); u++) {
		for (int v = 0; v < adjacency[u].adj.size(); v++) {
			if (edge_map.count(make_pair(adjacency[u].adj[v].id,u)) == 0) {
				edge_map.insert(make_pair(make_pair(u,adjacency[u].adj[v].id), adjacency[u].adj[v].cost)); 
			}
		}
	}

	// ordena os edges crescentemente

    auto cmp = [](const auto &p1, const auto &p2) {
        return p1.second < p2.second;
    };

    set < pair<pair<int, int>, int >, decltype( cmp )> s(edge_map.begin(), edge_map.end(), cmp);

	int * subset = new int[adjacency.size()];
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
            arvoreMinima[no.first.first].id = adjacency[no.first.first].id
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
            arvoreMinima[no.first.second].id = adjacency[no.first.second].id
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