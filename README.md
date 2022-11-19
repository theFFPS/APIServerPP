# APIServer++
## Overview
**Tested only on Linux!**
**Responses of this server sending only JSON data**
**APIServer++** is a C++ library to create servers for API for some web-application.
* [Requirements](https://github.com/theFFPS/APIServerPP#requirements)
* [Building](https://github.com/theFFPS/APIServerPP#building)
* [Installation](https://github.com/theFFPS/APIServerPP#installation)
* [Documetation](https://github.com/theFFPS/APIServerPP#documentation)
* [Example](https://github.com/theFFPS/APIServerPP#example)
> * [Not using API Key](https://github.com/theFFPS/APIServerPP#not-using-api-key)
> * [Using API Key](https://github.com/theFFPS/APIServerPP#using-api-key)
## Requirements
* [LibEvent](https://github.com/libevent/libevent)
* [Make](https://git.savannah.gnu.org/cgit/make.git)
* [G++ (GCC)](https://github.com/gcc-mirror/gcc)
## Building
```c++
make
```
## Installation
```c++
sudo make install
```
## Example
### Using API Key
```c++
#include <APIServer++/Server.hh>
#include <APIServer++/Hash.hh>

void Index(Request request) {
    request.Send(request.url);
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
```
### Not using API Key
```c++
#include <APIServer++/Server.hh>

void Index(Request request) {
    request.Send(request.url);
}

int main() {
    ADD_HANDLER("/index", Index);
    Listener listener = Listener("127.0.0.1", 8080);
    listener.Toggle("use_api_key", false);
    listener.Listen();
    return 0;
}
```
## Documentation
[Documentation](https://github.com/theFFPS/APIServerPP/)
