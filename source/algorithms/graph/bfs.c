#include "bfs.h"
#include "zmemory.h"
#include "clock.h"
#include "graph_generator.h"
#include "logger.h"
#include "zmemory.h"
#include "queue.h"
#include "utils.h"

void bfs(u32 start_node, u32 nodes, u8** graph, u8* vis) {

    queue* que = queue_create(u32);

    queue_push(que, &start_node);
    vis[start_node] = 1;
    u32 node;
    while (queue_length(que) != 0) {
        node = *(u32*)queue_front(que);
        queue_pop(que);
        LOGZ(" %u ", node);
        for (u32 i = 0; i < nodes; ++i) {
            if (!vis[i] && graph[node][i]) {
                queue_push(que, &i);
                vis[i] = 1;
            }
        }
    }

    queue_destroy(que);
}

void bfs_helper(u32 nodes, u8** graph) {

    u32 component = 0;
    u8 vis[nodes];
    for (u32 i = 0; i < nodes; ++i) {
        vis[i] = 0;
    }

    for (u32 i = 0; i < nodes; ++i) {
        if (!vis[i]) {
            component += 1;
            LOGZ("component %u bfs = ", component);
            bfs(i, nodes, graph, vis);
            LOGZ("\n");
        }
    }
}

void bfs_run() {

    random_seed();

    LOGD("running bfs ");

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
        bfs_helper(nodes, graph);
        clock_update(&clk);
        time_taken += clk.elapsed;
    }

    for (u32 i = 0; i < max_nodes; ++i) {
        zmemory_free(graph[i], max_nodes * sizeof(u8), MEMORY_TAG_ALGORITHM);
    }
    zmemory_free(graph, max_nodes * sizeof(u8*), MEMORY_TAG_ALGORITHM);

    LOGD("average time_s taken %lfs\n", time_taken / testcases);
}