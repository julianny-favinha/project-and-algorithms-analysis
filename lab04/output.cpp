#include "output.hpp"

bool sort_by_sec(const pair<int,int> &rhs, const pair<int,int> &lhs) {
	if (rhs.first == lhs.first) {
		return rhs.second < lhs.second;
	}
	
    return rhs.first < lhs.first;
} 

// grava no arquivo file_name.out a lista de arestas em ordem crescente
// vertices rotulados de 1 a n
// aresta 3 1 deve ser salva como 1 3
void save_output(char *file_name, vector<NodeSource> agm) {
	string buf(file_name);
	ofstream file;
	vector < pair<int, int> > edges;

	for (int i = 0; i < agm.size(); i++) {
		for (int j = 0; j < agm[i].adj.size(); j++) {
			if (i < agm[i].adj[j].id) {
				edges.push_back(make_pair(i+1, agm[i].adj[j].id+1));
			}
		}
	}

	buf.append(".out");
	file.open(buf);

	sort(edges.begin(), edges.end(), sort_by_sec);

	for (int i = 0; i < edges.size(); i++) {
		file << edges[i].first << " " << edges[i].second << endl; 
	}

	file.close();
}