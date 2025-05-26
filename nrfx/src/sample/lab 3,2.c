#include "list.h"
#define BUTTON1 23

int check_sorted_array(int array[],int num)
{

    int a,b;
    for(a=0;a!=num;a++)
    {
        for(b=a;b!=num;b++)
        {
            if(array[a]>array[b])
            {
                return 0;
            }
        }
    }
    return 1;
}


int* create_rand_array(int num, int millisek)
{
    int count,number;
    int*array;
    array=(int*)malloc(sizeof(int)*num);
        srand(millisek);

     for(count=0;count!=num;count++)
     {
        number=rand()%99+1;
        array[count]=number;
    }
    return array;
}

void swap_array(int array[],int n1, int n2)
{
    int temp;
    temp=array[n1];
    array[n1]=array[n2];
    array[n2]=temp;
}


void selection_sort(int array[],int num)
{
    int small, swap;
    int number,small_n;

    for(number=0;number<num;number++)
    {
        small=array[number];
        swap=number;
        for(small_n=number;small_n<num;small_n++)
        {
            if(small > array[small_n])
            {
                small=array[small_n];
                swap=small_n;
            }
        }
        swap_array(array,number,swap);
    }
}




void merge(int array[],int left,int mid,int right)
{
    int i,j,k;
    int n1 = mid - left + 1;
    int n2 = right - mid;
    int left_array[n1],right_array[n2];

    for(i=0;i<n1;i++)
    {
        left_array[i]=array[left+i];
    }

    for(j=0;j<n2;j++)
    {
        right_array[j]=array[mid+1+j];
    }

    i=0;
    j=0;
    k=left;

    while(i<n1 && j<n2)
    {
        if(left_array[i]<= right_array[j])
        {
            array[k]=left_array[i];
            i++;
        }
        else
        {
            array[k]=right_array[j];
            j++;
        }
        k++;
    }

    while(i < n1)
    {
        array[k]=left_array[i];
        i++;
        k++;
    }

    while(j < n2)
    {
        array[k]=right_array[j];
        j++;
        k++;
    }
}

void merge_sort(int array[],int left,int right)
{
  if(left<right)
  {
    int mid=left+(right-left)/2;
    merge_sort(array,left,mid);
    merge_sort(array,mid+1,right);
    merge(array,left,mid,right);
  }
}
int main(void)
{
    
    init_stuff();
    char number[100];
    int num=10,i,time,millisek,count,r_num;
    int *array;
    char string[10];
    List head = create_list_node(NULL);
    head->next=head;
    head->previous=head;
    nrf_gpio_cfg_input(BUTTON1,NRF_GPIO_PIN_PULLUP);
	char free_tree_char[]="\n\r";
    while(1)
    {
        millisek=nrfx_rtc_counter_get(&rtc_instance)/32.768;
        if(nrf_gpio_pin_read(23)==0)
        {
        for(count=1;count!=num+1;count++)
        {
           // r_num=rand()%99+1;
            //add_last(&head,r_num);
            array=create_rand_array(num,millisek);
        }
        break;
        }
     }
    //print_list(head);
    uarte_write(free_tree_char,strlen(free_tree_char));
    for(i=0;i<num;i++)
    {
		sprintf(string,"%d",array[i]);
		uarte_write(string,strlen(string));
        uarte_write(free_tree_char,strlen(free_tree_char));
    }
    uint32_t pretime = nrf_systick_val_get();
    //bubble_sort(head,head);
    merge_sort(array,2,num);
    uint32_t posttime = nrf_systick_val_get();
    for(i=0;i<num;i++)
    {
		sprintf(string,"%d",array[i]);
		uarte_write(string,strlen(string));
        uarte_write(free_tree_char,strlen(free_tree_char));
    }
   // print_list(head);
    uarte_write(free_tree_char,strlen(free_tree_char));
    time=pretime-posttime;
    sprintf(number,"%d",time);
    uarte_write(number,strlen(number));
}

/*merge_sort(array,0,num);
    for(i=0;i<num;i++)
    {
		sprintf(string,"%d",array[i]);
		uarte_write(string,strlen(string));
        uarte_write(free_tree_char,strlen(free_tree_char));
    }
        array=create_rand_array(num,millisek);
    selection_sort(array,num);*/

