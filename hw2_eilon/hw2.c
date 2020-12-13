#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/list.h>

int rec_solve(struct task_struct* pd);

asmlinkage long sys_hello(void) {
	printk("Hello, World!\n");
	return 0;
}

asmlinkage long sys_set_weight(int weight){

	if (weight < 0){
		return -EINVAL;
	}
	current->weight = weight;
	return 0;

}

asmlinkage long sys_get_total_weight(void){
 
 	return rec_solve(current);
}

asmlinkage long sys_get_heaviest_child(void){
	if(&(current->children) == current->children.next){
		return -ECHILD;
	}
	int max;
	max=0;
	pid_t pid_of_max=0;
 	struct task_struct *son;
	struct list_head *list;
	list_for_each(list, &(current->children)) {
		son = list_entry(list, struct task_struct, sibling);
		int child_total_w = rec_solve(son);
	  	if (child_total_w > max){
			max = child_total_w;
			pid_of_max = son->pid;
	 	}
	}
	return pid_of_max;
}
 
 
int rec_solve(struct task_struct* pd){
	
	int sum;
	sum=pd->weight;
 
 	if(&(current->children) == current->children.next){
 		return sum;
 	}
 	//#get weight of all my children recursivly and sum them
 
 	struct task_struct* son;
	struct list_head *list;
	list_for_each(list, &(pd->children)) {
		son = list_entry(list, struct task_struct, sibling);
 		sum += rec_solve(son);
 	}
 	return sum;
}
