#include <iostream>
#include "private_access.h"

// Определения макросов
#define EXPOSE_MEMBER(Class, Name) \
    template struct private_access::init_member<Class, decltype(Class::Name), &Class::Name>;

#define EXPOSE_METHOD(Class, Method, Signature) \
    template struct private_access::init_member<Class, Signature, &Class::Method>;

#define EXPOSE_STATIC_MEMBER(Class, Name) \
    template struct private_access::init_static_member<Class, decltype(Class::Name), &Class::Name>;

#define EXPOSE_STATIC_METHOD(Class, Method, Signature) \
    template struct private_access::init_static_member<Class, Signature, &Class::Method>;

// ==================== Пример класса ====================
class Dog {
public:
    Dog(std::string name) : name(name) {}

private:
    std::string bark() const { return name + ": Woof!"; }
    void rename(std::string new_name) { name = new_name; }

    static std::string species() { return "Canis familiaris"; }
    static int legs() { return 4; }

    std::string name = "";
    int age = 1000;
    static inline std::string secret = "Private static!";
};

// ==================== Инициализация указателей ====================
// Для полей
EXPOSE_MEMBER(Dog, name); // template struct private_access::init_member<Dog, std::string, &Dog::name>;
EXPOSE_MEMBER(Dog, age); // template struct private_access::init_member<Dog, int, &Dog::age>;

// Для методов
EXPOSE_METHOD(Dog, bark, std::string() const); // template struct private_access::init_member<Dog, std::string() const, &Dog::bark>;
EXPOSE_METHOD(Dog, rename, void(std::string)); // template struct private_access::init_member<Dog, void(std::string), &Dog::rename>;

// Статические члены
EXPOSE_STATIC_MEMBER(Dog, secret); // template struct private_access::init_static_member<Dog, std::string, &Dog::secret>;
EXPOSE_STATIC_METHOD(Dog, species, std::string()); // template struct private_access::init_static_member<Dog, std::string(), &Dog::species>;
EXPOSE_STATIC_METHOD(Dog, legs, int()); // template struct private_access::init_static_member<Dog, int(), &Dog::legs>;



// ==================== Использование ====================
int main() {
    Dog fido("Fido");

    // Доступ к членам-данным
    fido.*private_access::access_member<Dog, std::string>::ptr = "Rex";
    std::cout << fido.*private_access::access_member<Dog, std::string>::ptr << std::endl;  // Rex

    std::cout << fido.*private_access::access_member<Dog, int>::ptr << std::endl;     // 1000
    fido.*private_access::access_member<Dog, int>::ptr = 5;
    std::cout << fido.*private_access::access_member<Dog, int>::ptr << std::endl;     // 5

    // Статическая переменная
    std::cout << *private_access::access_static_member<Dog, std::string>::ptr << std::endl; // Private static!

    // Вызов методов
    // Не-const метод
    (fido.*private_access::access_member<Dog, void(std::string)>::ptr)("Max");

    // Const метод
    std::cout << (fido.*private_access::access_member<Dog, std::string() const>::ptr)() << std::endl; // Max: Woof!

    // Статические методы
    std::cout << (*private_access::access_static_member<Dog, std::string()>::ptr)() << std::endl; // Canis familiaris
    std::cout << (*private_access::access_static_member<Dog, int()>::ptr)() << std::endl;    // 4
}