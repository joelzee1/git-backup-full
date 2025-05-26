#include "list.h"




List create_empty_list(void)
{
    return NULL;
}


Node * create_list_node(const Data data)
{
	List new;
	new=(Node*)malloc(sizeof(Node));
	if(new!=NULL)
	{
		new->data=data;
		new->next=NULL;
		new->previous=NULL;
		return new;
	}
	else
	{
		assert(0);
	}

}


int is_empty(const List list)
{
	if(list->data==NULL)
	{
		return 1;
	}
	else
	return 0;
}


void add_first(List *list, const Data data)
{
	List new=create_list_node(data);
	int n=is_empty((*list)->next);
	if(n==1)
	{
		(*list)->next=new;
		(*list)->previous=new;
	
		new->next=*list;
		new->previous=*list;
	}
	else
	{
		new->next=(*list)->next; 
		new->previous=*list;
		(*list)->next=new;
		new->next->previous=new;
	}

}


void add_last(List *list, const Data data)
{
	List new;
	new=create_list_node(data);
	if(is_empty((*list)->previous)==1)
	{
		(*list)->previous=new;
		(*list)->next=new;
		new->next=*list;
		new->previous=*list;
	}
	else
	{
		new->previous=(*list)->previous;
		new->next=*list;
		(*list)->previous=new;
		new->previous->next=new;
	}
	
}


void remove_first(List *list)
{
	List remove= (*list)->next;
	if(is_empty((*list)->next)==0)
	{
		(*list)->next=(*list)->next->next;
		(*list)->next->previous=*list;
		free(remove);
	}
}


void remove_last(List *list)
{
	
	List remove= (*list)->previous;
	if(is_empty((*list)->previous)==0)
	{
		(*list)->previous=(*list)->previous->previous;
		(*list)->previous->next=*list;
		free(remove);
	}
	
}


void clear_list(List *list)
{
	if(is_empty((*list)->next)==0)
	{
		remove_first(list);
		clear_list(list);
	}
	else{}
}


void print_list(const List list)
{
	char string[10];
	char free_tree_char[]="\n\r";
	if(is_empty(list->next)!=1)
	{
		sprintf(string,"%d",list->next->data);
		uarte_write(string,strlen(string));
		uarte_write(free_tree_char,strlen(free_tree_char));
		print_list(list->next);
	}
	else{}
}


Data get_first_element(const List list)
{
	return list->next->data;
}


Data get_last_element(const List list)
{
	return list->previous->data;
}


int number_of_nodes(const List list)
{
	int count=0;
	
if(is_empty(list->next)==0)
{
count=1+number_of_nodes(list->next);
return count;
}
else
{
	return 0;
}

}


int search(const List list, const Data data)
{
	int search_yesno;
	if(is_empty(list->next)==0)
	{
		if(data==list->next->data)
		{
			return 1;
		}
		search_yesno=search(list->next,data);
		return search_yesno;
	}
	else
	{
		return 0;
	}
}


int remove_element(List *list, const Data data)
{
	int element_yesno;
	List remove=(*list)->next;
	
	if(is_empty((*list)->next)==0)
	{
		if(data==(*list)->next->data)
		{	
				(*list)->next=(*list)->next->next;
				(*list)->next->next->previous=*list;
			
			free(remove);
			return 1;
		}
		else
		{
			element_yesno=remove_element(&(*list)->next,data);
			return element_yesno;
		}
	}
	else
	{
		return 0;
	}

}

int check_large_small(List list)
{
    if(is_empty(list->next)==0)
    {
        if(list->data < list->next->data)
        {
            return 0;
        }
        else
        {
            return check_large_small(list->next);
        }
    }
    return 1;
}

int check_sorted_list(List list)
{
    int check;
    if(is_empty(list->next)==0)
    {
        check=check_large_small(list->next);
        if(check==0)
        {
            return 0;
        }
        else
        {
            return check_sorted_list(list->next);
        }
    }
    return 1;
}

void switch_node(List list)
{
    List temp;
    if(list->data!=NULL)
    {
        if(list->next->data!=NULL)
        {
            temp=list->next;
            list->next=temp->next;
            temp->previous=list->previous;
            list->previous=temp;
            temp->next=list;
            temp->previous->next=temp;
            list->next->previous=list;
        }
    }
}

void bubble_sort(List list,List head)
{
    if(check_sorted_list(head)==1)
    {
        return;
    }
    if(list->data < list->next->data)
    {
        switch_node(list);
    }
    bubble_sort(list->next,head);
}
/*
void create_rand_list(List list, int num,int millisek)
{
    int count,number;
    srand(millisek);
       for(count=1;count!=num+1;count++)
        {
            number=rand()%99+1;
            add_last(&list,number);
        }
}
*/