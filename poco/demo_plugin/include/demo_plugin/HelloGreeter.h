#pragma once
#include "demo_plugin/Greeter.h"

namespace demo_plugin {

class HelloGreeter : public Greeter {
public:
    HelloGreeter() = default;
    ~HelloGreeter() override = default;
    std::string greet(const std::string& name) override;
};

} // namespace demo_plugin
