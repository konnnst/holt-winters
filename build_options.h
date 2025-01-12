#ifndef DEBUG
#define DEBUG 0
#endif

// Modes: local, pg
#if MODE == PG
#define FREE free
#define MALLOC malloc
#else 
#define FREE free
#define MALLOC malloc
#endif