#include <APIServer++/Server.hh>

void Index(Request request) {
    request.Send("\"" + request.url + "\"");
}

int main() {
    ADD_HANDLER("/index", Index);
    Listener listener = Listener("127.0.0.1", 8080);
    listener.Toggle("use_api_key", false);
    listener.Listen();
    return 0;
}
