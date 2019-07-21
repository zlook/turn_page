/*
根据maibu社区提供代码修改，控制ppt翻页
date:2019-07-21
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "maibu_sdk.h"
#include "maibu_res.h"
#define SCROLL_KEY	200
uint32_t comid3=0;
uint32_t comid2=0;
uint32_t comid1=0;
static int8_t txtlayerid=0;
static int32_t  windowsid=-1;


void tx_back(enum ECommResult result, uint32_t comm_id, void *context)
{
}

void web_back(const uint8_t *buff, uint16_t size)
{
    int32_t zt=0;//从server返回的参数，用以在表盘显示操作结果，zt==1则打印“上翻”，zt==2，“下翻”
    maibu_get_json_int(buff, "msg", &zt);//这个接口是收到来自WEB的数据，buff   如果使用了filter_param，则返回值是过滤后的WEB返回值，可以分别使用
    P_Window p_window = app_window_stack_get_window_by_id(windowsid);

    if(NULL!=p_window)
    {
        P_Layer layer= app_window_get_layer_by_id(p_window,txtlayerid);

        if(NULL!=layer)
        {
           maibu_service_vibes_pulse(VibesPulseTypeShort,1);
           char sj[30] ="";
           if(zt==1){
                sprintf(sj,"%s","上翻");
                app_layer_set_text_text(layer,sj);
                }
           else if(zt==2){
                sprintf(sj,"%s","下翻");
                app_layer_set_text_text(layer,sj);
                }
           else{
                sprintf(sj,"%s","none");
                app_layer_set_text_text(layer,sj);
                }
        }

      app_window_update(p_window);
    }
       //maibu_service_vibes_pulse(VibesPulseTypeLong,1);
}

/*定义向上按键事件，传递forward*/
void scroll_select_up(void *context)
{
        char buff[30] ="";
        sprintf(buff,"%s","http://192.168.1.100:8089/page_up");
        comid1=maibu_comm_request_web(buff,"msg",0);
		//buff:访问的url
		//"msg"：filter_param 过滤参数，表示只接受过滤参数中key所对应的value值。不同的key使用英文逗号隔开，不需要过滤则设置为NULL。
		//0（sync_time） 同步间隔时间，如果为0，则表示只请求一次，sync_time实际最小有效值为5秒。

}
/*定义向下按键事件，传递Back*/
void scroll_select_down(void *context)
{
        char buff[30] ="";
        sprintf(buff,"%s","http://192.168.1.100:8089/page_down");
        comid2=maibu_comm_request_web(buff,"msg",0);
}

/*定义选择按键事件，传递Stop*/
void scroll_select_Select(void *context)
{
        char buff[30] ="";
        sprintf(buff,"%s","http://192.168.1.100:8089/extend");
        comid3=maibu_comm_request_web(buff,"msg",0);
}



/*定义后退按键事件*/
void scroll_select_back(void *context)
{
	P_Window p_window = (P_Window)context;
	app_window_stack_pop(p_window);
}


int main()
{
    // simulator_init();//模拟初始化

	/*创建窗口*/
	P_Window p_window = app_window_create();
	if (NULL == p_window)
	{
		return 0;
	}
        /*创建文本图层框架范围*/
	GRect frame = {{0, 45}, {40, 128}};
	unsigned char buf[]="PPT翻页";

	/*生成文本结构体, 依次为文本内容、文本显示框架、对齐方式、字体字号*/
	LayerText text = {buf, frame, GAlignCenter, U_GBK_SIMSUNBD_16};

	/*创建文本图层*/
	P_Layer layer = app_layer_create_text(&text);

	/*添加文本图层到窗口中*/

	txtlayerid=app_window_add_layer(p_window, layer);
        /*添加按键事件，实现上下翻页功能*/
	app_window_click_subscribe(p_window, ButtonIdDown, scroll_select_down);//按动向下键
	app_window_click_subscribe(p_window, ButtonIdUp, scroll_select_up);//按动向上键
	app_window_click_subscribe(p_window, ButtonIdBack, scroll_select_back);//按动back键
	app_window_click_subscribe(p_window, ButtonIdSelect, scroll_select_Select);//按动选择键

        maibu_comm_register_result_callback(tx_back);
        maibu_comm_register_web_callback(web_back);


	/*把窗口放入窗口栈中显示*/
	windowsid=app_window_stack_push(p_window);

  //simulator_wait();//模拟开始
	return 0;
}
