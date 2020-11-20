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

static TextField ip;
static TextFieldArea msg;
static TextFieldArea recvs;

class MP:public MessageProcesser{
	public:
		Cfg *common_cfg;
		MP():MessageProcesser(){
		}
		virtual void OnInit(){
			ip.setText(common_cfg->get("lang.zh_cn.textfield.ip"));
			msg.setText(common_cfg->get("lang.zh_cn.textarea.msg"));
			recvs.setText(common_cfg->get("lang.zh_cn.textarea.recvs"));
		}
		virtual void OnCommand(){return;}
		virtual void OnDestory(){
			running=false;
		}
		virtual void OnPaint(){return;}
		virtual string to_string(){
			return "MP("+std::to_string(int(this))+")";
		}
};
static MP mp;
static Window *win;
static Label ipl("");
static Label msgl("");
static Button bsend("",onSend);
static Button brestart("",[]()->void {
    running=false;
    Socket so("127.0.0.1",31126);
    so.send("閫€鍑?);
    so.close();
    delete win;
    win=new Window("TalkTalk0.0.1 - for C++",&mp);
    void cleanup();
    cleanup();
    do_restartable_init();
});

static void onSend() {
    logger.info("杩炴帴鍦板潃:"+ip.getText());
    logger.info("鍙戦€佹秷鎭?"+msg.getText());
    Socket out(ip.getText().c_str(),31126);
    if(*(out.getState())==error) {
        logger.error("鍙戦€佸け璐?");
        return;
    }
    out.send(msg.getText());
    out.close();
}

static int listener_thread(LPVOID lpParamter) {
    ServerSocket server(31126);
    Logger inner("[LISTENER_THREAD]");
    inner.info("寮€濮嬬洃鍚?..");
    while(running) {
        Socket so=server.accept();
        inner.info("寰楀埌杩炴帴");
        string str=so.recv();
        inner.info("鏁版嵁:"+str);
        recvs.setText(recvs.getText()+"\n"+to_string(
                          so.getAddr()->sin_port)
                      +":"+str);
        win->update();
    }
    inner.info("閫€鍑?\n");
    server.close();
    return 0;
}

void do_only_once_init(){
    //cout<<HttpRequest("http://changxiang.nat123.net",80,"/talktalk.html?from=a&to=b&msg=some%20text");
    logger.info("鍒濆鍖栫洃鍚嚎绋?);
    Thread(listener_thread).start();
}

void do_restartable_init(){
	Log::init("log.log");
    Log::setLevel(INFO);
    logger.info("鍒濆鍖栫綉缁?);
    use_net();
	logger.info("璇诲彇璁剧疆鏂囦欢:common.cfg");
    Cfg common_cfg("common.cfg",default_common_cfg);
    
    logger.info("璁剧疆UI閮ㄤ欢灏哄");
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
	
	logger.info("鍔犺浇璇█");
	ipl.setText(common_cfg.get("lang.zh_cn.label.ip"));
	msgl.setText(common_cfg.get("lang.zh_cn.label.msg"));
	bsend.setText(common_cfg.get("lang.zh_cn.button.send"));
	brestart.setText(common_cfg.get("lang.zh_cn.button.restart"));
	
	logger.info("娉ㄥ唽UI閮ㄤ欢");
	mp.common_cfg=&common_cfg;
	win=new Window("TalkTalk0.0.1 - for C++",&mp);
    win->addLabel(&ipl);
    win->addLabel(&msgl);
    win->addButton(&bsend);
    win->addButton(&brestart);
    win->addTextField(&ip);
    win->addTextFieldArea(&msg);
    win->addTextFieldArea(&recvs);
}

void cleanup(){
	unuse_net();
    logger.info("閫€鍑?\n");
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
	cout<<"1"<<endl; 
	try{
		appMain();
	}catch(exception &e){
		logger.fatal("鍙戠幇鏈崟鎹夌殑寮傚父!");
		logger.fatal(string("璇︾粏淇℃伅:")+e.what());
	}catch(...){
		logger.fatal("鍙戠幇鏈煡寮傚父!");
	}
    return 0;
}
