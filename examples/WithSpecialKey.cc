#include <APIServer++/Server.hh>
#include <APIServer++/Hash.hh>
#include <fstream>
Listener listener = Listener("127.0.0.1", 8080);

void Articles(Request request) {
    string buffer;
    string line;
    ifstream f ("files/file1.json");
    while (getline(f, line)) buffer += line;
    f.close();
    request.Send(buffer);
}

void NewKey(Request request) {
    Hash hash;
    string key = hash.GetHash("trfghjuw34irjfbvdtyuwj", 64);
    ofstream f ("files/keys");
    for (string str : listener.keys) f << str << endl;
    f << key;
    f.close();
    listener.AddKey(key);
    request.Send("\"" + key + "\"");
}

int main() {
    ADD_HANDLER("/articles", Articles);
    ADD_HANDLER("/new_key", NewKey);
    string line;
    listener.Toggle("use_api_key", true);
    listener.AddSpecialKey("adder");
    listener.AllowForSpecialKey("/new_key");
    listener.Toggle("use_special_keys_for_some_links_only", true);
    ifstream f ("files/keys");
    while (getline(f, line)) { listener.AddKey(line); }
    f.close();
    for (string str : listener.keys) cout << "KEY " << str << endl;
    listener.Listen();
    return 0;
}