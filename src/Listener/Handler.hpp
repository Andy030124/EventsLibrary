#pragma once
#include <unordered_map>
#include <functional>
#include <string>
#include <memory>

namespace events{

namespace _detail{

    struct EventBase_t{
        virtual ~EventBase_t() = default;
    };

    template<typename TF>
    struct EventImpl: EventBase_t{
        using type_t = std::function<TF>;

        explicit EventImpl(auto&& clbk)
        : _callback( clbk )
        {}

        template<typename... Argv>
        auto call(Argv&&... params) 
        { return _callback( std::forward<Argv>(params)... ); }

        type_t _callback{};
    };

} // Intern Implementations

struct HandlerEvent {
    using typeFunc_t = _detail::EventBase_t*;
    using key_t = std::string;
    using container_t = std::unordered_map<key_t, typeFunc_t>;

    explicit HandlerEvent() = default;

    ~HandlerEvent() {
        for(auto ptr : _events){
            delete _events[ ptr.first ];
        }
        _events.clear();
    }

    template<typename FunctionType>
    void handle(key_t&& __key, auto&& callback) noexcept;

    template<typename FunctionType>
    void handle(key_t& __key, auto&& callback) noexcept;

    void unhandle(key_t&& __key) noexcept;

    void unhandle(key_t& __key) noexcept;

    auto call(key_t&& _key) const noexcept;

    template<typename Func_t, typename... Argvs>
    auto call(const key_t&& _key, Argvs&&... params) const noexcept;

    template<typename Func_t, typename... Argvs>
    auto call(const key_t& _key, Argvs&&... params) const noexcept;

    auto call(key_t& _key) const noexcept;

    bool exists(key_t&& _key) const noexcept ;

    bool exists(key_t& _key) const noexcept ;

    auto begin() const noexcept{ return _events.begin(); }
    auto end() const noexcept{ return _events.end(); }

    private:
        container_t _events{};
};


} // events