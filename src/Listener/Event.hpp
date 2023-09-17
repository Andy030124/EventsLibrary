#pragma once
#include "./Handler.hpp"
#include <chrono>
#include <thread>
#include <vector>

namespace events{

struct EventEmitter{
    template<typename T>
    using duration_t = std::chrono::duration<T>;
    
    using duration_nan_t = duration_t<std::chrono::nanoseconds>;
    using duration_mic_t = duration_t<std::chrono::microseconds>;
    using duration_mil_t = duration_t<std::chrono::milliseconds>;
    using duration_sec_t = duration_t<std::chrono::seconds>;
    using duration_min_t = duration_t<std::chrono::minutes>;
    using duration_hou_t = duration_t<std::chrono::hours>;
    

    explicit EventEmitter() = default;
    ~EventEmitter(){
        for(auto* work: _works){
            work->detach();
            delete work;
        }
        _works.clear();
    }

    template<typename... Argv>
    void on(HandlerEvent::key_t key, auto&& callback);

    void undo(HandlerEvent::key_t key);

    template<typename... Argv>
    void emit(HandlerEvent::key_t key, Argv&&... params);

    template<typename TimeT, typename... Argv>
    auto* emitWork(HandlerEvent::key_t key, duration_t<TimeT>&& timetowait, Argv&&... params);

    template<typename... Argv>
    std::function<void(Argv...)>& get(HandlerEvent::key_t key) noexcept;

    template<typename... Argv>
    std::function<void(Argv...)>& at(HandlerEvent::key_t key) const;

    template<typename... Argv>
    std::function<void(Argv...)>& operator[](HandlerEvent::key_t key) noexcept;

    void waitWorks();

    private:
        HandlerEvent _handler{};
        std::vector<std::thread*> _works{};
};





} // Events namespace
