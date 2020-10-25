#pragma once

#include"networkHelper.h"

namespace Web {

#include <windows.h>
#include <wininet.h>
#pragma comment(lib,"Wininet.lib")
    /**
    std::string HttpRequest(char * lpHostName, short sPort=80,
                            char * lpUrl="/", char * lpMethod="GET",
                            char * lpPostData=NULL, int nPostDataLen=0){
        HINTERNET hInternet, hConnect, hRequest;

        BOOL bRet;

        std::string strResponse;

        hInternet = NULL;
        hConnect = NULL;
        hRequest = NULL;

        hInternet = (HINSTANCE)InternetOpen("User-Agent", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
        if (!hInternet)
            goto Ret0;

        hConnect = (HINSTANCE)InternetConnect(hInternet, lpHostName, sPort, NULL, "HTTP/1.1", INTERNET_SERVICE_HTTP, 0, 0);
        if (!hConnect)
            goto Ret0;

        hRequest = (HINSTANCE)HttpOpenRequest(hConnect, lpMethod, lpUrl, "HTTP/1.1", NULL, NULL, INTERNET_FLAG_RELOAD, 0);
        if (!hRequest)
            goto Ret0;

        bRet = HttpSendRequest(hRequest, NULL, 0, lpPostData, nPostDataLen);
        while (TRUE)
        {
            char cReadBuffer[4096];
            unsigned long lNumberOfBytesRead;
            bRet = InternetReadFile(hRequest, cReadBuffer, sizeof(cReadBuffer) - 1, &lNumberOfBytesRead);
            if (!bRet || !lNumberOfBytesRead)
                break;
            cReadBuffer[lNumberOfBytesRead] = 0;
            strResponse = strResponse + cReadBuffer;
        }

    Ret0:
        if (hRequest)
            InternetCloseHandle(hRequest);
        if (hConnect)
            InternetCloseHandle(hConnect);
        if (hInternet)
            InternetCloseHandle(hInternet);

        return strResponse;
    }
    /**/
    static size_t getFileSize(const std::string&
                              file_name) {
        std::ifstream in(file_name.c_str());
        in.seekg(0, std::ios::end);
        size_t size = in.tellg();
        in.close();
        printf("\n%d",size);
        return size;
    }


    static void string_replace( string &strBig,
                                const string &strsrc, const std::string &strdst) {
        string::size_type pos = 0;
        string::size_type srclen = strsrc.size();
        string::size_type dstlen = strdst.size();

        while( (pos=strBig.find(strsrc,
                                pos)) != string::npos ) {
            strBig.replace( pos, srclen, strdst );
            pos += dstlen;
        }
    }

    static string getName(string strFullName) {
        if (strFullName.empty()) {
            return "";
        }

        string_replace(strFullName, "/", "\\");

        string::size_type iPos =
            strFullName.find_last_of('\\') + 1;

        return strFullName.substr(iPos,
                                  strFullName.length() - iPos);
    }

    void sendFile(Socket &so,const char *filename,
                  int blocksize=1048576/*1MB*/) {
        ifstream fin(filename,ios_base::binary);
        if(!fin.good())puts("oops\n");
        int fsize=getFileSize(filename);
        int blocks=fsize/blocksize;
        int last=fsize%blocksize;
        printf(getName(string(filename)).c_str());
        printf("\n%d %d %d\n",fsize,blocks,last);
        so.send(/*string(strrchr(filename,'/')+1)*/
            getName(string(filename)));
        so.send((char *)&blocks,4);
        so.send((char *)&blocksize,4);
        so.send((char *)&last,4);
        char *data=new char[blocksize];
        while(blocks--) {
            fin.read(data,blocksize);
            so.send(data,blocksize);
        }
        delete[] data;
        if(last!=0) {
            data=new char[last];
            fin.read(data,last);
            so.send(data,last);
            delete[] data;
        }
        fin.close();
    }

    void recvFile(Socket &so,const char *savedir) {
        string filename=string(savedir)+"\\"+so.recv();
        ofstream fout(filename,ios_base::binary);
        puts(filename.c_str());
        if(!fout.good())puts("oops\n");
        int blocks;
        int blocksize;
        int last;
        so.recv((char *)&blocks);
        so.recv((char *)&blocksize);
        so.recv((char *)&last);
        printf("\n%d %d %d\n",blocks,blocksize,last);
        char *data=new char[blocksize];
        while(blocks--) {
            so.recv(data);
            fout.write(data,blocksize);
        }
        delete[] data;
        if(last!=0) {
            data=new char[last];
            so.recv(data);
            fout.write(data,last);
            delete[] data;
        }
        fout.close();
    }

}
