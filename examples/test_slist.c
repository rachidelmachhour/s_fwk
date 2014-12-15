
#include <stdlib.h>
#include <stdio.h>
#include "s_list.h"

void my_print(void *user_data)
{
	printf(" the data is : %p \n \n",user_data);
}

int main()
{
int i;
void *data;
struct s_list *list_d;
list_d=s_list_new();

for(i=0;i<5;i++)
{
	s_list_insert_last(list_d,(void *)i+5);
}


s_list_insert_first(list_d,(void *)18);
s_list_insert_first(list_d,(void *)19);
s_list_insert_first(list_d,(void *)20);

printf("the list is : %d \n",s_list_is_empty(list_d));
s_list_clean(list_d);
printf("the list is : %d \n",s_list_is_empty(list_d));

printf("position is : %d \n \n",s_list_get_position(list_d,(void *)5));
printf("la longeur est : %d \n \n",s_list_get_length(list_d));

i=s_list_delete_last(list_d);
if(i==-1)
{
	printf("list already empty !! \n \n");
}

s_list_delete_item(list_d,(void *)3);

s_list_insert_at_position(list_d,1,(void *)10);

data=s_list_get_data_at_position(list_d,7);

printf("this data is : %d \n \n \n \n",(int)data);

s_list_delete_at_position(list_d,6);

printf("la longeur est : %d \n \n",s_list_get_length(list_d));
s_list_print(list_d,my_print);


s_list_destroy(list_d);
}
