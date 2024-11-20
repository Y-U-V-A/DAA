#include "bfs.h"
#include "common.h"
#include "clock.h"
#include "graph_generator.h"
#include "logger.h"
#include "common.h"
#include "queue.h"

void bfs(const u32 nodes, const u8** graph, u32* out_bfs) {

    queue* que = queue_create(u32);

    u32 node = 0;
    queue_push(que, &node);

    u8 vis[nodes];
    for (u32 i = 0; i < nodes; ++i) {
        vis[i] = 0;
    }

    vis[0] = 1;

    u32 idx = 0;

    while (queue_size(que) != 0) {
        node = *(u32*)queue_front(que);
        queue_pop(que);
        out_bfs[idx++] = node;

        for (u32 i = 0; i < nodes; ++i) {
            if (!vis[i] && graph[node][i]) {
                queue_push(que, &i);
                vis[i] = 1;
            }
        }
    }

    queue_destroy(que);
}

void bfs_run() {

    LOGD("running bfs ");
    i32 n;
    LOGD("enter no of generate_random test cases to run");
    scan("%d", &n);
    LOGD("n = %d", n);
    f32 sparsity;
    LOGD("enter sparsity ");
    scan("%f", &sparsity);
    LOGD("sparsity = %f", sparsity);

    seed_random();

    clock clk;
    clock_start(&clk);

    const u32 max_nodes = 20;
    u32 array[max_nodes];

    u8** graph = (u8**)memory_allocate(max_nodes * sizeof(u8*), MEMORY_TAG_ALGORITHM);
    for (u32 i = 0; i < max_nodes; ++i) {
        graph[i] = (u8*)memory_allocate(max_nodes * sizeof(u8), MEMORY_TAG_ALGORITHM);
    }

    const u32 buff_size = 1024 * 1024; // 1 mb
    char* buffer = memory_allocate(buff_size * sizeof(char), MEMORY_TAG_UNKNOWN);

    for (i32 i = 0; i < n; ++i) {

        u32 nodes = generate_random(max_nodes);

        if (nodes == 0) {
            continue;
        }

        generate_undirected_graph(nodes, sparsity, graph);

        clock clk;
        clock_start(&clk);

        bfs(nodes, (const u8**)graph, array);

        clock_update(&clk);

        u32 offset = log_buffer(buffer, buff_size, "graph of nodes = %u  \n", nodes);

        for (u32 i = 0; i < nodes; ++i) {
            for (u32 j = 0; j < nodes; ++j) {

                offset += log_buffer(buffer + offset, buff_size - offset, " %u ", graph[i][j]);
            }
            offset += log_buffer(buffer + offset, buff_size - offset, "\n");
        }

        offset += log_buffer(buffer + offset, buff_size - offset, "bfs = ");

        for (u32 i = 0; i < nodes; ++i) {
            offset += log_buffer(buffer + offset, buff_size - offset, " %u ", array[i]);
        }

        offset += log_buffer(buffer + offset, buff_size - offset, "\ntime = %lfs", clk.elapsed);
        buffer[offset++] = '\n';
        buffer[offset++] = '\0';

        LOGD("%s", buffer);
    }

    clock_update(&clk);

    for (u32 i = 0; i < max_nodes; ++i) {
        memory_free(graph[i], max_nodes * sizeof(u8), MEMORY_TAG_ALGORITHM);
    }
    memory_free(graph, max_nodes * sizeof(u8*), MEMORY_TAG_ALGORITHM);
    memory_free(buffer, buff_size * sizeof(char), MEMORY_TAG_UNKNOWN);

    LOGD("total time_s taken %lfs\n", clk.elapsed);
}