#include "grasp.hpp"

// arvore geradora minima com restricao de grau dos vertices
Agm agm_grasp(int V, int E, vector<NodeSource> adjacency, vector< pair< pair<int, int>, float > > adjacency_edges, float interval) {
    vector<int> degree(V, 0);
    vector<int> component(V);
    vector<int> degrees_component(V, 0);
    vector< pair< pair<int, int>, float > > edges;
    Agm result;

    make_set(&component);
    result.cost = 0;

    // ordena de acordo com o custo da aresta
    vector< pair< pair<int, int>, float > > agm_edges;
    vector< pair< pair<int, int>, float > > adjacency_edges_copy = adjacency_edges;

    int numericInterval = (int) adjacency_edges.size() * interval;
    int beginInterval = 0;

    for (int i = 0; i < E; i++) {
        int rand_index = rand() % (numericInterval - beginInterval) + beginInterval;
        pair< pair<int, int>, float > edge = adjacency_edges_copy[rand_index];
        adjacency_edges_copy[rand_index] = adjacency_edges_copy[i];
        adjacency_edges_copy[i] = edge;
        beginInterval++;
        if (numericInterval < E) {
            numericInterval++;
        }
    }

    int k = 0;
    while (edges.size() < V - 1 && k < E) {
        int i = adjacency_edges_copy[k].first.first;
        int j = adjacency_edges_copy[k].first.second;

        if (degree[i] < adjacency[i].max_degree && degree[j] < adjacency[j].max_degree) {
            int ci = find_set(component, i);
            int cj = find_set(component, j);

            if (ci != cj) {
                if (edges.size() == V - 2) {
                    edges.push_back(adjacency_edges_copy[k]);

                    union_set(&component, i, j);
                    int sum_union = degrees_component[i] + degrees_component[j] + 2;
                    degrees_component[i] = sum_union;
                    degrees_component[j] = sum_union; 
                } else {
                    vector<int> vertices_ci = find_vertices(component, ci);
                    vector<int> vertices_cj = find_vertices(component, cj);

                    int sum_degrees_components = sum_degrees(adjacency, vertices_ci) + sum_degrees(adjacency, vertices_cj);

                    if (degrees_component[i] + degrees_component[j] + 2 < sum_degrees_components) {
                        edges.push_back(adjacency_edges_copy[k]);

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

    result.adjacency = transform(adjacency, edges, V);
    result.cost  = calculate_cost(result.adjacency);

    return result;
}
