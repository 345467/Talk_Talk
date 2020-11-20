#pragma once

#define UWM_NULL (WM_USER+1)

#if defined(interface)
 #undef interface
#endif
#if defined(impl)
 #undef impl
#endif

#define interface class
#define impl virtual public

interface Object{
	public:
		virtual string to_string() = 0;
		virtual int hash(){
			return (int)this;
		}
};

interface WindowPart:impl Object{
	protected:
		HWND hwnd;
		int x,y,w,h;
	public:
		virtual void init(HWND hwnd) = 0;
		virtual string to_string(){
			return "WindowPart("+std::to_string(hash())+
				 ")[with:hwnd="+std::to_string((int)hwnd)+","
						      "x="+std::to_string(y)+","
						      "y="+std::to_string(y)+","
						      "w="+std::to_string(w)+","
						     "h="+std::to_string(h)+"]";
		}
		virtual void setBounds(int ix,int iy,int iw,int ih) {
            x=ix;
            y=iy;
            w=iw;
            h=ih;
        }
};

interface MessageProcesser:impl Object{
	public:
		virtual void OnInit() = 0;
		virtual void OnCommand() = 0;
		virtual void OnDestory() = 0;
		virtual void OnPaint() = 0;
}

template<MessageProcesser *mp>
interface Window:impl WindowPart{
	string title;
	vector<Button *> buttons;
	vector<Label *> labels;
	vector<TextField *> textFields;
 	vector<TextFieldArea *> textFieldAreas;
	
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
                         WPARAM wParam, LPARAM lParam) {
    	HDC hdc=GetDC(hWnd);
    	for(Label *l:labels)
        	l->draw(hdc);
    	ReleaseDC(hWnd,hdc);
    	switch(message) {
        	case UWM_NULL:
            	break;
        	case WM_COMMAND:
				mp->OnCommand();
            	for(auto &it:buttons)
                if(it->code==LOWORD(wParam))
					it->proc();
            break;
			case WM_PAINT:
				mp->OnPaint();
        	case WM_DESTROY: {
				mp->OnDestory();
            	running=false;
            	PostQuitMessage(0);
            	break;
        	}
        	default:
           		return DefWindowProc(hWnd, message, wParam, lParam);
            	break;
    	}
    	return 0;
	}
	void initWindow(string ititle) {
    	title=ititle;
    	HINSTANCE hInstance=GetModuleHandle(NULL);
    	WNDCLASSEX wcex;
    	wcex.cbSize = sizeof(WNDCLASSEX);
    	wcex.style          = CS_HREDRAW | CS_VREDRAW;
    	wcex.lpfnWndProc    = WndProc;
    	wcex.cbClsExtra     = 0;
    	wcex.cbWndExtra     = 0;
    	wcex.hInstance      = hInstance;
    	wcex.hIcon          = LoadIcon(hInstance, NULL);
    	wcex.hCursor        = LoadCursor(NULL, NULL);
    	wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    	wcex.lpszMenuName   = NULL;
    	wcex.lpszClassName  = title.c_str();
    	wcex.hIconSm        = LoadIcon(hInstance, NULL);
    	if(!RegisterClassEx(&wcex)) logger.fatal("窗口类注册失败!");
    	hwnd = CreateWindow(title.c_str(),title.c_str(),
                        	WS_OVERLAPPEDWINDOW,0,0,600,400,NULL,NULL,
                        	hInstance,NULL);
    
    	for(Button *b:buttons) b->init(hwnd);
    	for(TextField *b:textFields) b->init(hwnd);
    	for(TextFieldArea *b:textFieldAreas) b->init(hwnd);
	}

	int windowLoop() {
    	ShowWindow(hwnd, SW_SHOWNORMAL);
    	UpdateWindow(hwnd);
    	MSG msg;
    	while (GetMessage(&msg, NULL, 0, 0)) {
        	TranslateMessage(&msg);
        	DispatchMessage(&msg);
    	}
    	return (int)msg.wParam;
	}
public:
	void addButton(Button *b) {
    	buttons.push_back(b);
	}
	void addLabel(Label *b) {
    	labels.push_back(b);
	}
	void addTextField(TextField *b) {
    	textFields.push_back(b);
	}
	void addTextFieldArea(TextFieldArea *b) {
    	textFieldAreas.push_back(b);
	}
	void close(){
    	SendMessage(hwnd,WM_DESTROY,NULL,NULL);
		HINSTANCE hInstance=GetModuleHandle(NULL);
    	UnregisterClass(title.c_str(),hInstance);
	}
	Window(string windowTitle) {
    	initWindow(windowTitle);
    	mp->OnInit();
    	windowLoop();
    	close();
	}
}

interface HaveText:impl Object{
	protected:
		string text="";
		HaveText(string i):text(i){
		}
	public:
		virtual string to_string(){
			return "HaveText("+std::to_string(hash())+
					")[with:text="+text+"]";
		}
		void setText(string strin) {
            text=strin;
        }
};

class Button:impl WindowPart,impl HaveText{
    public:
        typedef void (*PROC) ();
        PROC proc;
        int code;
	private:
        friend void initWindow(string);
        friend LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,
                                        LPARAM);
        virtual void init(HWND hwnd) {
        	static int icode=WM_USER+1;
            if(!CreateWindow("Button", text.c_str(),
                             WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,
                             x, y, w, h, hwnd, (HMENU)++icode,
                             GetModuleHandle(NULL), NULL))
							logger.error("按钮"+text+"创建失败!");
            code=icode;
        }
    public:
        Button(string itext,PROC iproc):HaveText(itext),
            proc(iproc) {}
        virtual string to_string(){
        	return "Button("+std::to_string(hash())+")[with:"+
        			 "code="+std::to_string(code)+
        			",proc="+std::to_string((int)proc)+
        			",WindowPart="+WindowPart::to_string()+
        			",HaveText="+HaveText::to_string()+"]";
		}
};

