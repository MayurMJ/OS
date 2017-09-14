static time_boot = 0;
void timer_irqhandler(void)
{
    kprint("time since boot %d s \n",time_boot++);
}
