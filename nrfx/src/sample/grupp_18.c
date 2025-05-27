#include "grupp_18.h"


uint8_t uarte_buffer;
nrfx_uarte_t instance = NRFX_UARTE_INSTANCE(0);
nrfx_rtc_t rtc_instance = NRFX_RTC_INSTANCE(0);

void uarte_write(char* data, int length)
{
    nrfx_uarte_tx(&instance, data, length, 0);
    while(nrfx_uarte_tx_in_progress(&instance)){};
}

void string_read(char string[])
{
    int a=1,n=0;
    char input_string[100];
    while(a!=0)
    {
        nrfx_uarte_rx (&instance, &uarte_buffer, sizeof(uarte_buffer));
        if(uarte_buffer=='\r')
         {
             input_string[n]='\0';
               a=0;
         }
        else
         {
             input_string[n]=uarte_buffer;
                n=n+1;
         }   
    }
   strcpy(string,input_string);
}

void send_int(int converted_string,char string[])
{
    sprintf(string,"%d",converted_string);
}

int read_int(char input_string[])
{
int converted_string;
converted_string=atoi(input_string);
return converted_string;
}
/*
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
        }
        else
        {
            temp[now]=uarte_buffer;
            now=now+1;
        }
        
    }
}
*/
void init_stuff(void)
{
    /*
    nrfx_uarte_config_t config = NRFX_UARTE_DEFAULT_CONFIG(20, 22);
    nrfx_err_t errr = nrfx_uarte_init(&instance, &config, uarte_handler);
    */
    nrfx_systick_init();
    nrfx_rtc_config_t config2 = NRFX_RTC_DEFAULT_CONFIG;
    nrfx_err_t errr_2 = nrfx_rtc_init(&rtc_instance, &config2,NULL);
    nrfx_rtc_enable(&rtc_instance);
}
/*
*/