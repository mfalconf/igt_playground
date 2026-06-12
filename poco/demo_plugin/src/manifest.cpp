#include "Poco/ClassLibrary.h"
#include "demo_plugin/Greeter.h"
#include "demo_plugin/HelloGreeter.h"

POCO_BEGIN_MANIFEST(demo_plugin::Greeter)
    POCO_EXPORT_CLASS(demo_plugin::HelloGreeter)
POCO_END_MANIFEST
