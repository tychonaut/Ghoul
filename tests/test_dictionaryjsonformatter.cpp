/*****************************************************************************************
 *                                                                                       *
 * GHOUL                                                                                 *
 * General Helpful Open Utility Library                                                  *
 *                                                                                       *
 * Copyright (c) 2012-2020                                                               *
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

#include "catch2/catch.hpp"

#include <ghoul/misc/dictionaryjsonformatter.h>
#include <ghoul/misc/dictionary.h>
#include <string>

TEST_CASE("DictionaryJsonFormatter: Empty Dictionary", "[dictionaryjsonformatter]") {
    ghoul::Dictionary d;
    std::string res = ghoul::formatJson(d);
    REQUIRE(res == "{}");
}

TEST_CASE("DictionaryJsonFormatter: Simple Dictionary", "[dictionaryjsonformatter]") {
    using namespace std::string_literals;
    ghoul::Dictionary d = {
        { "int", 1 },
        { "double", 2.0 },
        { "vec2", glm::vec2(0.f) },
        { "vec3", glm::vec3(0.f) },
        { "vec4", glm::vec4(0.f) },
        { "string", ""s}
    };

    std::string res = ghoul::formatJson(d);
    REQUIRE(
        res ==
        "{\"double\":2.000000E0,\"int\":1,\"string\":\"\","
        "\"vec2\":[0,0],\"vec3\":[0,0,0],"
        "\"vec4\":[0,0,0,0]}"
    );
}

TEST_CASE("DictionaryJsonFormatter: Nested Dictionary", "[dictionaryjsonformatter]") {
    using namespace std::string_literals;

    ghoul::Dictionary d = {
        { "int", 1 },
        { "double", 2.0 },
        { "vec2", glm::vec2(0.f) },
        { "vec3", glm::vec3(0.f) },
        { "vec4", glm::vec4(0.f) },
        { "string", ""s }
    };

    ghoul::Dictionary e = {
        { "int", 1 },
        { "double", 2.0 },
        { "vec2", glm::vec2(0.f) },
        { "vec3", glm::vec3(0.f) },
        { "vec4", glm::vec4(0.f) },
        { "string", ""s },
        { "dict" , d}
    };

    ghoul::Dictionary f = {
        { "int", 1 },
        { "double", 2.0 },
        { "vec2", glm::vec2(0.f) },
        { "vec3", glm::vec3(0.f) },
        { "vec4", glm::vec4(0.f) },
        { "string", ""s },
        { "dict" , e }
    };

    ghoul::Dictionary g = {
        { "int", 1 },
        { "double", 2.0 },
        { "vec2", glm::vec2(0.f) },
        { "vec3", glm::vec3(0.f) },
        { "vec4", glm::vec4(0.f) },
        { "string", ""s },
        { "dict" , f },
        { "dict2" , f },
        { "dict3" , f }
    };

    std::string res = ghoul::formatJson(g);

    REQUIRE(
        res ==
        "{\"dict\":{\"dict\":{\"dict\":{\"double\":2.000000E0,\"int\":1,"
        "\"string\":\"\",\"vec2\":[0,0],"
        "\"vec3\":[0,0,0],"
        "\"vec4\":[0,0,0,0]},"
        "\"double\":2.000000E0,\"int\":1,\"string\":\"\","
        "\"vec2\":[0,0],"
        "\"vec3\":[0,0,0],"
        "\"vec4\":[0,0,0,0]},"
        "\"double\":2.000000E0,\"int\":1,\"string\":\"\","
        "\"vec2\":[0,0],\"vec3\":[0,0,0],"
        "\"vec4\":[0,0,0,0]},"
        "\"dict2\":{\"dict\":{\"dict\":{\"double\":2.000000E0,\"int\":1,\"string\":\"\","
        "\"vec2\":[0,0],\"vec3\":[0,0,0],"
        "\"vec4\":[0,0,0,0]},\"double\":2.000000E0,\"int\":1,"
        "\"string\":\"\",\"vec2\":[0,0],"
        "\"vec3\":[0,0,0],"
        "\"vec4\":[0,0,0,0]},\"double\":2.000000E0,\"int\":1,"
        "\"string\":\"\",\"vec2\":[0,0],"
        "\"vec3\":[0,0,0],"
        "\"vec4\":[0,0,0,0]},\"dict3\":{\"dict\":{\"dict\":{"
        "\"double\":2.000000E0,\"int\":1,\"string\":\"\",\"vec2\":[0,0],"
        "\"vec3\":[0,0,0],"
        "\"vec4\":[0,0,0,0]},\"double\":2.000000E0,\"int\":1,"
        "\"string\":\"\",\"vec2\":[0,0],"
        "\"vec3\":[0,0,0],"
        "\"vec4\":[0,0,0,0]},\"double\":2.000000E0,\"int\":1,"
        "\"string\":\"\",\"vec2\":[0,0],"
        "\"vec3\":[0,0,0],"
        "\"vec4\":[0,0,0,0]},\"double\":2.000000E0,\"int\":1,"
        "\"string\":\"\",\"vec2\":[0,0],"
        "\"vec3\":[0,0,0],"
        "\"vec4\":[0,0,0,0]}"
    );
}
