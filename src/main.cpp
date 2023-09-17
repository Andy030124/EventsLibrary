#include "Events.hpp"
#include <iostream>
#include <chrono>

int main(int argc, char const *argv[]){
    using namespace std::chrono_literals;

    events::EventEmitter mevents{}; // main object

	// create event "/start"
    mevents.on("/start", [](){
        std::cout << "Hola Mundo! \n";
    });

	// to pass an argument to lambda we need specified type on template
    mevents.on<int>("/help", [](int a){
        std::cout << "Ayuda: " << a << "\n";
    });

    mevents.on<int, int, int&>("/sum", [](int a, int b, int& res){
        res = a+b;
        std::cout << "Suma de " << a << " + " << b << " = " << res << "\n";
    });

	// emit a work to time specified and arguments for pass
    mevents.emitWork("/help", 3s, 10);

	// instant emit 
    mevents.emit("/start");
    // instant emit with param
    mevents.emit("/help", 5);
	// disable event
    mevents.undo("/start");
    auto& func = mevents.get<int>("/help"); // get a callback associate with a event
    func(5);

    mevents.emit("/start");
    mevents.emit("/help", 2);

	// support reference
    int mresult = 0;

    mevents.emit("/sum", 5, 2, mresult);

    std::cout << "Resultado de suma: " << mresult << std::endl;

	// is good practice always on the final wait for all works
    mevents.waitWorks();

    return 0;
}
