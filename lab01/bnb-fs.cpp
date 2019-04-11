#include "bnb-fs.hpp"

// nomes dos arquivos de entrada
char *jobs_file_name;
char *params_file_name;

// encontrou uma solucao
bool found_solution = false;

// melhor limitante dual ate o momento
int best_dual = INT_MAX;

// tempo no qual melhor limitante dual foi obtido
clock_t time_best_dual;

// melhor limitante primal ate o momento
int best_primal = INT_MAX;

// tempo no qual melhor limitante primal foi obtido
clock_t time_best_primal;

// tempo de inicio de execucao
clock_t start;

// tempo maximo de execucao
int max_time;

// numero maximo de nos a serem explorados
int max_nodes_count;
int active_nodes_count = 0;

// numero de nos explorados
int nodes_visited_count = 0;

// verifica se tempo expirou
bool time_expired() {
    clock_t finish = clock();
    int total_time_execution = (int) ((finish - start) / (float)CLOCKS_PER_SEC);

    return total_time_execution > max_time;
}

// encontra melhores posicoes para continuar a recursao
vector<int> bound(vector< vector<int> > jobs, int n, vector<int> order, vector<int> remaining) {
    if (time_expired()) {
        print_best_solution();
        exit(0);
    }

    vector<int> best_positions;
    best_positions.reserve(n);

    vector<int> lowers;
    lowers.reserve(remaining.size());

    for (int i = 0; i < (int)remaining.size(); i++) {
        int job = remaining[i];

        vector<int> new_order = order;
        new_order.push_back(job);

        vector<int> new_remaining = remaining;
        new_remaining.erase(new_remaining.begin() + i);

        // encontra os limitantes duais e primais para cada tarefa
        Bounds bounds = estimate_bounds(jobs, n, new_order, new_remaining);

        if (time_expired()) {
            print_best_solution();
            exit(0);
        }

        lowers.push_back(bounds.lower_bound);

        if (bounds.upper_bound < best_primal) {
            best_primal = bounds.upper_bound;
            time_best_primal = clock();
            found_solution = true;
        }
    }

    int best_estimative = best_primal;

    for (int i = 0; i < (int)lowers.size(); i++) {
        int job = remaining[i];

        if (lowers[i] == best_estimative) {
            best_positions.push_back(job);
        } else {
            if (lowers[i] < best_estimative) {
                best_estimative = lowers[i];
                best_positions.clear();
                best_positions.push_back(job);
            }
        }

        if (lowers[i] < best_dual) {
            best_dual = lowers[i];
            time_best_dual = clock();
        }
    }

    return best_positions;
}

void branch(vector< vector<int> > jobs, int n, vector<int> order, vector<int> remaining) {
    if (time_expired()) {
        print_best_solution();
        exit(0);
    }

    if ((int)order.size() < n-1) {
        vector<int> best_positions = bound(jobs, n, order, remaining);

        active_nodes_count += best_positions.size();

        if (active_nodes_count > max_nodes_count) {
            print_best_solution();
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < (int)best_positions.size(); i++) {
            int job = best_positions[i];

            // encontra posicao para remover job
            int index = 0;
            for (index = 0; index < (int)remaining.size(); index++) {
                if (remaining[index] == job) {
                    break;
                }
            }

            order.push_back(job);
            remaining.erase(remaining.begin() + index);

            nodes_visited_count++;
            active_nodes_count -= 1;

            branch(jobs, n, order, remaining);

            order.pop_back();
            remaining.push_back(job);
        }
    } else {
        order.push_back(remaining[0]);
        remaining.clear();

        int total_sum = total_time_sum(jobs, order);

        if (total_sum < best_primal) {
            best_primal = total_sum;
            time_best_primal = clock();
            found_solution = true;
        }
    }
}

void print_best_solution() {
    clock_t finish = clock();
    double total_time_execution = ((finish - start) / (float)CLOCKS_PER_SEC);

    if (found_solution) {
        double total_time_best_dual = ((time_best_dual - start) / (float)CLOCKS_PER_SEC);
        double total_time_best_primal = ((time_best_primal - start) / (float)CLOCKS_PER_SEC);

        printf("%s,%d,%d,%d,%.2f,%.2f,%.2f\n", jobs_file_name, best_primal, best_dual, nodes_visited_count, total_time_best_primal, total_time_best_dual, total_time_execution);
    } else {
        printf("%s,,,%d,,,%.2f\n", jobs_file_name, nodes_visited_count, total_time_execution);
    }
}

int main(int argc, char *argv[]) {
    start = clock();

    jobs_file_name = argv[1];
    params_file_name = argv[2];

    Jobs jobs = read_jobs_file(jobs_file_name);
    Params params = read_params_file(params_file_name);
    
    max_nodes_count = params.max_nodes_count;
    max_time = params.max_time;

    vector<int> order;
    order.reserve(jobs.count);
    vector<int> remaining;
    remaining.reserve(jobs.count);

    for (int i = 0; i < jobs.count; i++) {
        remaining.push_back(i);
    }

    branch(jobs.jobs, jobs.count, order, remaining);

    print_best_solution();

    return 0;
}
