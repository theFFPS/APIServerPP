#include <Server.hh>
#include <evhttp.h>
bool ADD_HANDLER(string path, void (*function)(Request request)) { for (auto val : handlers) { if (val.first == path) return false; } handlers.insert({ path, function }); return true; }
Listener::Listener(string address, int port) { this->port = port; this->address = address; }
Listener::~Listener() {}
bool Listener::Listen() {
    if (!event_init()) { cerr << "Failed to init libevent." << endl; return -1; }
    unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start(address.c_str(), port), &evhttp_free);
    if (!Server) { cerr << "Failed to init http server." << endl; return -1; }
    void (*OnReq)(evhttp_request *req, Listener listener) = [] (evhttp_request *req, Listener listener) {
        auto *OutBuf = evhttp_request_get_output_buffer(req);
        if (listener.Get("use_api_key")) {
            Request request = Request(req, OutBuf);
            if (request.args.size() == 0) { request.SendNoCategory("{ \"success\":false, \"cause\":\"Missing one or more arguments! [ key ]\" }"); return; }
            string key = request.GetArg("key");
            if (! listener.IsKey(key)) { request.SendNoCategory("{ \"success\":false, \"cause\":\"Wrong key provided!\" }"); return; }
        }
        if (!OutBuf) return;
        Request request = Request(req, OutBuf);
        string url = req->uri;
        bool found = false;
        for (auto value : handlers) if (url.find(value.first) != string::npos) { found = true; value.second(request); } 
        if (! found) request.SendNoCategory("{ \"success\":false, \"cause\":\"Not found!\" }");
    };
    evhttp_set_gencb(Server.get(), (void (*) (evhttp_request*, void*))OnReq, (void*)this);
    if (event_dispatch() == -1) {
        cerr << "Failed to run message loop." << endl;
        return -1;
    }
    return true;
}
bool Listener::AddKey(string key) { keys.push_back(key); return true; }
bool Listener::IsKey(string key) { for (string kval : keys) if (kval == key) return true; return false; }
bool Listener::Get(string setting) { for (auto val : toggle) if (val.first == setting) return val.second; return false; }
bool Listener::Toggle(string setting, bool value) {
    map<string, bool> toggles;
    for (auto val : toggle) { if (val.first == setting) continue; toggles.insert(val); }
    toggle = toggles;
    toggle.insert({ setting, value });
    return true;
}