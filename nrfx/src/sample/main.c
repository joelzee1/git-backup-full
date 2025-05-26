/*
function generator(addition subtraction maybe division)
increasing dificultys
timer 
lose condition
score

*/
#include "grupp_18.h"

nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
nrfx_rtc_t rtc_instance = NRFX_RTC_INSTANCE(0);

uint8_t uarte_buffer;
int answer=1,state=0,now=0;
char temp[100];

int read_int(char input_string[])
{
int converted_string;
converted_string=atoi(input_string);
return converted_string;
}

void uarte_handler(nrfx_uarte_event_t const *p_event, void *p_context)
{
    nrfx_uarte_t * p_inst = p_context;
    if (p_event->type == NRFX_UARTE_EVT_RX_DONE)
    {
        state=1;
        nrfx_uarte_rx (&instance, &uarte_buffer, sizeof(uarte_buffer));
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
            now=now+1;
        }
        
    }
}

void init_stuff(void)
{
    NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(20, 22);
    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, uarte_handler);
    nrfx_systick_init();
    nrfx_rtc_config_t config2 = NRFX_RTC_DEFAULT_CONFIG;
    nrfx_err_t errr_2 = nrfx_rtc_init(&rtc_instance, &config2,NULL);
    nrfx_rtc_enable(&rtc_instance);
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
    int timer,end;
    for(timer=seconds;end==0;timer--)
    {
        if(state==1)
        {
            if(answer==facit)
            {
                return level+1;
                state=0;
                end=0;
            }
            else
            {
                level=0;
                state=0; 
                return 0;
            }
        }
        if(timer==0)
        {
            return 0;
        }

        nrfx_systick_delay_ms(1000);
    } 
}



int main(void)
{
    int facit=1,timer,level,game_on,start,time,next;
    char enter[]="\n\r";
    char intro[]="welcome to THE Game";
    char win[]="You WIN";
    char next_level[]="next level!";
    char continue_[]="enter 1 to continue";
    char start_[]="enter 1 to start the Game";
    char lose_answer[]="Wrong Answer, Game Over";
    char lose_time[]="Your Time has run out, Game Over";
    char restart[]="Press 0 to restart";
    char clear_screen[]="\033c";
    init_stuff();

    while(1)
    {
       
        start=1;
        game_on=1;
        next=1;
        uarte_write(clear_screen,sizeof(clear_screen));
        uarte_write(intro,sizeof(intro));
        uarte_write(enter,sizeof(enter));
        uarte_write(start_,sizeof(start_));
        uarte_write(enter,sizeof(enter));
        while(start==1)
        {
           time=nrfx_rtc_counter_get(&rtc_instance)/32.768;
            
            if(state==1)
            {
                if(answer==1)
                {
                    start=0;
                }
                state=0;
            }
        }
        while(game_on==1)
        {
            uarte_write(clear_screen,sizeof(clear_screen));
            uarte_write(continue_,sizeof(continue_));
            while(next==1)
            {
                time=nrfx_rtc_counter_get(&rtc_instance)/32.768;
            
                if(state==1)
                {
                    if(answer==1)
                    {
                        uarte_write(clear_screen,sizeof(clear_screen));
                        next=0;
                    }
                    state=0;
                }
            }
        
            switch(level)
            {
                case 0:

                break;
                case 1:

                break;
                case 2:

                break;
                
                case 3: 

                break;
            }
        }
    }
}