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

#include <ghoul/filesystem/filesystem.h>
#include <ghoul/misc/csvreader.h>

TEST_CASE("CSVReader: Initial", "[csvreader]") {
    std::string test0 = absPath("${UNIT_TEST}/csvreader/test0.csv");
    std::vector<std::vector<std::string>> t0 = ghoul::loadCSVFile(test0);
}

TEST_CASE("CSVReader: Full Load", "[csvreader]") {
    std::string test0 = absPath("${UNIT_TEST}/csvreader/test0.csv");
    std::vector<std::vector<std::string>> noHeader = ghoul::loadCSVFile(test0);
    std::vector<std::vector<std::string>> header = ghoul::loadCSVFile(test0, true);

    REQUIRE(noHeader.size() == 351);
    REQUIRE(header.size() == 352);

    for (size_t i = 1; i < header.size(); ++i) {
        REQUIRE(noHeader[i - 1].size() == 19);
        REQUIRE(header[i].size() == 19);
        REQUIRE(noHeader[i - 1] == header[i]);
    }

    REQUIRE(header[1][2] == "flora");
    REQUIRE(header[93][10] == "303.82211");
    REQUIRE(header[177][13] == "0.19090799");
    REQUIRE(header[283][2] == "eduarda");
}

TEST_CASE("CSVReader: Column Number", "[csvreader]") {
    std::string test0 = absPath("${UNIT_TEST}/csvreader/test0.csv");
    std::vector<int> col = { 1, 2, 4, 6 };
    std::vector<std::vector<std::string>> noHeader = ghoul::loadCSVFile(test0, col);
    std::vector<std::vector<std::string>> header = ghoul::loadCSVFile(test0, col, true);

    REQUIRE(noHeader.size() == 351);
    REQUIRE(header.size() == 352);

    for (size_t i = 1; i < header.size(); ++i) {
        REQUIRE(noHeader[i - 1].size() == 4);
        REQUIRE(header[i].size() == 4);
        REQUIRE(noHeader[i - 1] == header[i]);
    }

    REQUIRE(header[73][0] == "106");
    REQUIRE(header[227][0] == "279");
    REQUIRE(header[65][2] == "0.15");
    REQUIRE(header[65][3] == "53849");
}

TEST_CASE("CSVReader: Column Name", "[csvreader]") {
    std::string test0 = absPath("${UNIT_TEST}/csvreader/test0.csv");
    //   4, 7, 2
    std::vector<std::string> col = { "slope_g", "obs_num", "designation" };
    std::vector<std::vector<std::string>> noHeader = ghoul::loadCSVFile(test0, col);
    std::vector<std::vector<std::string>> header = ghoul::loadCSVFile(test0, col, true);

    REQUIRE(noHeader.size() == 351);
    REQUIRE(header.size() == 352);

    for (size_t i = 1; i < header.size(); ++i) {
        REQUIRE(noHeader[i - 1].size() == 3);
        REQUIRE(header[i].size() == 3);
        REQUIRE(noHeader[i - 1] == header[i]);
}

    REQUIRE(header[84][0] == "0.15");
    REQUIRE(header[261][1] == "2142");
    REQUIRE(header[84][2] == "peitho");
}
