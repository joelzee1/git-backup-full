/*
function generator(addition subtraction maybe division)
increasing dificultys
timer 
lose condition
score
*/
#include "game_first.h"


int main(void)
{
    int facit,timer,level,game_on,start,time,next,restart;
    nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(20, 22);
    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, uarte_handler);
    NVIC_ClearPendingIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    NVIC_EnableIRQ(NRFX_IRQ_NUMBER_GET(NRF_UARTE_INST_GET(0)));
    char enter[]="\n\r";
    char intro[]="welcome to THE Game";
    char win[]="congrats, You WIN";
    char next_level[]="next level!";
    char continue_[]="enter 1 to continue";
    char start_[]="enter 1 to start the Game";
    
    char restart_[]="Press 0 to restart";
    char clear_screen[]="\033c";
    init_stuff();
    nrfx_uarte_rx (&instance, &uarte_buffer, sizeof(uarte_buffer));
    while(1)
    {
        restart=1;
        level=1;
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
            if(state==1)
            {
                if(answer==1)
                {
                start=0;
                state=0;
                }
            }
        }
        
        while(game_on==1)
        {
            
            if(level>0)
            {
                next=1;
                uarte_write(clear_screen,sizeof(clear_screen));
                uarte_write(continue_,sizeof(continue_));
                uarte_write(enter,sizeof(enter));
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
            }
            switch(level)
            {
                case 0:
                uarte_write(enter,sizeof(enter));
                uarte_write(restart_,sizeof(restart_));
                uarte_write(enter,sizeof(enter));
                while(restart==1)
                {
                     if(answer==0)
                    {
                        restart=0;
                        game_on=0;
                    }
                    state=0;
                }

                
                break;
                case 1:
                        facit=function_addition(20,time);
                        uarte_write(enter,sizeof(enter));
                        level=countdown(30,level,facit);

                break;
                case 2:
                        facit=function_subtraction(20,time);
                        uarte_write(enter,sizeof(enter));
                        level=countdown(10,level,facit);
                break;
                
                case 3: 
                        facit=function_multiplication(10,time);
                        uarte_write(enter,sizeof(enter));
                        level=countdown(10,level,facit);
                break;
                case 4:
                        facit=function_addition(20,time);
                        int choice=1;
                        charthers(choice);
                        
                        facit=facit+function_subtraction(20,time+1000);
                        uarte_write(enter,sizeof(enter));
                        level=countdown(30,level,facit);
                break;
                case 5:
                        uarte_write(win,sizeof(win));
                        level=0;
                break;
            }
        }
    }
}