
namespace private_access {

    // ==================== ������� ������� ====================
    template<typename ClassType, typename MemberType>
    struct access_member {
        static MemberType ClassType::* ptr;
    };

    template<typename ClassType, typename MemberType>
    MemberType ClassType::* access_member<ClassType, MemberType>::ptr = nullptr;

    // ������������� ��� �������
    template<typename ClassType, typename RetType, typename... Args>
    struct access_member<ClassType, RetType(Args...)> {
        static RetType(ClassType::* ptr)(Args...);
    };

    template<typename ClassType, typename RetType, typename... Args>
    RetType(ClassType::* access_member<ClassType, RetType(Args...)>::ptr)(Args...) = nullptr;

    // ������������� ��� const �������
    template<typename ClassType, typename RetType, typename... Args>
    struct access_member<ClassType, RetType(Args...) const> {
        static RetType(ClassType::* ptr)(Args...) const;
    };

    template<typename ClassType, typename RetType, typename... Args>
    RetType(ClassType::* access_member<ClassType, RetType(Args...) const>::ptr)(Args...) const = nullptr;

    // ������� ��� ����������� ������
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

    // ==================== �������������� ====================
    // ��� ������-������
    template<typename ClassType, typename MemberType, MemberType ClassType::* ptr>
    struct init_member {
        init_member() { access_member<ClassType, MemberType>::ptr = ptr; }
        static init_member instance;
    };

    template<typename ClassType, typename MemberType, MemberType ClassType::* ptr>
    init_member<ClassType, MemberType, ptr> init_member<ClassType, MemberType, ptr>::instance;

    // ��� �������
    template<typename ClassType, typename RetType, typename... Args, RetType(ClassType::* ptr)(Args...)>
    struct init_member<ClassType, RetType(Args...), ptr> {
        init_member() { access_member<ClassType, RetType(Args...)>::ptr = ptr; }
        static init_member instance;
    };

    template<typename ClassType, typename RetType, typename... Args, RetType(ClassType::* ptr)(Args...)>
    init_member<ClassType, RetType(Args...), ptr> init_member<ClassType, RetType(Args...), ptr>::instance;

    // ��� const �������
    template<typename ClassType, typename RetType, typename... Args, RetType(ClassType::* ptr)(Args...) const>
    struct init_member<ClassType, RetType(Args...) const, ptr> {
        init_member() { access_member<ClassType, RetType(Args...) const>::ptr = ptr; }
        static init_member instance;
    };

    template<typename ClassType, typename RetType, typename... Args, RetType(ClassType::* ptr)(Args...) const>
    init_member<ClassType, RetType(Args...) const, ptr> init_member<ClassType, RetType(Args...) const, ptr>::instance;

    // ��� ����������� ������
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