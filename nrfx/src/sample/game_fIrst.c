#include "game_first.h"

int answer;
int state=0;
int now=0;
char temp[100];

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    nrfx_uarte_t * p_inst = p_context;
    
        if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
        {
            if(uarte_buffer=='\r')
            {
                temp[now]='\0';
                answer=read_int(temp);
                state=1;
                now=0;
            }
            else
            {
                temp[now]=uarte_buffer;
                now++;
            }
            nrfx_uarte_rx (&instance, &uarte_buffer, sizeof(uarte_buffer));
        }
        
  
}


void charthers(int choice)
{
    switch(choice)
    {
        case 1:
        char addition[]="+";
        uarte_write(addition,sizeof(addition));
        break; 
        case 2:
        char subtraction[]="-";
        uarte_write(subtraction,sizeof(subtraction));
        break;
        case 3:
       char multiplication[]= "X";
        uarte_write(multiplication,sizeof(multiplication));
        break;  
    }
    
    
}
int function_addition(int diff,int time)
{
    srand(time);
    int var1,var2;
    char var1_char[3],var2_char[3];
    char addition[]="+";
    var1=rand()%diff+1;
    var2=rand()%diff+1;
    send_int(var1,var1_char);
    send_int(var2,var2_char);

    uarte_write(var1_char,sizeof(var1_char));
    uarte_write(addition,sizeof(addition));
    uarte_write(var2_char,sizeof(var2_char));

    return var1+var2;
}

int function_subtraction(int diff,int time)
{
    srand(time);
    int var1,var2;
    char var1_char[3],var2_char[3];
    char subtraction[]="-";
    var1=rand()%diff+1;
    var2=rand()%diff+1; 
    send_int(var1,var1_char);
    send_int(var2,var2_char);

    uarte_write(var1_char,sizeof(var1_char));
    uarte_write(subtraction,sizeof(subtraction));
    uarte_write(var2_char,sizeof(var2_char));

    return var1-var2;
}

int function_multiplication(int diff,int time)
{
    srand(time);
    int var1,var2;
    char var1_char[3],var2_char[3];
    char multiplication[]="X";
    var1=rand()%diff+1;
    var2=rand()%diff+1;
    send_int(var1,var1_char);
    send_int(var2,var2_char);

    uarte_write(var1_char,sizeof(var1_char));
    uarte_write(multiplication,sizeof(multiplication));
    uarte_write(var2_char,sizeof(var2_char));

    return var1*var2;
}

int countdown(int seconds,int level, int facit)
{
    int timer;
    char lose_time[]="Your Time has run out, Game Over";
    char lose_answer[]="Wrong Answer, Game Over";
    for(timer=seconds;timer>-1;timer--)
    {
        if(state==1)
        {
            state=0;
            if(answer==facit)
            {
                return level+1;
            }
            else
            {
                uarte_write(lose_answer,sizeof(lose_answer));
                return 0;
            }
        }
        if(timer==0)
        {
            uarte_write(lose_time,sizeof(lose_time));
            return 0;
        }

        nrfx_systick_delay_ms(1000);
    } 
}
