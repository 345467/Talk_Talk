#pragma once

#include<windows.h>
using namespace std;

class Thread{
    static DWORD WINAPI thread_proc(LPVOID pointer){
		Thread *t=(Thread *)pointer;
		return t->proc(t->arg);
    }
    typedef int (*proc_type)(LPVOID);
    proc_type proc;
    HANDLE thandle;
    LPVOID arg;
public:
    Thread(proc_type iproc){
        proc=iproc;
    }
    void start(LPVOID iarg=NULL){
    	arg=iarg;
        thandle=CreateThread(0,0,thread_proc,this,0,0);
    }
    void sleep(){

    }
};
