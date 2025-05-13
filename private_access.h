#pragma once

namespace private_access {

    // ==================== 1. Типы указателей на члены ====================

    // Базовый шаблон: указатель на нестатический член
    template<typename ClassType, typename MemberType>
    struct access_member {
        static MemberType ClassType::* ptr;
    };

    template<typename ClassType, typename MemberType>
    MemberType ClassType::* access_member<ClassType, MemberType>::ptr = nullptr;

    // Специализация для методов
    template<typename ClassType, typename RetType, typename... Args>
    struct access_member<ClassType, RetType(Args...)> {
        static RetType(ClassType::* ptr)(Args...);
    };

    template<typename ClassType, typename RetType, typename... Args>
    RetType(ClassType::* access_member<ClassType, RetType(Args...)>::ptr)(Args...) = nullptr;

    // Специализация для const методов
    template<typename ClassType, typename RetType, typename... Args>
    struct access_member<ClassType, RetType(Args...) const> {
        static RetType(ClassType::* ptr)(Args...) const;
    };

    template<typename ClassType, typename RetType, typename... Args>
    RetType(ClassType::* access_member<ClassType, RetType(Args...) const>::ptr)(Args...) const = nullptr;


    // ==================== 2. Типы указателей на статические члены ====================

    // Для статических данных
    template<typename ClassType, typename MemberType>
    struct access_static_member {
        static MemberType* ptr;
    };

    template<typename ClassType, typename MemberType>
    MemberType* access_static_member<ClassType, MemberType>::ptr = nullptr;

    // Для статических функций
    template<typename ClassType, typename RetType, typename... Args>
    struct access_static_member<ClassType, RetType(Args...)> {
        static RetType(*ptr)(Args...);
    };

    template<typename ClassType, typename RetType, typename... Args>
    RetType(*access_static_member<ClassType, RetType(Args...)>::ptr)(Args...) = nullptr;


    // ==================== 3. Инициализаторы указателей ====================

    // Общий случай: инициализация нестатического члена
    template<typename ClassType, typename MemberType, MemberType ClassType::* ptr>
    struct init_member {
        init_member() { access_member<ClassType, MemberType>::ptr = ptr; }
        static init_member instance;
    };

    template<typename ClassType, typename MemberType, MemberType ClassType::* ptr>
    init_member<ClassType, MemberType, ptr> init_member<ClassType, MemberType, ptr>::instance;

    // Специализация: инициализация нестатического метода
    template<typename ClassType, typename RetType, typename... Args, RetType(ClassType::* ptr)(Args...)>
    struct init_member<ClassType, RetType(Args...), ptr> {
        init_member() { access_member<ClassType, RetType(Args...)>::ptr = ptr; }
        static init_member instance;
    };

    template<typename ClassType, typename RetType, typename... Args, RetType(ClassType::* ptr)(Args...)>
    init_member<ClassType, RetType(Args...), ptr> init_member<ClassType, RetType(Args...), ptr>::instance;

    // Специализация: инициализация const метода
    template<typename ClassType, typename RetType, typename... Args, RetType(ClassType::* ptr)(Args...) const>
    struct init_member<ClassType, RetType(Args...) const, ptr> {
        init_member() { access_member<ClassType, RetType(Args...) const>::ptr = ptr; }
        static init_member instance;
    };

    template<typename ClassType, typename RetType, typename... Args, RetType(ClassType::* ptr)(Args...) const>
    init_member<ClassType, RetType(Args...) const, ptr> init_member<ClassType, RetType(Args...) const, ptr>::instance;


    // Инициализация статических членов
    template<typename ClassType, typename MemberType, MemberType* ptr>
    struct init_static_member {
        init_static_member() { access_static_member<ClassType, MemberType>::ptr = ptr; }
        static init_static_member instance;
    };

    template<typename ClassType, typename MemberType, MemberType* ptr>
    init_static_member<ClassType, MemberType, ptr> init_static_member<ClassType, MemberType, ptr>::instance;

    // Инициализация статических методов
    template<typename ClassType, typename RetType, typename... Args, RetType(*ptr)(Args...)>
    struct init_static_member<ClassType, RetType(Args...), ptr> {
        init_static_member() { access_static_member<ClassType, RetType(Args...)>::ptr = ptr; }
        static init_static_member instance;
    };

    template<typename ClassType, typename RetType, typename... Args, RetType(*ptr)(Args...)>
    init_static_member<ClassType, RetType(Args...), ptr> init_static_member<ClassType, RetType(Args...), ptr>::instance;


    // ==================== 4. Макросы для удобного использования ====================

    // Макросы для нестатических членов
#define _EXPOSE_MEMBER_IMPL(Class, Name) \
    template struct private_access::init_member<Class, decltype(Class::Name), &Class::Name>;

#define _EXPOSE_METHOD_IMPL(Class, Method, Signature) \
    template struct private_access::init_member<Class, Signature, &Class::Method>;

// Макросы для статических членов
#define _EXPOSE_STATIC_MEMBER_IMPL(Class, Name) \
    template struct private_access::init_static_member<Class, decltype(Class::Name), &Class::Name>;

#define _EXPOSE_STATIC_METHOD_IMPL(Class, Method, Signature) \
    template struct private_access::init_static_member<Class, Signature, &Class::Method>;

// Макросы с контекстом (через __private_access_class)
#define EXPOSE_MEMBER(Name) \
    _EXPOSE_MEMBER_IMPL(__private_access_class, Name)

#define EXPOSE_METHOD(Method, Signature) \
    _EXPOSE_METHOD_IMPL(__private_access_class, Method, Signature)

#define EXPOSE_STATIC_MEMBER(Name) \
    _EXPOSE_STATIC_MEMBER_IMPL(__private_access_class, Name)

#define EXPOSE_STATIC_METHOD(Method, Signature) \
    _EXPOSE_STATIC_METHOD_IMPL(__private_access_class, Method, Signature)

// Основной макрос PRIVATE
#define PRIVATE(Class, Body)             \
    namespace private_access {           \
    typedef Class __private_access_class; \
    Body }

} // namespace private_access