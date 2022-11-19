#include <APIServer++/Server.hh>
#include <APIServer++/Hash.hh>

void Index(Request request) {
    request.Send("\"" + request.url + "\"");
}

int main() {
    ADD_HANDLER("/index", Index);
    Listener listener = Listener("127.0.0.1", 8080);
    listener.Toggle("use_api_key", true);
    Hash hash;
    listener.AddKey(hash.GetHash("tfbju76tfghjuytfhu7tf", 64));
    listener.AddKey(hash.GetHash("tyguitr6572ygyrt6728esd", 64));
    for (string str : listener.keys) cout << "KEY " << str << endl;
    listener.Listen();
    return 0;
}