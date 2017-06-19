/* 

   gcc test_simple.c -o test_simple -lpth 

   t1 runs faster than t2
   main() monitors t1's state
   todo: implement t3 to monitor t1's state,
         use yield() inside main loop (more common). 

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>

#include "pth.h"

static void* t1_func(void *arg)
{
    int i;
    long val;
    for (i = 0; i < 10; i++) {
        val += 100;
        printf("t1_func: loop=%i\n", i );
        // pth_yield(NULL);
        pth_usleep(700); 
    }
    return NULL;
}

static void* t2_func(void *arg)
{
    int i;
    long val;
    for (i = 0; i < 10; i++) {
        val += 100;
        printf("t2_func: loop=%i\n", i );
        pth_usleep(1000); 
    }
    return NULL;
}

int main(int argc, char **argv)
{
    pth_attr_t attr1, attr2;
    pth_t tid1,tid2;
    int rc;
    rc = pth_init();
    tid1 = pth_spawn(PTH_ATTR_DEFAULT, t1_func, (void *)0);
    attr1 = pth_attr_of(tid1);
    pth_attr_set(attr1, PTH_ATTR_JOINABLE, FALSE );
    
    tid2 = pth_spawn(PTH_ATTR_DEFAULT, t2_func, (void *)0);
    attr2 = pth_attr_of(tid2);

    
    for(int i=0;i<6;i++)  {

        long t = pth_ctrl(PTH_CTRL_GETTHREADS);
        printf("main thread. total=%ld\n", t);

        pth_attr_t attr = pth_attr_of(tid1);
        pth_state_t state;
        pth_attr_get(attr, PTH_ATTR_STATE, &state );
        printf("t1 state=%u %s\n", state, state==2 || state==1 ? "RUNNING" : "unknown" );

        pth_usleep(2000); 
        // pth_yield(NULL);

        
    }

}

