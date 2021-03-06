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

#include <ghoul/misc/dictionary.h>
#include <ghoul/misc/templatefactory.h>

 /*
  * Test checklist:
  * +++ Correctness for direct subclass
  * +++ Non-interference
  * +++ Deep inheritance
  * +++ Default constructor
  * +++ Default constructor does not exist
  * +++ Dictionary constructor
  * +++ Dictionary constructor does not exist
  * +++ Default + Dictionary ctor
  * +++ Class does not exist
  * +++ Correctness for 'hasClass'
  * +++ Custom factory function pointer
  * +++ Custom factory std::function
  */

namespace {
    struct BaseClass {
    public:
        virtual ~BaseClass() {} // virtual method necessary for RTTI

        int value1 = -1;
        int value2 = -2;
    };

    struct SubClassDefault : public BaseClass {
    public:
        SubClassDefault() : BaseClass() {
            value1 = 1;
            value2 = 2;
        };
    };

    struct SubClassDefault2 : public BaseClass {
    public:
        SubClassDefault2() : BaseClass() {
            value1 = 21;
            value2 = 22;
        }
    };

    struct SubClassDictionary : public BaseClass {
    public:
        explicit SubClassDictionary(const ghoul::Dictionary& dict) : BaseClass() {
            dict.getValue("value1", value1);
            dict.getValue("value2", value2);
        };
    };

    struct SubClassDefaultDictionary : public BaseClass {
    public:
        SubClassDefaultDictionary() : BaseClass() {
            value1 = 31;
            value2 = 32;
        };
        explicit SubClassDefaultDictionary(const ghoul::Dictionary& dict) : BaseClass() {
            dict.getValue("value1", value1);
            dict.getValue("value2", value2);
        };
    };

    struct SubClassMultipleLayers : public SubClassDefault {
        SubClassMultipleLayers() {};
    };

    struct FunctionPointerClass : public BaseClass {};
    struct StdFunctionClass : public BaseClass {};

    BaseClass* createFunctionPointerClass(bool useDictionary, const ghoul::Dictionary&) {
        if (useDictionary) {
            return new FunctionPointerClass;
        }
        else {
            return nullptr;
        }
    }
} // namespace

TEST_CASE("TemplateFactory: Correctness Direct Subclass", "[templatefactory]") {
    ghoul::TemplateFactory<BaseClass> factory;

    factory.registerClass<SubClassDefault>("SubClassDefault");

    std::unique_ptr<BaseClass> obj = factory.create("SubClassDefault");
    REQUIRE(obj != nullptr);

    SubClassDefault* derived = dynamic_cast<SubClassDefault*>(obj.get());
    REQUIRE(derived != nullptr);
}

TEST_CASE("TemplateFactory: Correctness Deep SubClass", "[templatefactory]") {
    ghoul::TemplateFactory<BaseClass> factory;

    factory.registerClass<SubClassMultipleLayers>("SubClassMultipleLayers");

    std::unique_ptr<BaseClass> obj = factory.create("SubClassMultipleLayers");
    REQUIRE(obj != nullptr);

    SubClassMultipleLayers* derived = dynamic_cast<SubClassMultipleLayers*>(obj.get());
    REQUIRE(derived != nullptr);
}

TEST_CASE("TemplateFactory: Non Interference", "[templatefactory]") {
    ghoul::TemplateFactory<BaseClass> factory;

    factory.registerClass<SubClassDefault>("SubClassDefault");
    factory.registerClass<SubClassDefault2>("SubClassDefault2");

    std::unique_ptr<BaseClass> obj = factory.create("SubClassDefault");
    REQUIRE(obj != nullptr);
    std::unique_ptr<BaseClass> obj2 = factory.create("SubClassDefault2");
    REQUIRE(obj2 != nullptr);
    REQUIRE(obj != obj2);

    SubClassDefault* derived = dynamic_cast<SubClassDefault*>(obj.get());
    REQUIRE(derived != nullptr);

    SubClassDefault2* derived2 = dynamic_cast<SubClassDefault2*>(obj2.get());
    REQUIRE(derived2 != nullptr);
}

TEST_CASE("TemplateFactory: Default Constructor", "[templatefactory]") {
    ghoul::TemplateFactory<BaseClass> factory;

    factory.registerClass<SubClassDefault>("SubClassDefault");

    std::unique_ptr<BaseClass> obj = factory.create("SubClassDefault");
    REQUIRE(obj != nullptr);

    REQUIRE(obj->value1 == 1);
    REQUIRE(obj->value2 == 2);
}

