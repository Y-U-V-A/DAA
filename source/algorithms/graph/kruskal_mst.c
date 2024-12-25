#include "kruskal_mst.h"
#include "zmemory.h"
#include "clock.h"
#include "graph_generator.h"
#include "logger.h"
#include "zmemory.h"
#include "utils.h"
#include "disjoint_set.h"
#include "darray.h"

typedef struct kruskal_mst_pair {
    /* data */
    u64 u;
    u64 v;
    u64 weight;
} kruskal_mst_pair;

int kruskal_mst_compare(const void* a, const void* b) {
    const kruskal_mst_pair* arg1 = (const kruskal_mst_pair*)a;
    const kruskal_mst_pair* arg2 = (const kruskal_mst_pair*)b;

    if (arg1->weight < arg2->weight)
        return -1;
    if (arg1->weight > arg2->weight)
        return 1;
    return 0;
}

void kruskal_mst(u32 nodes, u32** graph) {
    kruskal_mst_pair* array = darray_create(kruskal_mst_pair);
    for (u32 i = 0; i < nodes; ++i) {
        for (u32 j = 0; j < nodes; ++j) {
            if (graph[i][j]) {
                kruskal_mst_pair pair = {.u = i, .v = j, .weight = graph[i][j]};
                darray_push_back(array, pair);
            }
        }
    }
    u32 len = darray_length(array);
    builtin_quick_sort(array, len, sizeof(kruskal_mst_pair), kruskal_mst_compare);

    // LOGZ("sorted edges\n");
    // for (u32 i = 0; i < len; ++i) {
    //     LOGZ("(%u,%u),%u\n", array[i].u, array[i].v, array[i].weight);
    // }

    disjoint_set* set = disjoint_set_create(nodes);
    u32 mst_sum = 0;
    for (u32 i = 0; i < len; ++i) {
        if (disjoint_set_find_u_parent(set, array[i].u) != disjoint_set_find_u_parent(set, array[i].v)) {
            disjoint_set_union_by_rank(set, array[i].u, array[i].v);
            mst_sum += array[i].weight;
        }
    }

    LOGZ("kruskal_mst sum including all components = %u\n", mst_sum);

    darray_destroy(array);
    disjoint_set_destroy(set);
}

void kruskal_mst_run() {

    random_seed();

    LOGD("running kruskal_mst ");

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
        kruskal_mst(nodes, graph);
        clock_update(&clk);
        time_taken += clk.elapsed;
    }

    for (u32 i = 0; i < max_nodes; ++i) {
        zmemory_free(graph[i], max_nodes * sizeof(u32), MEMORY_TAG_ALGORITHM);
    }
    zmemory_free(graph, max_nodes * sizeof(u32*), MEMORY_TAG_ALGORITHM);

    LOGD("average time_s taken %lfs\n", time_taken / testcases);
}