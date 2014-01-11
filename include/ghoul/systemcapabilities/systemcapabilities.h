/*****************************************************************************************
 *                                                                                       *
 * GHOUL                                                                                 *
 * General Helpful Open Utility Library                                                  *
 *                                                                                       *
 * Copyright (c) 2012 Alexander Bock                                                     *
 *                                                                                       *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this  *
 * software and associated documentation files (the "Software"), to deal in the Software *
 * without restriction, including without limitation the rights to use, copy, modify,    *
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, and to    *
 * permit persons to whom the Software is furnished to do so, subject to the following   *
 * conditions:                                                                           *
 *                                                                                       *
 * The above copyright notice and this permission notice shall be included in all copies *
 * or substantial portions of the Software.                                              *
 *                                                                                       *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,   *
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A         *
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT    *
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF  *
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE  *
 * OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                         *
 ****************************************************************************************/

#ifndef __SYSTEMCAPABILITIES_H__
#define __SYSTEMCAPABILITIES_H__

#include "systemcapabilities/cpucapabilitiescomponent.h"
#include "systemcapabilities/openglcapabilitiescomponent.h"

#include <string>
#include <vector>

namespace ghoul {
namespace systemcapabilities {

/**
 * The SystemCapabilities class allows access to the functionality the system provides.
 * The class is composed of SystemCapabilityComponent%s with each component checking for a
 * specific kind of capabilities (for example OpenGLCapabilities or CPUCapabilities) and
 * provides access to the components with the templated #component method. The values are
 * not guaranteed to be constant over the lifetime of the application, but most values can
 * be considered to be static and cache-able.
 */

class SystemCapabilities {
public:
    SystemCapabilities();
    ~SystemCapabilities();

    static void initialize();
    static void deinitialize();
    static SystemCapabilities& ref();
    static bool isInitialized();

    void detectCapabilities();
    void logCapabilities() const;

    void addComponent(SystemCapabilitiesComponent* component);

    CPUCapabilitiesComponent* cpuCapabilitiesComponent();
    OpenGLCapabilitiesComponent* openGLCapabilitiesComponent();

    template <class T>
    T* component(const std::string& name);

private:
    /// Not implemented, usage should create linker error
    SystemCapabilities(const SystemCapabilities& rhs);

    void clearCapabilities();

    std::vector<SystemCapabilitiesComponent*> _components;

    static SystemCapabilities* _systemCapabilities;  ///< singleton member
};

} // namespace systemcapabilities
} // namespace ghoul

#define SysCap (ghoul::systemcapabilities::SystemCapabilities::ref())

#endif // __SYSTEMCAPABILITIES_H__