TEST_CASE("TemplateFactory: No Default Constructor Exists", "[templatefactory]") {
    ghoul::TemplateFactory<BaseClass> factory;

    factory.registerClass<SubClassDictionary>("SubClassDictionary");

    REQUIRE_THROWS_AS(
        factory.create("SubClassDictionary"),
        ghoul::TemplateFactoryBase::TemplateConstructionError
    );
}

TEST_CASE("TemplateFactory: Dictionary Constructor", "[templatefactory]") {
    ghoul::TemplateFactory<BaseClass> factory;

    factory.registerClass<SubClassDictionary>("SubClassDictionary");

    ghoul::Dictionary dict = { { "value1", 100 }, { "value2", 200 } };
    std::unique_ptr<BaseClass> obj = factory.create("SubClassDictionary", dict);
    REQUIRE(obj != nullptr);

    REQUIRE(obj->value1 == 100);
    REQUIRE(obj->value2 == 200);
}

TEST_CASE("TemplateFactory: No Dictionary Constructor Exists", "[templatefactory]") {
    ghoul::TemplateFactory<BaseClass> factory;

    factory.registerClass<SubClassDefault>("SubClassDefault");

    ghoul::Dictionary dict = { { "value1", 100 }, { "value2", 200 } };

    REQUIRE_THROWS_AS(
        factory.create("SubClassDefault", dict),
        ghoul::TemplateFactoryBase::TemplateConstructionError
    );
}

TEST_CASE("TemplateFactory: Class Does Not Exist", "[templatefactory]") {
    ghoul::TemplateFactory<BaseClass> factory;

    factory.registerClass<SubClassDefault>("SubClassDefault");

    std::unique_ptr<BaseClass> obj = factory.create("SubClassDefault");
    REQUIRE(obj != nullptr);

    REQUIRE_THROWS_AS(
        factory.create("DoesNotExist"),
        ghoul::TemplateFactoryBase::TemplateClassNotFoundError
    );
}

TEST_CASE("TemplateFactory: Default Dictionary Constructor", "[templatefactory]") {
    ghoul::TemplateFactory<BaseClass> factory;

    // SubClassDefaultDictionary 31 32
    factory.registerClass<SubClassDefaultDictionary>("class");

    std::unique_ptr<BaseClass> obj = factory.create("class");
    REQUIRE(obj != nullptr);
    REQUIRE(obj->value1 == 31);
    REQUIRE(obj->value2 == 32);

    ghoul::Dictionary dict = { { "value1", 41 }, { "value2", 42 } };
    std::unique_ptr<BaseClass> obj2 = factory.create("class", dict);
    REQUIRE(obj2 != nullptr);
    REQUIRE(obj2->value1 == 41);
    REQUIRE(obj2->value2 == 42);
}

TEST_CASE("TemplateFactory: Correctness For HasClass", "[templatefactory]") {
    ghoul::TemplateFactory<BaseClass> factory;

    factory.registerClass<SubClassDictionary>("SubClassDictionary");

    REQUIRE(factory.hasClass("SubClassDictionary"));
    REQUIRE_FALSE(factory.hasClass("DoesNotExist"));
}

TEST_CASE("TemplateFactory: Function Pointer Construction", "[templatefactory]") {
    ghoul::TemplateFactory<BaseClass> factory;

    factory.registerClass("ptr", &createFunctionPointerClass);

    std::unique_ptr<BaseClass> obj = factory.create("ptr");
    REQUIRE(obj == nullptr);

    std::unique_ptr<BaseClass> obj2 = factory.create("ptr", {});
    REQUIRE(obj2 != nullptr);
}

TEST_CASE("TemplateFactory: Std Function Construction", "[templatefactory]") {
    ghoul::TemplateFactory<BaseClass> factory;

    std::function<BaseClass* (bool, const ghoul::Dictionary&)> function =
        [](bool use, const ghoul::Dictionary&) -> BaseClass* {
            return use ? new StdFunctionClass : nullptr;
    };
    factory.registerClass("ptr", function);

    std::unique_ptr<BaseClass> obj = factory.create("ptr");
    REQUIRE(obj == nullptr);

    std::unique_ptr<BaseClass> obj2 = factory.create("ptr", {});
    REQUIRE(obj2 != nullptr);
}
