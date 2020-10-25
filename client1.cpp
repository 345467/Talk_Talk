#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

static bool running=true;

#include "log.h"
Logger logger("[MAIN_THREAD]");
#include "advancedNetworkHelper.h"
using namespace Web;
#include "UIHelper.h"
#include "cfg.h"
#include "default_cfgs.h"
#include "threadHelper.h"

static void onSend();
void do_restartable_init();

static Label ipl("");
static Label msgl("");
static TextField ip;
static TextFieldArea msg;
static TextFieldArea recvs;
static Button bsend("",onSend);
static Button brestart("",[]()->void {
    running=false;
    Socket so("127.0.0.1",5010);
    so.send("�˳�");
    so.close();
    close();
    void cleanup();
    cleanup();
    do_restartable_init();
});

static void onSend() {
    logger.info("���ӵ�ַ:"+ip.getText());
    logger.info("������Ϣ:"+msg.getText());
    Socket out(ip.getText().c_str(),31126);
    if(*(out.getState())==error) {
        logger.error("����ʧ��!");
        return;
    }
    out.send(msg.getText());
    out.close();
}

static int listener_thread(LPVOID lpParamter) {
    ServerSocket server(31126);
    Logger inner("[LISTENER_THREAD]");
    inner.info("��ʼ����...");
    while(running) {
        Socket so=server.accept();
        inner.info("�õ�����");
        string str=so.recv();
        inner.info("����:"+str);
        recvs.setText(recvs.getText()+"\n"+to_string(
                          so.getAddr()->sin_port)
                      +":"+str);
        SendMessage(hwnd,UWM_NULL,0,0);
    }
    inner.info("�˳�!\n");
    server.close();
    return 0;
}

void do_only_once_init(){
	Log::init("log.log");
    Log::setLevel(INFO);
    logger.info("��ʼ������");
    use_net();

    //cout<<HttpRequest("http://changxiang.nat123.net",80,"/talktalk.html?from=a&to=b&msg=some%20text");
    logger.info("��ʼ�������߳�");
    Thread(listener_thread).start();
}

void do_restartable_init(){
	logger.info("��ȡ�����ļ�:common.cfg");
    Cfg common_cfg("common.cfg",default_common_cfg);
    
    logger.info("����UI�����ߴ�");
    Log::setLevel(levelFromString(common_cfg.get("log.level")));
    ipl.setBounds(sti(common_cfg.get("gui.label.ip.x")),
				 sti(common_cfg.get("gui.label.ip.y")));
	msgl.setBounds(sti(common_cfg.get("gui.label.msg.x")),
				  sti(common_cfg.get("gui.label.msg.y")));
	ip.setBounds(sti(common_cfg.get("gui.textfield.ip.x")),
				 sti(common_cfg.get("gui.textfield.ip.y")),
				 sti(common_cfg.get("gui.textfield.ip.w")),
				sti(common_cfg.get("gui.textfield.ip.h")));
	msg.setBounds(sti(common_cfg.get("gui.textarea.msg.x")),
				  sti(common_cfg.get("gui.textarea.msg.y")),
				  sti(common_cfg.get("gui.textarea.msg.w")),
				 sti(common_cfg.get("gui.textarea.msg.h")));
	recvs.setBounds(sti(common_cfg.get("gui.textarea.recvs.x")),
				    sti(common_cfg.get("gui.textarea.recvs.y")),
				    sti(common_cfg.get("gui.textarea.recvs.w")),
				   sti(common_cfg.get("gui.textarea.recvs.h")));
    bsend.setBounds(sti(common_cfg.get("gui.button.send.x")),
				    sti(common_cfg.get("gui.button.send.y")),
				    sti(common_cfg.get("gui.button.send.w")),
				   sti(common_cfg.get("gui.button.send.h")));
    brestart.setBounds(sti(common_cfg.get("gui.button.exit.x")),
				       sti(common_cfg.get("gui.button.exit.y")),
				       sti(common_cfg.get("gui.button.exit.w")),
				      sti(common_cfg.get("gui.button.exit.h")));
	
	logger.info(bsend.to_string());
	
	logger.info("��������");
	ipl.setText(common_cfg.get("lang.zh_cn.label.ip"));
	msgl.setText(common_cfg.get("lang.zh_cn.label.msg"));
	bsend.setText(common_cfg.get("lang.zh_cn.button.send"));
	brestart.setText(common_cfg.get("lang.zh_cn.button.restart"));
	
	logger.info("ע��UI����");
    addLabel(&ipl);
    addLabel(&msgl);
    addButton(&bsend);
    addButton(&brestart);
    addTextField(&ip);
    addTextFieldArea(&msg);
    addTextFieldArea(&recvs);
    window("TalkTalk0.0.1 - for C++",[&common_cfg]()->void {
        ip.setText(common_cfg.get("lang.zh_cn.textfield.ip"));
		msg.setText(common_cfg.get("lang.zh_cn.textarea.msg"));
		recvs.setText(common_cfg.get("lang.zh_cn.textarea.recvs"));
    });
}

void cleanup(){
	unuse_net();
    logger.info("�˳�!\n");
    Log::exit();
}

void appMain(){
	do_only_once_init();
	do_restartable_init();
	cleanup();
    system("pause");
}
//server.natappfree.cc
int main(int argc, char* argv[]) {
	try{
		appMain();
	}catch(exception &e){
		logger.fatal("����δ��׽���쳣!");
		logger.fatal(string("��ϸ��Ϣ:")+e.what());
	}catch(...){
		logger.fatal("����δ֪�쳣!");
	}
    return 0;
}
