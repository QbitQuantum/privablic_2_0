
namespace private_access {

    // ==================== Базовые шаблоны ====================
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

    // Шаблоны для статических членов
    template<typename ClassType, typename MemberType>
    struct access_static_member {
        static MemberType* ptr;
    };

    template<typename ClassType, typename MemberType>
    MemberType* access_static_member<ClassType, MemberType>::ptr = nullptr;

    template<typename ClassType, typename RetType, typename... Args>
    struct access_static_member<ClassType, RetType(Args...)> {
        static RetType(*ptr)(Args...);
    };

    template<typename ClassType, typename RetType, typename... Args>
    RetType(*access_static_member<ClassType, RetType(Args...)>::ptr)(Args...) = nullptr;

    // ==================== Инициализаторы ====================
    // Для членов-данных
    template<typename ClassType, typename MemberType, MemberType ClassType::* ptr>
    struct init_member {
        init_member() { access_member<ClassType, MemberType>::ptr = ptr; }
        static init_member instance;
    };

    template<typename ClassType, typename MemberType, MemberType ClassType::* ptr>
    init_member<ClassType, MemberType, ptr> init_member<ClassType, MemberType, ptr>::instance;

    // Для методов
    template<typename ClassType, typename RetType, typename... Args, RetType(ClassType::* ptr)(Args...)>
    struct init_member<ClassType, RetType(Args...), ptr> {
        init_member() { access_member<ClassType, RetType(Args...)>::ptr = ptr; }
        static init_member instance;
    };

    template<typename ClassType, typename RetType, typename... Args, RetType(ClassType::* ptr)(Args...)>
    init_member<ClassType, RetType(Args...), ptr> init_member<ClassType, RetType(Args...), ptr>::instance;

    // Для const методов
    template<typename ClassType, typename RetType, typename... Args, RetType(ClassType::* ptr)(Args...) const>
    struct init_member<ClassType, RetType(Args...) const, ptr> {
        init_member() { access_member<ClassType, RetType(Args...) const>::ptr = ptr; }
        static init_member instance;
    };

    template<typename ClassType, typename RetType, typename... Args, RetType(ClassType::* ptr)(Args...) const>
    init_member<ClassType, RetType(Args...) const, ptr> init_member<ClassType, RetType(Args...) const, ptr>::instance;

    // Для статических членов
    template<typename ClassType, typename MemberType, MemberType* ptr>
    struct init_static_member {
        init_static_member() { access_static_member<ClassType, MemberType>::ptr = ptr; }
        static init_static_member instance;
    };

    template<typename ClassType, typename MemberType, MemberType* ptr>
    init_static_member<ClassType, MemberType, ptr> init_static_member<ClassType, MemberType, ptr>::instance;

    template<typename ClassType, typename RetType, typename... Args, RetType(*ptr)(Args...)>
    struct init_static_member<ClassType, RetType(Args...), ptr> {
        init_static_member() { access_static_member<ClassType, RetType(Args...)>::ptr = ptr; }
        static init_static_member instance;
    };

    template<typename ClassType, typename RetType, typename... Args, RetType(*ptr)(Args...)>
    init_static_member<ClassType, RetType(Args...), ptr> init_static_member<ClassType, RetType(Args...), ptr>::instance;
}

// Определения макросов
#define EXPOSE_MEMBER(Class, Name) \
    template struct private_access::init_member<Class, decltype(Class::Name), &Class::Name>;

#define EXPOSE_METHOD(Class, Method, Signature) \
    template struct private_access::init_member<Class, Signature, &Class::Method>;

#define EXPOSE_STATIC_MEMBER(Class, Name) \
    template struct private_access::init_static_member<Class, decltype(Class::Name), &Class::Name>;

#define EXPOSE_STATIC_METHOD(Class, Method, Signature) \
    template struct private_access::init_static_member<Class, Signature, &Class::Method>;