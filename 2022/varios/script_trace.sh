echo "0" > /sys/kernel/debug/tracing/tracing_on
echo "raw_syscalls:sys_exit" > /sys/kernel/debug/tracing/set_event
echo "raw_syscalls:sys_enter" >> /sys/kernel/debug/tracing/set_event
echo "irq:irq_handler_entry" >> /sys/kernel/debug/tracing/set_event
echo "irq:irq_handler_exit" >> /sys/kernel/debug/tracing/set_event
echo "sched:sched_switch" >> /sys/kernel/debug/tracing/set_event
echo "timer:timer_expire_entry" >> /sys/kernel/debug/tracing/set_event
echo "timer:timer_expire_exit" >> /sys/kernel/debug/tracing/set_event
echo "1" > /sys/kernel/debug/tracing/tracing_on
ls -l
echo "0" > /sys/kernel/debug/tracing/tracing_on
cat /sys/kernel/debug/tracing/trace > traza.out
