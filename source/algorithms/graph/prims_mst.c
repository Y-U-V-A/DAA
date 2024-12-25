#include "prims_mst.h"
#include "zmemory.h"
#include "clock.h"
#include "graph_generator.h"
#include "logger.h"
#include "zmemory.h"
#include "utils.h"
#include "priority_queue.h"

typedef struct prims_mst_pair {
    u32 node;
    u32 weight;
} prims_mst_pair;

bool prims_mst_priority_cmp_func(const void* parent, const void* child) {
    return ((const prims_mst_pair*)parent)->weight < ((const prims_mst_pair*)child)->weight;
}

void prims_mst(u32 start_node, u32 nodes, u32** graph, u32* vis) {
    priority_queue* que = priority_queue_create(prims_mst_pair, prims_mst_priority_cmp_func);

    prims_mst_pair NODE = {.node = start_node, .weight = 0};
    priority_queue_push(que, &NODE);
    u32 min_edge_wt = 0;

    while (priority_queue_length(que) != 0) {

        prims_mst_pair node = *(prims_mst_pair*)priority_queue_top(que);
        priority_queue_pop(que);

        if (vis[node.node]) {
            continue;
        }

        vis[node.node] = 1;
        min_edge_wt += node.weight;

        for (u32 i = 0; i < nodes; ++i) {
            if (graph[node.node][i]) {
                NODE.node = i;
                NODE.weight = graph[node.node][i];
                priority_queue_push(que, &NODE);
            }
        }
    }

    LOGZ(" %u ", min_edge_wt);

    priority_queue_destroy(que);
}

void prims_mst_helper(u32 nodes, u32** graph) {
    u32 component = 0;
    u32 vis[nodes];
    for (u32 i = 0; i < nodes; ++i) {
        vis[i] = 0;
    }

    for (u32 i = 0; i < nodes; ++i) {
        if (!vis[i]) {
            component += 1;
            LOGZ("component %u mst sum = ", component);
            prims_mst(i, nodes, graph, vis);
            LOGZ("\n");
        }
    }
}

void prims_mst_run() {

    random_seed();

    LOGD("running prims_mst ");

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
        prims_mst_helper(nodes, graph);
        clock_update(&clk);
        time_taken += clk.elapsed;
    }

    for (u32 i = 0; i < max_nodes; ++i) {
        zmemory_free(graph[i], max_nodes * sizeof(u32), MEMORY_TAG_ALGORITHM);
    }
    zmemory_free(graph, max_nodes * sizeof(u32*), MEMORY_TAG_ALGORITHM);

    LOGD("average time_s taken %lfs\n", time_taken / testcases);
}