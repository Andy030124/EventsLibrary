#include "Events.hpp"
#include <iostream>
#include <chrono>
#include <thread>

int main(int argc, char const *argv[]){
    using namespace std::chrono_literals;

    events::EventEmitter mevents{};

    mevents.on("/start", [](){
        std::cout << "Hola Mundo! \n";
    });

    mevents.on<int>("/help", [](int a){
        std::cout << "Ayuda: " << a << "\n";
    });

    mevents.on<int, int, int&>("/sum", [](int a, int b, int& res){
        res = a+b;
        std::cout << "Suma de " << a << " + " << b << " = " << res << "\n";
    });

    mevents.emitWork("/help", 3s, 10);

    mevents.emit("/start");
    mevents.emit("/help", 5);

    mevents.undo("/start");
    auto& func = mevents.get<int>("/help");
    func(5);

    mevents.emit("/start");
    mevents.emit("/help", 2);

    int mresult = 0;

    mevents.emit("/sum", 5, 2, mresult);

    std::cout << "Resultado de suma: " << mresult << std::endl;

    mevents.waitWorks();

    return 0;
}
