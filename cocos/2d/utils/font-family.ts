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

interface FontFamilySource {
    readonly useSystemFont: boolean;
    readonly fontFamily: string;
    readonly font: { readonly _nativeAsset: unknown } | null;
}

/** Select the existing font family for both TTF and character rendering. */
export function getCanvasFontFamily (source: FontFamilySource): string {
    let family = 'Arial';
    if (!source.useSystemFont) {
        if (source.font) {
            family = source.font._nativeAsset as string || 'Arial';
        }
    } else {
        family = source.fontFamily || 'Arial';
    }
    return family;
}

/** Build the shared font description without changing family or style semantics. */
export function buildFontDesc (size: number, family: string, bold = false, italic = false): string {
    let description = `${size.toString()}px `;
    description += family;
    if (bold) {
        description = `bold ${description}`;
    }
    if (italic) {
        description = `italic ${description}`;
    }
    return description;
}