class Label : impl WindowPart,impl HaveText{
	virtual void init(HWND ihwnd){
		hwnd=CreateWindow(
                "static",text.c_str(),
        		WS_CHILD|WS_VISIBLE|SS_CENTER|SS_CENTERIMAGE,
                w,y,w,h,ihwnd,NULL,NULL,NULL);
        if(!hwnd) logger.error("标签创建失败!");
	}
	public:
        Label(string itext):HaveText(itext) {}
        void draw(HDC hdc) {
            TextOut(hdc,x,y,text.c_str(),text.size());
        }
        void setBounds(int ix,int iy){
        	WindowPart::setBounds(ix,iy,-1,-1);
		}
        virtual string to_string(){
        	return "Label("+std::to_string(hash())+")[with:"+
        			"WindowPart="+WindowPart::to_string()+
        			",HaveText="+HaveText::to_string()+"]";
		}
};

class TextField : impl WindowPart{
        friend void initWindow(string);
        friend LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,
                                        LPARAM);
        virtual void init(HWND ihwnd) {
            if(!(hwnd=CreateWindow("edit",NULL,
                                  WS_CHILD|WS_VISIBLE|WS_BORDER,
                                  x,y,w,h,ihwnd,NULL,NULL,NULL)))
                logger.error("文本框创建失败!");
        }
    public:
        string getText() {
            char buffer[5000];
            GetWindowText(hwnd,buffer,4995);
            return string(buffer);
        }
        TextField(int ix=0,int iy=0,int iw=0,int ih=0) {
            setBounds(ix,iy,iw,ih);
        }
        void setText(string str) {
            SetWindowText(this->hwnd,str.c_str());
        }
        virtual string to_string(){
        	return "TextField("+std::to_string(hash())+
					")[with:text="+getText()+
        			",WindowPart="+WindowPart::to_string()+"]";
		}
};

class TextFieldArea : impl WindowPart{
    private:
        friend void initWindow(string);
        friend LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,
                                        LPARAM);
        virtual void init(HWND ihwnd) {
            if(!(hwnd=CreateWindow("edit",NULL,
                                  WS_CHILD|WS_VISIBLE|WS_BORDER|
                                  WS_HSCROLL|WS_VSCROLL|ES_MULTILINE,
                                  x,y,w,h,ihwnd,NULL,NULL,NULL)))
                logger.error("文本域创建失败!");
        }
    public:
        void setText(string str) {
            SetWindowText(this->hwnd,str.c_str());
        }
        string getText() {
            char buffer[5000];
            GetWindowText(this->hwnd,buffer,4995);
            return string(buffer);
        }
        TextFieldArea(int ix=0,int iy=0,int iw=0,int ih=0) {
            setBounds(ix,iy,iw,ih);
        }
        virtual string to_string(){
        	return "TextArea("+std::to_string(hash())+
					")[with:text=\""+getText()+
        			"\",WindowPart="+WindowPart::to_string()+"]";
		}
};

static string title;
HWND hwnd;
static vector<Button*> buttons;
static vector<Label*> labels;
static vector<TextField *> textFields;
static vector<TextFieldArea *> textFieldAreas;

void addButton(Button *b) {
    buttons.push_back(b);
}

void addLabel(Label *b) {
    labels.push_back(b);
}

void addTextField(TextField *b) {
    textFields.push_back(b);
}

void addTextFieldArea(TextFieldArea *b) {
    textFieldAreas.push_back(b);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
                         WPARAM wParam, LPARAM lParam) {
    HDC hdc=GetDC(hWnd);
    for(Label *l:labels)
        l->draw(hdc);
    ReleaseDC(hWnd,hdc);
    switch(message) {
        case UWM_NULL:
            break;
        case WM_COMMAND:
            for(auto &it:buttons)
                if(it->code==LOWORD(wParam))
					it->proc();
            break;
        case WM_DESTROY: {
            running=false;
            PostQuitMessage(0);
            break;
        }
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
            break;
    }
    return 0;
}

void initWindow(string ititle) {
    title=ititle;
    HINSTANCE hInstance=GetModuleHandle(NULL);
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, NULL);
    wcex.hCursor        = LoadCursor(NULL, NULL);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;
    wcex.lpszClassName  = title.c_str();
    wcex.hIconSm        = LoadIcon(hInstance, NULL);
    if(!RegisterClassEx(&wcex)) logger.fatal("窗口类注册失败!");
    hwnd = CreateWindow(title.c_str(),title.c_str(),
                        WS_OVERLAPPEDWINDOW,0,0,600,400,NULL,NULL,
                        hInstance,NULL);
    
    for(Button *b:buttons) b->init(hwnd);
    for(TextField *b:textFields) b->init(hwnd);
    for(TextFieldArea *b:textFieldAreas) b->init(hwnd);
}

static int windowLoop() {
    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

static void destoryWindow() {
    HINSTANCE hInstance=GetModuleHandle(NULL);
    UnregisterClass(title.c_str(),hInstance);
}

void close(){
    SendMessage(hwnd,WM_DESTROY,NULL,NULL);
	destoryWindow();
}

template<typename _Ft>
void window(string windowTitle,_Ft afterInit) {
    initWindow(windowTitle);
    afterInit();
    windowLoop();
    destoryWindow();
}

