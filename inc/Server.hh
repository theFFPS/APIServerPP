#pragma once
#ifndef __SERVER__HH__
#define __SERVER__HH__
#include "Request.hh"
#include <iostream>
#include <memory>
#include <cstdint>
#include <future>
#include <map>
using namespace std;
map<string, void (*)(Request request)> handlers;
map<string, int> operationsWithTimes;
Request last;
bool ADD_HANDLER(string path, void (*function)(Request request));
class Listener {
    public:
        string address;
        int port;
        list<string> keys;
        map<string, bool> toggle = {
            { "use_api_key", true }
        };
        Listener(string address, int port);
        ~Listener();
        bool Toggle(string setting, bool value);
        bool Get(string setting);
        bool AddKey(string key);
        bool IsKey(string key);
        bool Listen();
};
#endif