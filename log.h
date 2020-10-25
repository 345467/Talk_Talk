#pragma once

enum Level {
    DEBUG=0,INFO,WARN,ERR,FATAL
};

#define CASE(V) if(in== #V ) return V
Level levelFromString(string in){
	CASE(DEBUG);
	CASE(INFO);
	CASE(WARN);
	CASE(ERR);
	CASE(FATAL);
	return INFO;
}
#undef CASE

int sti(string str){
	if(str=="null")return 0;
	return stoi(str);
}

namespace {
	CRITICAL_SECTION __lock;
    ofstream fout;
    Level level;
}

namespace Log {
    void init(string fname) {
    	InitializeCriticalSection(&__lock);
        fout.open(fname);
        if(!fout.good()) exit(-1);
    }
    void setLevel(Level in) {
        level=in;
    }
    Level getLevel() {
        return level;
    }
    void exit() {
        fout.close();
    }
}

class Logger {
        string threadname;
        string make_msg(string type,string msg) {
            auto tmp=time(NULL);
            auto time_str=ctime(&tmp);
            time_str[strlen(time_str)-2]='\0';
            return type+threadname+time_str+"-"
                   +to_string(clock()%1000)+"\t | "+msg+"\n";
        }
        void thread_safe_print(string s){
        	EnterCriticalSection(&__lock);
        	fout<<s;
            cout<<s;
            LeaveCriticalSection(&__lock);
		}
    public:
        Logger(string in) {
            threadname=in;
        }
        void debug(string msg) {
            if(level>DEBUG) return;
            thread_safe_print(make_msg("[DEBUG]",msg));
        }
        void info(string msg) {
            if(level>INFO) return;
            thread_safe_print(make_msg("[INFO]",msg));
        }
        void warn(string msg) {
            if(level>WARN) return;
            thread_safe_print(make_msg("[WARN]",msg));
        }
        void error(string msg) {
            if(level>ERR) return;
            thread_safe_print(make_msg("[ERROR]",msg));
        }
        void fatal(string msg) {
            if(level>FATAL) return;
            thread_safe_print(make_msg("[FATAL]",msg));
        }
};
