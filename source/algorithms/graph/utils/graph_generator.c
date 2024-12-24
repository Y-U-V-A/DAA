#include "graph_generator.h"
#include "logger.h"
#include "zmemory.h"
#include "utils.h"

#define RAND_MAX 0x7fff

void generate_directed_graph(u32 nodes, f32 edge_probability, u8** out_graph) {

    for (u32 i = 0; i < nodes; ++i) {
        for (u32 j = 0; j < nodes; ++j) {
            if (i == j) {
                out_graph[i][j] = 0;
            } else {
                if ((float)random_int(0, RAND_MAX) / RAND_MAX < edge_probability) {
                    out_graph[i][j] = 1;
                } else {
                    out_graph[i][j] = 0;
                }
            }
        }
    }
}

void generate_directed_acyclic_graph(u32 nodes, f32 edge_probability, u8** out_graph) {

    for (u32 i = 0; i < nodes; ++i) {
        for (u32 j = i + 1; j < nodes; ++j) {
            if ((float)random_int(0, RAND_MAX) / RAND_MAX < edge_probability) {
                out_graph[i][j] = 1;
            } else {
                out_graph[i][j] = 0;
            }
        }
    }
}

void generate_undirected_graph(u32 nodes, f32 edge_probability, u8** out_graph) {

    for (u32 i = 0; i < nodes; ++i) {
        for (u32 j = i + 1; j < nodes; ++j) {

            if ((float)random_int(0, RAND_MAX) / RAND_MAX < edge_probability) {
                out_graph[i][j] = 1;
                out_graph[j][i] = 1;
            } else {
                out_graph[i][j] = 0;
                out_graph[j][i] = 0;
            }
        }
        out_graph[i][i] = 0;
    }
}

void generate_directed_weighted_graph(u32 nodes, u32 max_weight, f32 edge_probability, u32** out_graph) {

    for (u32 i = 0; i < nodes; ++i) {
        for (u32 j = 0; j < nodes; ++j) {
            if (i == j) {
                out_graph[i][j] = 0;
            } else {
                if ((float)random_int(0, RAND_MAX) / RAND_MAX < edge_probability) {
                    u32 weight = random_int(0, max_weight) + 1;
                    out_graph[i][j] = weight;
                } else {
                    out_graph[i][j] = 0;
                }
            }
        }
    }
}

void generate_undirected_weighted_graph(u32 nodes, u32 max_weight, f32 edge_probability, u32** out_graph) {

    for (u32 i = 0; i < nodes; ++i) {
        for (u32 j = i + 1; j < nodes; ++j) {
            if ((float)random_int(0, RAND_MAX) / RAND_MAX < edge_probability) {
                u32 weight = random_int(0, max_weight) + 1;
                out_graph[i][j] = weight;
                out_graph[j][i] = weight;
            } else {
                out_graph[i][j] = 0;
                out_graph[j][i] = 0;
            }
        }
        out_graph[i][i] = 0;
    }
}