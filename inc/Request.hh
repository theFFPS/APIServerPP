#pragma once
#ifndef __REQUEST__HH__
#define __REQUEST__HH__
#include <string>
#include <evhttp.h>
#include <map>
#include <list>
using namespace std;
class Request { 
    private:
        evbuffer *OutBuf;
    public:
        evhttp_request *data; 
        string url;
        map<string, string> args;
        Request(evhttp_request *data, evbuffer *OutBuf);
        ~Request();
        list<string> split(string text, char del);
        void Send(string text);
        void SendNoCategory(string text);
        string GetArg(string name);
};
#endif