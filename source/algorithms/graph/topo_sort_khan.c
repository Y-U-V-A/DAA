#include "topo_sort_khan.h"
#include "zmemory.h"
#include "clock.h"
#include "graph_generator.h"
#include "logger.h"
#include "zmemory.h"
#include "utils.h"
#include "queue.h"

void topo_sort_khan(u32 nodes, u8** graph) {

    u32 in_degrees[nodes];
    for (u32 i = 0; i < nodes; ++i) {
        in_degrees[i] = 0;
    }

    for (u32 i = 0; i < nodes; ++i) {
        for (u32 j = 0; j < nodes; ++j) {
            if (graph[i][j]) {
                in_degrees[j] += 1;
            }
        }
    }

    queue* que = queue_create(u32);

    for (u32 i = 0; i < nodes; ++i) {
        if (in_degrees[i] == 0) {
            queue_push(que, &i);
        }
    }

    u32 count = 0;

    LOGZ("topo sort = ");
    while (queue_length(que) != 0) {
        u32 node = *(u32*)queue_front(que);
        queue_pop(que);
        count += 1;
        LOGZ(" %u ", node);
        for (u32 i = 0; i < nodes; ++i) {
            if (graph[node][i]) {
                in_degrees[i] -= 1;
                if (in_degrees[i] == 0) {
                    queue_push(que, &i);
                }
            }
        }
    }
    LOGZ("\n");

    if (count < nodes) {
        LOGE("provided graph is not DAG");
        return;
    }

    queue_destroy(que);
}

void topo_sort_khan_run() {

    random_seed();

    LOGD("running topo_sort_khan ");

    u32 testcases;
    LOGD("enter no of random test cases to run ");
    input("%u", &testcases);

    u32 max_nodes;
    LOGD("enter max_nodes for undirected weighted graph ");
    input("%u", &max_nodes);

    f32 edge_probability;
    LOGD("enter edge_probability for directed acyclic graph edges");
    input("%f", &edge_probability);

    u8** graph = (u8**)zmemory_allocate(max_nodes * sizeof(u8*), MEMORY_TAG_ALGORITHM);
    for (u32 i = 0; i < max_nodes; ++i) {
        graph[i] = (u8*)zmemory_allocate(max_nodes * sizeof(u8), MEMORY_TAG_ALGORITHM);
    }

    f64 time_taken = 0.0;

    for (u32 i = 0; i < testcases; ++i) {

        u32 nodes = (u32)random_int(1, max_nodes);

        generate_directed_acyclic_graph(nodes, edge_probability, graph);

        LOGZ("directed acyclic graph of nodes = %u  \n", nodes);
        for (u32 j = 0; j < nodes; ++j) {
            for (u32 k = 0; k < nodes; ++k) {

                LOGZ(" %u ", graph[j][k]);
            }
            LOGZ("\n");
        }

        clock clk;
        clock_set(&clk);
        topo_sort_khan(nodes, graph);
        clock_update(&clk);
        time_taken += clk.elapsed;
    }

    for (u32 i = 0; i < max_nodes; ++i) {
        zmemory_free(graph[i], max_nodes * sizeof(u8), MEMORY_TAG_ALGORITHM);
    }
    zmemory_free(graph, max_nodes * sizeof(u8*), MEMORY_TAG_ALGORITHM);

    LOGD("average time_s taken %lfs\n", time_taken / testcases);
}