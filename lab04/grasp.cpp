#include "grasp.hpp"

// arvore geradora minima com restricao de grau dos vertices
vector<NodeSource> agm_grasp(int V, int E, vector<NodeSource> adjacency, float interval, int times) {
    vector< pair< pair<int, int>, int > > adjacency_edges = create_adjacency_edges(adjacency);
    vector<int> degree(V, 0);
    vector<int> component(V);
    vector<int> degrees_component(V, 0);
    vector< pair< pair<int, int>, int > > edges;
    int better_cost = 0;
    vector<NodeSource> best_agm_adjacency;

    make_set(&component);

    // ordena de acordo com o custo da aresta
    sort(adjacency_edges.begin(), adjacency_edges.end(), sort_cost_ascending);

    for (int i = 0; i < times; i++) {

        vector< pair< pair<int, int>, int > > agm_edges;

        int numericInterval = (int) adjacency_edges.size() * interval;
        int beginInterval = 0;
        vector< pair< pair<int, int>, int > > random_adjacency_edges;

        for (int i = 0; i < adjacency_edges.size(); i++) {
            int rand_index = rand() % numericInterval + beginInterval;
            random_adjacency_edges.push_back(adjacency_edges[rand_index]);
            beginInterval++;
            if (numericInterval < adjacency_edges.size()) {
                numericInterval++;
            }
        }

        int k = 0;
        while (edges.size() < V - 1 && k < E) {
            int i = random_adjacency_edges[k].first.first;
            int j = random_adjacency_edges[k].first.second;

            if (degree[i] < adjacency[i].max_degree && degree[j] < adjacency[j].max_degree) {
                int ci = find_set(component, i);
                int cj = find_set(component, j);

                if (ci != cj) {
                    if (edges.size() == V - 2) {
                        edges.push_back(random_adjacency_edges[k]);

                        union_set(&component, i, j);
                        int sum_union = degrees_component[i] + degrees_component[j] + 2;
                        degrees_component[i] = sum_union;
                        degrees_component[j] = sum_union; 
                    } else {
                        vector<int> vertices_ci = find_vertices(component, ci);
                        vector<int> vertices_cj = find_vertices(component, cj);

                        int sum_degrees_components = sum_degrees(adjacency, vertices_ci) + sum_degrees(adjacency, vertices_cj);

                        if (degrees_component[i] + degrees_component[j] + 2 < sum_degrees_components) {
                            edges.push_back(random_adjacency_edges[k]);

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

        vector<NodeSource> agm_adjacency = transform(adjacency, edges, V);
        int solution_cost = calculate_cost(agm_adjacency);

        if (better_cost == 0 || solution_cost < better_cost) {
            best_agm_adjacency = agm_adjacency;
            better_cost = solution_cost;
        }
    }

    // print_graph(best_agm_adjacency);

    return best_agm_adjacency;
}
