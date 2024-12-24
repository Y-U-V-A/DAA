#ifndef GRAPH_GENERATOR__H
#define GRAPH_GENERATOR__H

#include "defines.h"

void generate_directed_graph(u32 nodes, f32 edge_probability, u8** out_graph);

void generate_directed_acyclic_graph(u32 nodes, f32 edge_probability, u8** out_graph);

void generate_undirected_graph(u32 nodes, f32 edge_probability, u8** out_graph);

void generate_directed_weighted_graph(u32 nodes, u32 max_weight, f32 edge_probability, u32** out_graph);

void generate_undirected_weighted_graph(u32 nodes, u32 max_weight, f32 edge_probability, u32** out_graph);

#endif