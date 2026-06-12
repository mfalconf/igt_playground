
#include "demo_plugin/Greeter.h"
#include "demo_plugin/HelloGreeter.h"

namespace demo_plugin {

std::string HelloGreeter::greet(const std::string& name) {
    return std::string("Hello, ") + name;
}

} // namespace demo_plugin
