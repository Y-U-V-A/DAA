#include "floyd_warshall_shortest_paths.h"
#include "zmemory.h"
#include "clock.h"
#include "graph_generator.h"
#include "logger.h"
#include "zmemory.h"
#include "utils.h"

void floyd_warshall_shortest_paths(u32 nodes, u32** graph) {

    for (u32 k = 0; k < nodes; ++k) {
        u32 shortest_paths[nodes];
        for (u32 i = 0; i < nodes; ++i) {
            shortest_paths[i] = 10000000;
        }

        shortest_paths[k] = 0;

        for (u32 i = 0; i < nodes; ++i) {
            for (u32 j = 0; j < nodes; ++j) {
                if (graph[i][j] && shortest_paths[i] + graph[i][j] < shortest_paths[j]) {
                    shortest_paths[j] = shortest_paths[i] + graph[i][j];
                }
            }
        }

        LOGZ("dijkstra_shortest_paths from node %u = ", k);
        for (u32 i = 0; i < nodes; ++i) {
            LOGZ(" %u ", shortest_paths[i]);
        }
        LOGZ("\n");
    }
}

void floyd_warshall_shortest_paths_run() {

    random_seed();

    LOGD("running floyd_warshall_shortest_paths ");

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
        floyd_warshall_shortest_paths(nodes, graph);
        clock_update(&clk);
        time_taken += clk.elapsed;
    }

    for (u32 i = 0; i < max_nodes; ++i) {
        zmemory_free(graph[i], max_nodes * sizeof(u32), MEMORY_TAG_ALGORITHM);
    }
    zmemory_free(graph, max_nodes * sizeof(u32*), MEMORY_TAG_ALGORITHM);

    LOGD("average time_s taken %lfs\n", time_taken / testcases);
}