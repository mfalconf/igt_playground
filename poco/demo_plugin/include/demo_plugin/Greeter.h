#pragma once
#include <string>

namespace demo_plugin {

class Greeter {
public:
    virtual ~Greeter() = default;
    virtual std::string greet(const std::string& name) = 0;
};

} // namespace demo_plugin
