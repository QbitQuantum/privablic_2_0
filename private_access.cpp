#include <iostream>
#include "private_access.h"

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

// ==================== Инициализация ====================
PRIVATE(Dog, 
    EXPOSE_MEMBER(name);
    EXPOSE_MEMBER(age);

    EXPOSE_METHOD(bark, std::string() const);
    EXPOSE_METHOD(rename, void(std::string));

    EXPOSE_STATIC_MEMBER(secret);
    EXPOSE_STATIC_METHOD(species, std::string());
    EXPOSE_STATIC_METHOD(legs, int())
    );

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