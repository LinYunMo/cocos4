/*
 Copyright (c) 2026 Xiamen Yaji Software Co., Ltd.

 https://www.cocos.com/

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
*/

#include "platform/interfaces/modules/canvas/CanvasFont.h"

#include <cstdlib>
#include <regex>

namespace cc::detail {

CanvasFont parseCanvasFont(const std::string &font) {
#if CC_PLATFORM == CC_PLATFORM_WINDOWS

    std::string boldStr;
    std::string fontName = "Arial";
    std::string fontSizeStr = "30";

    std::regex re(R"(\s*((\d+)([\.]\d+)?)px\s+([^\r\n]*))");
    std::match_results<std::string::const_iterator> results;
    if (std::regex_search(font.cbegin(), font.cend(), results, re)) {
        fontSizeStr = results[2].str();
        // support get font name from `60px American` or `60px "American abc-abc_abc"`
        // support get font name contain space,example `times new roman`
        // if regex rule that does not conform to the rules,such as Chinese,it defaults to Arial
        std::match_results<std::string::const_iterator> fontResults;
        std::regex fontRe(R"(([\w\s-]+|"[\w\s-]+"$))");
        std::string tmp(results[4].str());
        if (std::regex_match(tmp, fontResults, fontRe)) {
            fontName = results[4].str();
        }
    }

    auto fontSize = static_cast<float>(atof(fontSizeStr.c_str()));
    bool isBold = !boldStr.empty() || font.find("bold", 0) != std::string::npos || font.find("Bold", 0) != std::string::npos;
    bool isItalic = font.find("italic", 0) != std::string::npos || font.find("Italic", 0) != std::string::npos;
    return {fontName, static_cast<float>(fontSize), isBold, isItalic, false, false};
#elif CC_PLATFORM == CC_PLATFORM_QNX

    std::string boldStr;
    std::string fontName = "Arial";
    std::string fontSizeStr = "30";

    // support get font name from `60px American` or `60px "American abc-abc_abc"`
    std::regex re("(bold)?\\s*((\\d+)([\\.]\\d+)?)px\\s+([\\w-]+|\"[\\w -]+\"$)");
    std::match_results<std::string::const_iterator> results;
    if (std::regex_search(font.cbegin(), font.cend(), results, re)) {
        boldStr = results[1].str();
        fontSizeStr = results[2].str();
        fontName = results[5].str();
    }
    bool isItalic = font.find("italic", 0) != std::string::npos || font.find("Italic", 0) != std::string::npos;
    auto fontSize = static_cast<float>(atof(fontSizeStr.c_str()));
    bool isBold = !boldStr.empty() || font.find("bold", 0) != std::string::npos || font.find("Bold", 0) != std::string::npos;
    // SE_LOGD("CanvasRenderingContext2D::setfont: %s, Size: %f, isBold: %b\n", fontName.c_str(), fontSize, isBold);
    return {fontName, fontSize, isBold, isItalic, false, false};
#elif CC_PLATFORM == CC_PLATFORM_LINUX

    std::string fontName = "sans-serif";
    std::string fontSizeStr = "30";
    std::regex re(R"(\s*((\d+)([\.]\d+)?)px\s+([^\r\n]*))");
    std::match_results<std::string::const_iterator> results;
    if (std::regex_search(font.cbegin(), font.cend(), results, re)) {
        fontSizeStr = results[2].str();
        // support get font name from `60px American` or `60px "American abc-abc_abc"`
        // support get font name contain space,example `times new roman`
        // if regex rule that does not conform to the rules,such as Chinese,it defaults to sans-serif
        std::match_results<std::string::const_iterator> fontResults;
        std::regex fontRe(R"(([\w\s-]+|"[\w\s-]+"$))");
        std::string tmp(results[4].str());
        if (std::regex_match(tmp, fontResults, fontRe)) {
            // fontName = results[4].str();
        }
    }

    bool isBold = font.find("bold", 0) != std::string::npos || font.find("Bold", 0) != std::string::npos;
    bool isItalic = font.find("italic", 0) != std::string::npos || font.find("Italic", 0) != std::string::npos;
    float fontSize = static_cast<float>(atof(fontSizeStr.c_str()));
    // SE_LOGD("CanvasRenderingContext2D::setfont: %s, Size: %f, isBold: %b\n", fontName.c_str(), fontSize, isBold);
    return {fontName, fontSize, isBold, isItalic, false, false};
#elif CC_PLATFORM == CC_PLATFORM_ANDROID || CC_PLATFORM == CC_PLATFORM_OHOS || CC_PLATFORM == CC_PLATFORM_OPENHARMONY
    std::string fontName = "sans-serif";
    std::string fontSizeStr = "30";
    std::regex re(R"(\s*((\d+)([\.]\d+)?)px\s+([^\r\n]*))");
    std::match_results<std::string::const_iterator> results;
    if (std::regex_search(font.cbegin(), font.cend(), results, re)) {
        fontSizeStr = results[2].str();
        // support get font name from `60px American` or `60px "American abc-abc_abc"`
        // support get font name contain space,example `times new roman`
        // if regex rule that does not conform to the rules,such as Chinese,it defaults to sans-serif
        std::match_results<std::string::const_iterator> fontResults;
        std::regex fontRe(R"(([\w\s-]+|"[\w\s-]+"$))");
        std::string tmp(results[4].str());
        if (std::regex_match(tmp, fontResults, fontRe)) {
            fontName = results[4].str();
        }
    }

    double fontSize = atof(fontSizeStr.c_str());
    bool isBold = font.find("bold", 0) != std::string::npos || font.find("Bold", 0) != std::string::npos;
    bool isItalic = font.find("italic", 0) != std::string::npos || font.find("Italic", 0) != std::string::npos;
    bool isSmallCaps = font.find("small-caps", 0) != std::string::npos || font.find("Small-Caps") != std::string::npos;
    bool isOblique = font.find("oblique", 0) != std::string::npos || font.find("Oblique", 0) != std::string::npos;
    // font-style: italic, oblique, normal
    // font-weight: normal, bold
    // font-variant: normal, small-caps
    return {fontName, static_cast<float>(fontSize), isBold, isItalic, isOblique, isSmallCaps};
#elif CC_PLATFORM == CC_PLATFORM_MACOS || CC_PLATFORM == CC_PLATFORM_IOS

    std::string boldStr;
    std::string fontName = "Arial";
    std::string fontSizeStr = "30";
    bool isItalic = font.find("italic", 0) != std::string::npos || font.find("Italic", 0) != std::string::npos;

    // support get font name from `60px American` or `60px "American abc-abc_abc"`
    std::regex re("(bold)?\\s*((\\d+)([\\.]\\d+)?)px\\s+([\\w-]+|\"[\\w -]+\"$)");
    std::match_results<std::string::const_iterator> results;
    if (std::regex_search(font.cbegin(), font.cend(), results, re)) {
        boldStr = results[1].str();
        fontSizeStr = results[2].str();
        fontName = results[5].str();
    }
    float fontSize = atof(fontSizeStr.c_str());
    bool isBold = !boldStr.empty() || font.find("bold", 0) != std::string::npos || font.find("Bold", 0) != std::string::npos;
    return {fontName, static_cast<float>(fontSize), isBold, isItalic, false, false};
#endif
    return {};
}

} // namespace cc::detail
