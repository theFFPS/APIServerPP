#include <Request.hh>
#include <vector>
list<string> Request::split(string text, char del) {
    list<string> out;
    string cache;
    text += del;
    for (int i = 0; i < (int)text.size(); i++) {
        if (text[i] == del) { out.push_back(cache); cache.clear(); continue; }
        cache += text[i];
    }
    return out;
}
Request::Request() { this->args = {}, this->data = NULL, this->url = ""; }
string Request::GetArg(string name) { for (auto val : args) if (val.first == name) return val.second; return ""; }
Request::Request(evhttp_request *data, evbuffer *OutBuf) { 
    this->data = data; 
    this->OutBuf = OutBuf; 
    list<string> splitted = split(data->uri, '?');
    vector<string> splittedAsVector;
    for (string str : splitted) splittedAsVector.push_back(str);
    this->url = splittedAsVector[0];
    if (splittedAsVector.size() >= 2) {
        string argsAsString = splittedAsVector[1];
        list<string> splittedArgs = split(argsAsString, '&');
        for (string str : splittedArgs) {
            list<string> splittedArg = split(str, '=');
            vector<string> splittedArgAsVector;
            for (string token : splittedArg) splittedArgAsVector.push_back(token);
            if (splittedArgAsVector.size() < 2) continue;
            this->args.insert({ splittedArgAsVector[0], splittedArgAsVector[1] });
        }
    } 
}
Request::~Request() {}
void Request::Send(string text) { 
    list<string> splitted = split(url, '/');
    vector<string> splittedAsVector;
    for (string str : splitted) splittedAsVector.push_back(str);
    string category = splittedAsVector[splittedAsVector.size() - 1];
    evhttp_add_header(data->output_headers, "Content-Type", "application/json");
    string toSend = "{ \"success\":true, \"" + category + "\":" + text + " }";
    evbuffer_add_printf(OutBuf, toSend.c_str()); 
    evhttp_send_reply(data, HTTP_OK, "", OutBuf); 
}
void Request::SendNoCategory(string text) { 
    evbuffer_add_printf(OutBuf, text.c_str()); 
    evhttp_add_header(data->output_headers, "Content-Type", "application/json");
    evhttp_send_reply(data, HTTP_OK, "", OutBuf); 
}