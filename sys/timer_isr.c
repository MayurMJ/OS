#include <sys/kprintf.h>
#include <sys/defs.h>
#include <sys/scheduler.h>


uint64_t sleep_timer = 0;
extern int timer_isr;
static int time_boot = 0;
static int num_int = 0;
static int count18 = 1;
char *process_in(int n, char *str) {
	if (n - sleep_timer >= 5) {
		SLEEPING_TASK->state = READY;
		sleep_timer = 0;
	}
	else {
		sleep_timer = n;
	}
	//kprintf("sleep timer %d\n",sleep_timer);
        int64_t num = n;
        if (num == 0) {
                str[14] = '0';
                //str[14] = '\0';
                return str;
        }
        //str[16] = '\0';
        int str_index = 14;
        int is_neg = 0;
        if (num < 0) {
                is_neg = 1;
                num = 0 - num;
        }
        while (num > 0) {
                char c = (num % 10) + '0';
                num = num / 10;
                str[str_index] = c;
		str_index-=2;
        }
        if (is_neg) {
                str[str_index] = '-';
		str_index-=2;
        }
        char *finstr = str + str_index + 2;
        return finstr;
}
void timer_irqhandler(void)
{
    num_int++;
    if(count18 == 1) {
        if(num_int == 18) {
            num_int = 0;
	    count18 = 0;
	    time_boot++;
	}
    }
    else {
        if(num_int == 19) {
            num_int = 0;
	    count18 = 1;
	    time_boot++;
	}
    }
    char * videomem = (char*)(0xffffffff80000000+0xb8f70);
    char * str = "time since boot:";
    //char * itr = (char *) &str[0];
    int i = 0;
    while(str[i]) {
        *videomem = str[i];
        videomem+=2;
        i++;
    }
    videomem = (char*)(0xffffffff80000000+0xb8f9e);
    *videomem = 's';
    videomem = (char*)(0xffffffff80000000+0xb8f8e);
    process_in(time_boot,videomem);
    //kprintf("address of timer_isr %x\n",&timer_isr);
    //kprintf("time since boot %d s \n",time_boot++);
}
