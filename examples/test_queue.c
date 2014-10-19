
#include <stdlib.h>
#include <stdio.h>
#include "s_queue.h"

void my_print(void *user_data)
{
	printf(" the data is : %p \n \n",user_data);
}

int main()
{
	int i;
	struct s_queue *queue_d;
	void *ret;

	queue_d=s_queue_new();

	for(i=0;i<10;i++)
		s_queue_push(queue_d,(void *)i+5);

	s_queue_push_first(queue_d,(void *)20);
	s_queue_push_first(queue_d,(void *)21);

	s_queue_push_at_position(queue_d,(void *)30,4);
	s_queue_push_at_position(queue_d,(void *)31,1);

	ret=s_queue_pull_at_position(queue_d,4);



	ret = s_queue_pull(queue_d);

	printf("we pull : %p \n \n",ret);

	printf("the length of the queue is : %d \n \n",s_queue_get_length(queue_d));

	ret = s_queue_pull(queue_d);

	printf("we pull : %p \n \n",ret);

	printf("the length of the queue is : %d \n \n",s_queue_get_length(queue_d));

	ret = s_queue_pull(queue_d);

	printf("we pull : %p \n \n",ret);

	printf("the length of the queue is : %d \n \n",s_queue_get_length(queue_d));


	s_queue_show(queue_d, my_print);

}
