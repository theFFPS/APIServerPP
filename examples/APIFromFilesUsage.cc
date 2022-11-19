#include <APIServer++/Server.hh>
#include <fstream>

void Articles(Request request) {
    string buffer;
    string line;
    ifstream f ("files/file1.json");
    while (getline(f, line)) buffer += line;
    f.close();
    request.Send(buffer);
}

int main() {
    ADD_HANDLER("/articles", Articles);
    string line;
    Listener listener = Listener("127.0.0.1", 8080);
    listener.Toggle("use_api_key", true);
    ifstream f ("files/keys");
    while (getline(f, line)) { listener.AddKey(line); }
    f.close();
    for (string str : listener.keys) cout << "KEY " << str << endl;
    listener.Listen();
    return 0;
}