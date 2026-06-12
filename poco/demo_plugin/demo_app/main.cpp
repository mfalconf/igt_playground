#include <iostream>
#include "demo_plugin/Greeter.h"
#include "Poco/ClassLoader.h"
#include "Poco/Exception.h"

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: demo_plugin_app <absolute-plugin-path>\n";
        return 2;
    }
    const char* libPath = argv[1];
    Poco::ClassLoader<demo_plugin::Greeter> loader;
    try {
        loader.loadLibrary(libPath);
        const Poco::Manifest<demo_plugin::Greeter>& manifest = loader.manifestFor(libPath);
        if (manifest.empty()) { std::cerr << "No classes found\n"; return 3; }
        std::string firstClassName = (*manifest.begin())->name();

        demo_plugin::Greeter* g = loader.create(firstClassName);
        std::cout << g->greet("World") << std::endl;
        loader.destroy(firstClassName, g);
        loader.unloadLibrary(libPath);
    } catch (const Poco::Exception& ex) {
        std::cerr << "POCO error: " << ex.displayText() << std::endl;
        return 4;
    }
    return 0;
}
