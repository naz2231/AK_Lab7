#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <hello1.h>
#include <linux/slab.h>
MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("AK_Lab7");
MODULE_AUTHOR("Nazar Martyniuk IO-82");

EXPORT_SYMBOL(print_hello);

static struct my_list_head *head;

void clear_my_list(void)
{
    struct my_list_head* temp_first;
    struct my_list_head* temp_second;

    temp_first = head;

    while(temp_first != NULL) {
        temp_second = temp_first->next;
        kfree(temp_first);
        temp_first = temp_second;
    }
}

static int print_hello(uint count)
{
    int i;
    struct my_list_head *temp_head1;
    struct my_list_head *temp_head2;

    head = kmalloc(sizeof(struct my_list_head*), GFP_KERNEL);

    temp_head1 = head;
    
    BUG_ON(count > 10);
    
    if (count == 0)
    {
        pr_warn("c = 0");
    }
    else if (count >= 5 && count <= 10)
    {
        pr_warn("5 < c < 10");
    }
    for (i = 0; i < count; i++)
    {
        temp_head1->next = kmalloc(sizeof(struct my_list_head*), GFP_KERNEL);
        if (i == 11) 
		temp_head1 = NULL;
	if (ZERO_OR_NULL_PTR(temp_head1))
		goto clear;
        temp_head1->time = ktime_get();
        pr_info("Hello, world\n");
        temp_head1->post_time = ktime_get();
        temp_head2 = temp_head1;
        temp_head1 = temp_head1->next;
    }
    kfree(temp_head2->next);
    temp_head2->next = NULL;
    return 0;
    
    clear:
	pr_err("No memory left\n");
	clear_my_list();
	return -ENOMEM;
}

static int __init hello1_init(void)
{
    pr_info("Hello1 started");
    pr_info("");
    return 0;
}

static void __exit hello1_exit(void)
{
    struct my_list_head* temp_first = head;
    struct my_list_head* temp_second = temp_first;
    pr_debug("------------Start------------");
    while(temp_first != NULL)
    {
        pr_debug("Working time: %lld", temp_first->post_time - temp_first->time);
        temp_second = temp_first;
        temp_first=temp_second->next;
        kfree(temp_second);
    }
    pr_debug("------------End------------");
    pr_info("Hello1 finished");
    pr_info("");
}

module_init(hello1_init);
module_exit(hello1_exit);
