#include "kosaraju_strongly_connected.h"
#include "zmemory.h"
#include "clock.h"
#include "graph_generator.h"
#include "logger.h"
#include "zmemory.h"
#include "utils.h"
#include "stack.h"

void dfs_first(u32 node, u32 nodes, u8** graph, u8* vis, stack* stk) {
    vis[node] = 1;
    for (u32 i = 0; i < nodes; ++i) {
        if (!vis[i] && graph[node][i]) {
            dfs_first(i, nodes, graph, vis, stk);
        }
    }
    stack_push(stk, &node);
}
void dfs_second(u32 node, u32 nodes, u8** graph, u8* vis) {
    vis[node] = 1;
    for (u32 i = 0; i < nodes; ++i) {
        if (!vis[i] && graph[node][i]) {
            dfs_second(i, nodes, graph, vis);
        }
    }
}

void kosaraju_strongly_connected(u32 nodes, u8** graph) {

    u8 vis[nodes];
    for (u32 i = 0; i < nodes; ++i) {
        vis[i] = 0;
    }
    stack* stk = stack_create(u32);
    for (u32 i = 0; i < nodes; ++i) {
        if (!vis[i]) {
            dfs_first(i, nodes, graph, vis, stk);
        }
    }

    for (u32 i = 0; i < nodes; ++i) {
        vis[i] = 0;
    }
    // reverse the graph
    for (u32 i = 0; i < nodes; ++i) {
        for (u32 j = 0; j < nodes; ++j) {
            if (graph[i][j]) {
                graph[i][j] = 0;
                graph[j][i] = 1;
            }
        }
    }
    u32 components = 0;
    while (stack_length(stk) != 0) {
        u32 node = *(u32*)stack_top(stk);
        stack_pop(stk);
        if (!vis[node]) {
            dfs_second(node, nodes, graph, vis);
            components += 1;
        }
    }

    LOGZ("kosaraju_strongly_connected_components = %u\n", components);

    stack_destroy(stk);
}

void kosaraju_strongly_connected_run() {

    random_seed();

    LOGD("running kosaraju_strongly_connected ");

    u32 testcases;
    LOGD("enter no of random test cases to run ");
    input("%u", &testcases);

    u32 max_nodes;
    LOGD("enter max_nodes for undirected weighted graph ");
    input("%u", &max_nodes);

    f32 edge_probability;
    LOGD("enter edge_probability for directed graph edges");
    input("%f", &edge_probability);

    u8** graph = (u8**)zmemory_allocate(max_nodes * sizeof(u8*), MEMORY_TAG_ALGORITHM);
    for (u32 i = 0; i < max_nodes; ++i) {
        graph[i] = (u8*)zmemory_allocate(max_nodes * sizeof(u8), MEMORY_TAG_ALGORITHM);
    }

    f64 time_taken = 0.0;

    for (u32 i = 0; i < testcases; ++i) {

        u32 nodes = (u32)random_int(1, max_nodes);

        generate_directed_graph(nodes, edge_probability, graph);

        LOGZ("directed graph of nodes = %u  \n", nodes);
        for (u32 j = 0; j < nodes; ++j) {
            for (u32 k = 0; k < nodes; ++k) {

                LOGZ(" %u ", graph[j][k]);
            }
            LOGZ("\n");
        }

        clock clk;
        clock_set(&clk);
        kosaraju_strongly_connected(nodes, graph);
        clock_update(&clk);
        time_taken += clk.elapsed;
    }

    for (u32 i = 0; i < max_nodes; ++i) {
        zmemory_free(graph[i], max_nodes * sizeof(u8), MEMORY_TAG_ALGORITHM);
    }
    zmemory_free(graph, max_nodes * sizeof(u8*), MEMORY_TAG_ALGORITHM);

    LOGD("average time_s taken %lfs\n", time_taken / testcases);
}