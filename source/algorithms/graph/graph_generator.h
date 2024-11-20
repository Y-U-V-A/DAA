#ifndef GRAPH_GENERATOR_H
#define GRAPH_GENERATOR_H

#include "defines.h"

void generate_directed_graph(const u32 nodes, const f32 sparsity, u8** graph);

void generate_undirected_graph(const u32 nodes, const f32 sparsity, u8** graph);

void generate_directed_weighted_graph(const u32 nodes, const u32 max_weight, const f32 sparsity, u32** graph);

void generate_undirected_weighted_graph(const u32 nodes, const u32 max_weight, const f32 sparsity, u32** graph);

#endif