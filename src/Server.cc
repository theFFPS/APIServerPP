#include <Server.hh>
#include <unistd.h>
#include <evhttp.h>
string formatAsCategoryWithArgs(Request req) {
    string finished_args = req.url;
    for (auto arg : req.args) finished_args += arg.first + "=" + arg.second + "&";
    if (finished_args[finished_args.size()-1] == '&') finished_args.pop_back();
    return req.url + "?" + finished_args;
}
void _start_server(string address, int port, Listener *thi) {
    if (!event_init()) { cerr << "Failed to init libevent." << endl; return; }
    unique_ptr<evhttp, decltype(&evhttp_free)> Server(evhttp_start(address.c_str(), port), &evhttp_free);
    if (!Server) { cerr << "Failed to init http server." << endl; return; }
    void (*OnReq)(evhttp_request *req, Listener listener) = [] (evhttp_request *req, Listener listener) {
        auto *OutBuf = evhttp_request_get_output_buffer(req);
        Request request = Request(req, OutBuf);
        last = request;
        if (listener.Get("use_api_key")) {
            if (request.args.size() == 0) { request.SendNoCategory("{ \"success\":false, \"cause\":\"Missing one or more arguments! [ key ]\" }"); return; }
            string key = request.GetArg("key");
            if (! listener.IsKey(key)) { request.SendNoCategory("{ \"success\":false, \"cause\":\"Wrong key provided!\" }"); return; }
            string thisUrl = formatAsCategoryWithArgs(request);
            for (auto val : operationsWithTimes) { if (val.first == thisUrl) if (val.second < 3) { request.SendNoCategory("{ \"success\":false, \"cause\":\"Accessing fast!\" }"); return; } }
            operationsWithTimes.insert({ formatAsCategoryWithArgs(request), 0 });
        }
        if (!OutBuf) return;
        string url = req->uri;
        bool found = false;
        for (auto value : handlers) if (url.find(value.first) != string::npos) { found = true; value.second(request); } 
        if (! found) request.SendNoCategory("{ \"success\":false, \"cause\":\"Not found!\" }");
    };
    evhttp_set_gencb(Server.get(), (void (*) (evhttp_request*, void*))OnReq, (void*)thi);
    if (event_dispatch() == -1) {
        cerr << "Failed to run message loop." << endl;
        return;
    }
}
void _prn_new_conn_info() {
    while (1) {
        if (last.url == "") continue;
        operationsWithTimes.insert({ { formatAsCategoryWithArgs(last), 1 } });
        cout << "URL " << last.url << endl;
        cout << "ARGS [" << endl;
        for (auto arg : last.args) cout << "  [ " << arg.first << " | " << arg.second << " ]" << endl;
        cout << "]" << endl;
        last = Request();
    }
}
void _timers_updating() {
    while (1) {
        map<string, int> toReplace;
        usleep(2000000);
        for (auto val : operationsWithTimes) { if (val.second > 3) continue; toReplace.insert({ val.first, val.second+1 }); }
        operationsWithTimes = toReplace;
    }
}
bool ADD_HANDLER(string path, void (*function)(Request request)) { for (auto val : handlers) { if (val.first == path) return false; } handlers.insert({ path, function }); return true; }
Listener::Listener(string address, int port) { this->port = port; this->address = address; }
Listener::~Listener() {}
bool Listener::Listen() {
    auto a1 = async(launch::async, _start_server, address, port, this);
    auto a2 = async(launch::async, _prn_new_conn_info);
    auto a3 = async(launch::async, _timers_updating);
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