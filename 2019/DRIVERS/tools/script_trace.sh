#!/bin/ash

echo 0 > /sys/kernel/debug/tracing/tracing_on
echo "sched:sched_switch"  >  /sys/kernel/debug/tracing/set_event
echo "sched:sched_wakeup"  >> /sys/kernel/debug/tracing/set_event
echo "irq:irq_handler_entry"  >>  /sys/kernel/debug/tracing/set_event
echo "irq:irq_handler_exit"  >>  /sys/kernel/debug/tracing/set_event
echo 1 > /sys/kernel/debug/tracing/tracing_on

virt_dev_read_block /dev/virt_dev_0 1 &
virt_dev_write /dev/virt_dev_0 23

echo 0 > /sys/kernel/debug/tracing/tracing_on
