#include "dijkstra_shortest_path.h"
#include "zmemory.h"
#include "clock.h"
#include "graph_generator.h"
#include "logger.h"
#include "zmemory.h"
#include "utils.h"
#include "priority_queue.h"

typedef struct dijkstra_shortest_path_pair {
    u32 node;
    u32 weight;
} dijkstra_shortest_path_pair;

bool dijkstra_shortest_path_priority_cmp_func(const void* parent, const void* child) {
    return ((const dijkstra_shortest_path_pair*)parent)->weight < ((const dijkstra_shortest_path_pair*)child)->weight;
}

void dijkstra_shortest_path(u32 nodes, u32** graph) {
    priority_queue* que = priority_queue_create(dijkstra_shortest_path_pair, dijkstra_shortest_path_priority_cmp_func);

    u32 shortest_paths[nodes];

    for (u32 i = 0; i < nodes; ++i) {
        shortest_paths[i] = 10000000;
    }

    shortest_paths[0] = 0;
    dijkstra_shortest_path_pair NODE = {.node = 0, .weight = 0};
    priority_queue_push(que, &NODE);

    while (priority_queue_length(que) != 0) {
        dijkstra_shortest_path_pair node = *(dijkstra_shortest_path_pair*)priority_queue_top(que);
        priority_queue_pop(que);

        for (u32 i = 0; i < nodes; ++i) {
            if (graph[node.node][i] &&
                (node.weight + graph[node.node][i] < shortest_paths[i])) {

                shortest_paths[i] = node.weight + graph[node.node][i];
                NODE.node = i;
                NODE.weight = shortest_paths[i];
                priority_queue_push(que, &NODE);
            }
        }
    }

    LOGZ("dijkstra_shortest_paths from node zero = ");
    for (u32 i = 0; i < nodes; ++i) {
        LOGZ(" %u ", shortest_paths[i]);
    }
    LOGZ("\n");

    priority_queue_destroy(que);
}

void dijkstra_shortest_path_run() {

    random_seed();

    LOGD("running dijkstra_shortest_path ");

    u32 testcases;
    LOGD("enter no of random test cases to run ");
    input("%u", &testcases);

    u32 max_nodes;
    LOGD("enter max_nodes for undirected weighted graph ");
    input("%u", &max_nodes);

    u32 max_weight;
    LOGD("enter max_weight for undirected weighted graph");
    input("%u", &max_weight);

    f32 edge_probability;
    LOGD("enter edge_probability for undirected weighted graph edges");
    input("%f", &edge_probability);

    u32** graph = (u32**)zmemory_allocate(max_nodes * sizeof(u32*), MEMORY_TAG_ALGORITHM);
    for (u32 i = 0; i < max_nodes; ++i) {
        graph[i] = (u32*)zmemory_allocate(max_nodes * sizeof(u32), MEMORY_TAG_ALGORITHM);
    }

    f64 time_taken = 0.0;

    for (u32 i = 0; i < testcases; ++i) {

        u32 nodes = (u32)random_int(1, max_nodes);

        generate_undirected_weighted_graph(nodes, max_weight, edge_probability, graph);

        LOGZ("undirected weighted graph of nodes = %u  \n", nodes);
        for (u32 j = 0; j < nodes; ++j) {
            for (u32 k = 0; k < nodes; ++k) {

                LOGZ(" %u ", graph[j][k]);
            }
            LOGZ("\n");
        }

        clock clk;
        clock_set(&clk);
        dijkstra_shortest_path(nodes, graph);
        clock_update(&clk);
        time_taken += clk.elapsed;
    }

    for (u32 i = 0; i < max_nodes; ++i) {
        zmemory_free(graph[i], max_nodes * sizeof(u32), MEMORY_TAG_ALGORITHM);
    }
    zmemory_free(graph, max_nodes * sizeof(u32*), MEMORY_TAG_ALGORITHM);

    LOGD("average time_s taken %lfs\n", time_taken / testcases);
}