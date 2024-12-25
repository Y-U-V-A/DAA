#include "includes.h"
int main() { zmemory_init(); zmemory_log(); logger_init(1024 * 1024); _run(); logger_shutdown(); zmemory_log(); zmemory_destroy(); return 0; }
