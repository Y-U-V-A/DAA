#include "graph_generator.h"
#include "logger.h"
#include "common.h"

#define RAND_MAX 0x7fff

void generate_directed_graph(const u32 nodes, const f32 sparsity, u8** graph) {
    seed_random();

    for (u32 i = 0; i < nodes; ++i) {
        for (u32 j = 0; j < nodes; ++j) {
            if (i == j) {
                graph[i][j] = 0;
            } else {
                if ((float)generate_random(RAND_MAX) / RAND_MAX < sparsity) {
                    graph[i][j] = 1;
                } else {
                    graph[i][j] = 0;
                }
            }
        }
    }
}

void generate_undirected_graph(const u32 nodes, const f32 sparsity, u8** graph) {

    seed_random();

    for (u32 i = 0; i < nodes; ++i) {
        for (u32 j = i + 1; j < nodes; ++j) {

            if ((float)generate_random(RAND_MAX) / RAND_MAX < sparsity) {
                graph[i][j] = 1;
                graph[j][i] = 1;
            } else {
                graph[i][j] = 0;
                graph[j][i] = 0;
            }
        }
        graph[i][i] = 0;
    }
}

void generate_directed_weighted_graph(const u32 nodes, const u32 max_weight, const f32 sparsity, u32** graph) {
    seed_random();

    for (u32 i = 0; i < nodes; ++i) {
        for (u32 j = 0; j < nodes; ++j) {
            if (i == j) {
                graph[i][j] = 0;
            } else {
                if ((float)generate_random(RAND_MAX) / RAND_MAX < sparsity) {
                    u32 weight = generate_random(max_weight) + 1;
                    graph[i][j] = weight;
                } else {
                    graph[i][j] = 0;
                }
            }
        }
    }
}

void generate_undirected_weighted_graph(const u32 nodes, const u32 max_weight, const f32 sparsity, u32** graph) {
    seed_random();

    for (u32 i = 0; i < nodes; ++i) {
        for (u32 j = i + 1; j < nodes; ++j) {
            if ((float)generate_random(RAND_MAX) / RAND_MAX < sparsity) {
                u32 weight = generate_random(max_weight) + 1;
                graph[i][j] = weight;
                graph[j][i] = weight;
            } else {
                graph[i][j] = 0;
                graph[j][i] = 0;
            }
        }
        graph[i][i] = 0;
    }
}