#include "Trace.h"
#include "Cache.h"
#include "pbPlots.h"
#include "supportLib.h"

extern TraceParser *initTraceParser(const char * mem_file);
extern bool getRequest(TraceParser *mem_trace);

extern Cache *initCache(unsigned block_size,unsigned cache_size,unsigned assoc);
extern bool accessBlock(Cache *cache, Request *req, uint64_t access_time);
extern bool insertBlock(Cache *cache, Request *req, uint64_t access_time, uint64_t *wb_addr);

int main(int argc, const char *argv[])
{	
    /*if (argc != 2)
    {
        printf("Usage: %s %s\n", argv[0], "<mem-file>");

        return 0;
    }*/

    for(int i=1;i<=8;i=i*2)
    {


        // Initialize a CPU trace parser
        TraceParser *mem_trace = initTraceParser(argv[1]);

        // Initialize a Cache
        Cache *cache = initCache(512,4096,2*i);
        
        // Running the trace
        uint64_t num_of_reqs = 0;
        uint64_t hits = 0;
        uint64_t misses = 0;
        uint64_t num_evicts = 0;

        uint64_t cycles = 0;
        while (getRequest(mem_trace))
        {
            // Step one, accessBlock()
            if (accessBlock(cache, mem_trace->cur_req, cycles))
            {
                // Cache hit
                hits++;
            }
            else
            {
                // Cache miss!
                misses++;
                // Step two, insertBlock()
    //            printf("Inserting: %"PRIu64"\n", mem_trace->cur_req->load_or_store_addr);
                uint64_t wb_addr;
                if (insertBlock(cache, mem_trace->cur_req, cycles, &wb_addr))
                {
                    num_evicts++;
    //                printf("Evicted: %"PRIu64"\n", wb_addr);
                }
            }

            ++num_of_reqs;
            ++cycles;
        }

        double miss_rate = (double)misses / ((double)hits + (double)misses);
        printf("Miss rate: %lf%%\n", miss_rate * 100);
    }
    int x = 0;
    x = plotGraph();
    printf("Press Any Key to Continue\n");
    getchar();

}
