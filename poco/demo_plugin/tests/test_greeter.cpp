#include <gtest/gtest.h>
#include "demo_plugin/Greeter.h"
#include "Poco/ClassLoader.h"
#include "Poco/Exception.h"
#include <iostream>
#include <vector>

static std::string g_pluginPath;

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    if (argc < 2) {
        std::cerr << "Usage: demo_plugin_tests <absolute-plugin-path>\n";
        return 2;
    }
    g_pluginPath = argv[1];
    return RUN_ALL_TESTS();
}

TEST(Plugin, LoadAndGreet) {
    Poco::ClassLoader<demo_plugin::Greeter> loader;
    EXPECT_NO_THROW(loader.loadLibrary(g_pluginPath));
    const Poco::Manifest<demo_plugin::Greeter>* manifest = loader.findManifest(g_pluginPath);
    ASSERT_NE(manifest, nullptr);

    std::vector<std::string> classes;
    for (auto it = manifest->begin(); it != manifest->end(); ++it) {
        const Poco::AbstractMetaObject<demo_plugin::Greeter>* meta = *it;
        classes.push_back(meta->name());
    }

    EXPECT_GE(classes.size(), 1u);
    demo_plugin::Greeter* g = loader.create(classes.front());
    ASSERT_NE(g, nullptr);
    EXPECT_EQ(g->greet("Test"), "Hello, Test");
    // destroy requires the class name and the object pointer
    loader.destroy(classes.front(), g);
    loader.unloadLibrary(g_pluginPath);
}
