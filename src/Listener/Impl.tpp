#pragma once
#include "./Event.hpp"


namespace events{

/* EVENT EMITTER */

template<typename... Argv>
void 
EventEmitter::on(HandlerEvent::key_t key, auto&& callback)
{ _handler.template handle<void(Argv...)>(std::move(key), callback); }


void 
EventEmitter::undo(HandlerEvent::key_t key)
{ _handler.unhandle(std::move(key)); }

template<typename... Argv>
void 
EventEmitter::emit(HandlerEvent::key_t key, Argv&&... params)
{ _handler.template call<void(Argv...)>(std::move(key), std::forward<Argv>(params)...); };

template<typename TimeT, typename... Argv>
auto* 
EventEmitter::emitWork(HandlerEvent::key_t key, duration_t<TimeT>&& timetowait, Argv&&... params){
    auto* ptr = _works.emplace_back(
        new std::thread{[this, key, timetowait](auto&&... argvs){
            std::this_thread::sleep_for( timetowait );
            this->emit(std::move(key), std::forward<Argv>(argvs)...);
        }, std::forward<Argv>(params)...}
    );
    return ptr;
}

template<typename... Argv>
std::function<void(Argv...)>&
EventEmitter::get(HandlerEvent::key_t key) noexcept{
    for( auto& elmnt : _handler ){
        if( elmnt.first != key )
            continue;
        auto* _eventCallable = 
            dynamic_cast<
                _detail::EventImpl<void(Argv...)>*
            >( elmnt.second );
        return _eventCallable->_callback;
    }
    this->on<Argv...>(key,[](Argv&&... params){});
    return this->template get<Argv...>(key);
}

template<typename... Argv>
std::function<void(Argv...)>& 
EventEmitter::at(HandlerEvent::key_t key) const{
    for( auto& elmnt : _handler ){
        if( elmnt.first != key )
            continue;
        auto* _eventCallable = 
            dynamic_cast<
                _detail::EventImpl<void(Argv...)>*
            >( elmnt.second );
        return _eventCallable->_callback;
    }
    throw std::runtime_error("Event inexistent!");
}

template<typename... Argv>
std::function<void(Argv...)>& 
EventEmitter::operator[](HandlerEvent::key_t key) noexcept
{ return this->template get<Argv...>(key); }

void 
EventEmitter::waitWorks(){
    for(auto* work: _works){
        if( work->joinable() )
            work->join();
        delete work;
    }
    _works.clear();
}

/* HANDLER EVENTS */

template<typename FunctionType>
void 
HandlerEvent::handle(HandlerEvent::key_t&& __key, auto&& callback) noexcept {
    _events[__key.c_str()] = 
        dynamic_cast<_detail::EventBase_t*>(
            new _detail::EventImpl<FunctionType>(
                callback
            )
        )
    ;
}

template<typename FunctionType>
void 
HandlerEvent::handle(HandlerEvent::key_t& __key, auto&& callback) noexcept 
{ this->template handle<FunctionType>(std::move(__key), std::forward(callback)); }

void 
HandlerEvent::unhandle(HandlerEvent::key_t&& __key) noexcept {
    if ( _events.count(__key) > 0 ){
        delete _events[__key];
        _events.erase(__key);
    }
}

void 
HandlerEvent::unhandle(HandlerEvent::key_t& __key) noexcept 
{ this->unhandle( std::move(__key) ); }

auto 
HandlerEvent::call(HandlerEvent::key_t&& _key) const noexcept{
    if( _events.count(_key) > 0 ){
        auto* _clbk = _events.at(_key);
        auto* _eventCallable = 
            dynamic_cast<
                _detail::EventImpl<void()>*
            >( _clbk );
        return _eventCallable->template call();
    }
    return;
}

template<typename Func_t, typename... Argvs>
auto 
HandlerEvent::call(const HandlerEvent::key_t&& _key, Argvs&&... params) const noexcept{
    if( _events.count(_key) > 0 ){
        auto* _clbk = _events.at(_key);
        auto* _eventCallable = 
            reinterpret_cast<
                _detail::EventImpl<Func_t>*
            >( _clbk );
        return _eventCallable->template call( std::forward<Argvs>(params)... );
    }
    return;
}

template<typename Func_t, typename... Argvs>
auto 
HandlerEvent::call(const HandlerEvent::key_t& _key, Argvs&&... params) const noexcept
{ return this->call(std::move(_key), std::forward<Argvs>(params)...); }


auto 
HandlerEvent::call(HandlerEvent::key_t& _key) const noexcept
{ return this->call(std::move(_key)); }


bool 
HandlerEvent::exists(HandlerEvent::key_t&& _key) const noexcept 
{ return ( _events.count(_key) > 0 ); }

bool 
HandlerEvent::exists(HandlerEvent::key_t& _key) const noexcept 
{ return ( _events.count(_key) > 0 ); }



} // events