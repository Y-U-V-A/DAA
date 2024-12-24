#include "algorithms/sorting/bubble.h"
#include "algorithms/sorting/insertion.h"
#include "algorithms/sorting/merge.h"
#include "algorithms/sorting/quick.h"
#include "algorithms/sorting/radix.h"
#include "algorithms/sorting/selection.h"
#include "algorithms/graph/bfs.h"
#include "algorithms/graph/dfs.h"
#include "algorithms/graph/topo_sort_dfs.h"
#include "algorithms/graph/topo_sort_khan.h"
#include "algorithms/graph/kosaraju_strongly_connected.h"
#include "algorithms/graph/dijkstra_shortest_path.h"
#include "algorithms/graph/bellman_ford_shortest_path.h"
#include "algorithms/graph/floyd_warshall_shortest_paths.h"
#include "logger.h"
#include "zmemory.h"

int main() {

    zmemory_init();
    zmemory_log();

    logger_init(1024 * 1024); // 1mb

    floyd_warshall_shortest_paths_run();

    logger_shutdown();

    zmemory_log();
    zmemory_destroy();
}