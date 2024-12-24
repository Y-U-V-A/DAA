#include "dfs.h"
#include "zmemory.h"
#include "clock.h"
#include "graph_generator.h"
#include "logger.h"
#include "zmemory.h"
#include "utils.h"

void dfs(u32 node, u32 nodes, u8** graph, u8* vis) {
    vis[node] = 1;
    LOGZ(" %u ", node);
    for (u32 i = 0; i < nodes; ++i) {
        if (!vis[i] && graph[node][i]) {
            dfs(i, nodes, graph, vis);
        }
    }
}

void dfs_helper(u32 nodes, u8** graph) {

    u32 component = 0;
    u8 vis[nodes];
    for (u32 i = 0; i < nodes; ++i) {
        vis[i] = 0;
    }

    for (u32 i = 0; i < nodes; ++i) {
        if (!vis[i]) {
            component += 1;
            LOGZ("component %u dfs = ", component);
            dfs(i, nodes, graph, vis);
            LOGZ("\n");
        }
    }
}

void dfs_run() {

    random_seed();

    LOGD("running dfs ");

    u32 testcases;
    LOGD("enter no of random test cases to run ");
    input("%u", &testcases);

    u32 max_nodes;
    LOGD("enter max_nodes for undirected weighted graph ");
    input("%u", &max_nodes);

    f32 edge_probability;
    LOGD("enter edge_probability for undirected graph edges");
    input("%f", &edge_probability);

    u8** graph = (u8**)zmemory_allocate(max_nodes * sizeof(u8*), MEMORY_TAG_ALGORITHM);
    for (u32 i = 0; i < max_nodes; ++i) {
        graph[i] = (u8*)zmemory_allocate(max_nodes * sizeof(u8), MEMORY_TAG_ALGORITHM);
    }

    f64 time_taken = 0.0;

    for (u32 i = 0; i < testcases; ++i) {

        u32 nodes = (u32)random_int(1, max_nodes);

        generate_undirected_graph(nodes, edge_probability, graph);

        LOGZ("undirected graph of nodes = %u  \n", nodes);
        for (u32 j = 0; j < nodes; ++j) {
            for (u32 k = 0; k < nodes; ++k) {

                LOGZ(" %u ", graph[j][k]);
            }
            LOGZ("\n");
        }

        clock clk;
        clock_set(&clk);
        dfs_helper(nodes, graph);
        clock_update(&clk);
        time_taken += clk.elapsed;
    }

    for (u32 i = 0; i < max_nodes; ++i) {
        zmemory_free(graph[i], max_nodes * sizeof(u8), MEMORY_TAG_ALGORITHM);
    }
    zmemory_free(graph, max_nodes * sizeof(u8*), MEMORY_TAG_ALGORITHM);

    LOGD("average time_s taken %lfs\n", time_taken / testcases);
}