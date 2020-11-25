#include <napi.h>
#include <uv.h>
#include <uvw.hpp>
#include <iostream>
#include <future>

std::promise<const char*> conn(uvw::Loop &loop) {
    std::promise<const char*> promise;

    auto tcp = loop.resource<uvw::TCPHandle>();

    tcp->on<uvw::ErrorEvent>([](const uvw::ErrorEvent &, uvw::TCPHandle &) { /* handle errors */ });

    tcp->once<uvw::ConnectEvent>([](const uvw::ConnectEvent &, uvw::TCPHandle &tcp) {
        auto dataWrite = std::unique_ptr<char[]>(new char[2]{ 'b', 'c' });
        tcp.write(std::move(dataWrite), 2);
    });

    tcp->once<uvw::WriteEvent>([&promise](const uvw::WriteEvent &, uvw::TCPHandle &tcp) {
//       std::cout << "written";
       promise.set_value("12345");
       tcp.close();
    });

    tcp->connect(std::string{"127.0.0.1"}, 12345);

    return promise;
}


static Napi::String Method(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
//    std::cout << "1" << std::endl;
    auto loop = uvw::Loop::create();
//    std::cout << "2" << std::endl;
    auto promise = conn(*loop);
//    std::cout << "3" << std::endl;
    auto future = promise.get_future();
//    std::cout << "4" << std::endl;
    loop->run();
    auto result = future.get();
    return Napi::String::New(env, result);
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "hello"),
                Napi::Function::New(env, Method));
    return exports;
}

NODE_API_MODULE(hello, Init)