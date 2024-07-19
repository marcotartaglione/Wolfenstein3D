#include "TRT_Engine.h"

static int32_t fontBackgroundColor = -1;

static Image* symbols[MAX_SYMBOLS];
char* allSymbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+-=[]{};':,.<>?/|\\\"";
static uint32_t allSymbolsLen = 0;

int32_t symbolIndex(char symbol) {
    for (int i = 0; i < allSymbolsLen; ++i) {
        if (allSymbols[i] == symbol)
            return i;
    }

    return -1;
}

char* symbolToFilename(char symbol) {
    char* filename = malloc(64);

    if (symbol >= 'A' && symbol <= 'Z') {
        sprintf(filename, "upper_%c.png", symbol + 32);
        return filename;
    }
    if (symbol >= 'a' && symbol <= 'z') {
        sprintf(filename, "lower_%c.png", symbol);
        return filename;
    }
    if (symbol >= '0' && symbol <= '9') {
        sprintf(filename, "number_%c.png", symbol);
        return filename;
    }

    switch (symbol) {
        case '!':
            sprintf(filename, "exclamation_mark.png");
            break;
        case '@':
            sprintf(filename, "at.png");
            break;
        case '#':
            sprintf(filename, "hash.png");
            break;
        case '$':
            sprintf(filename, "dollar.png");
            break;
        case '%':
            sprintf(filename, "percent.png");
            break;
        case '^':
            sprintf(filename, "caret.png");
            break;
        case '&':
            sprintf(filename, "and.png");
            break;
        case '*':
            sprintf(filename, "asterisk.png");
            break;
        case '(':
            sprintf(filename, "round_bracket_open.png");
            break;
        case ')':
            sprintf(filename, "round_bracket_close.png");
            break;
        case '_':
            sprintf(filename, "underscore.png");
            break;
        case '+':
            sprintf(filename, "plus.png");
            break;
        case '-':
            sprintf(filename, "minus.png");
            break;
        case '=':
            sprintf(filename, "equal.png");
            break;
        case '[':
            sprintf(filename, "square_bracket_open.png");
            break;
        case ']':
            sprintf(filename, "square_bracket_close.png");
            break;
        case '{':
            sprintf(filename, "curly_bracket_open.png");
            break;
        case '}':
            sprintf(filename, "curly_bracket_close.png");
            break;
        case ';':
            sprintf(filename, "semicolon.png");
            break;
        case '\'':
            sprintf(filename, "apostrophe.png");
            break;
        case ':':
            sprintf(filename, "colon.png");
            break;
        case ',':
            sprintf(filename, "comma.png");
            break;
        case '.':
            sprintf(filename, "dot.png");
            break;
        case '<':
            sprintf(filename, "minor.png");
            break;
        case '>':
            sprintf(filename, "major.png");
            break;
        default:
        case '?':
            sprintf(filename, "question_mark.png");
            break;
        case '/':
            sprintf(filename, "slash.png");
            break;
        case '|':
            sprintf(filename, "pipe.png");
            break;
        case '\\':
            sprintf(filename, "backslash.png");
            break;
        case '\"':
            sprintf(filename, "quote.png");
            break;
    }

    return filename;
}

void loadSymbols(char* directory) {
    if (!allSymbolsLen)
        allSymbolsLen = MIN(MAX_SYMBOLS, strlen(allSymbols));

    for (int bold = 0; bold <= 1; ++bold) {
        for (int i = 0; i < allSymbolsLen; ++i) {
            char path[512];
            char* filename = symbolToFilename(allSymbols[i]);

            if (bold)
                sprintf(path, "%sbold_%s", directory, filename);
            else
                sprintf(path, "%s%s", directory, filename);


            symbols[i + allSymbolsLen * bold] = getImage(path);
            free(filename);
        }
    }
}

Image** phraseToImages(char* phrase, uint8_t bold) {
    uint32_t len = strlen(phrase);
    Image** images = malloc(len * sizeof(Image*));

    static int32_t notFoundIndex = -1;
    if (notFoundIndex == -1)
        notFoundIndex = symbolIndex('?');

    for (uint32_t i = 0; i < len; ++i) {
        int32_t index = symbolIndex(phrase[i]);
        if (index == -1)
            index = notFoundIndex;

        images[i] = symbols[index + allSymbolsLen * bold];
    }

    return images;
}

void drawText(char* text, Vec2 position, uint32_t height, uint32_t color, uint8_t bold, TextHorizontalAlignment horizontalAlignment) {
    uint32_t offsetFromStart = 0;
    uint32_t pixelOffsetFromBorder = 0;

    if (horizontalAlignment != TEXT_H_ALIGN_NONE) {
        for (uint32_t i = 0; i < strlen(text); ++i) {
            pixelOffsetFromBorder += symbols[symbolIndex(text[i]) + allSymbolsLen * bold]->width;
        }
    }

    for (uint32_t i = 0; i < strlen(text); ++i) {
        Image* symbol = symbols[symbolIndex(text[i]) + allSymbolsLen * bold];

        float fontHeightRatio = (float) height / (float) symbol->height;
        for (uint32_t x = 0; x < symbol->width; ++x) {
            for (uint32_t y = 0; y < symbol->height; ++y) {
                uint32_t textureIndex = (y * symbol->width + x) * 3;

                uint32_t r = symbol->data[textureIndex + 0] << 16;
                uint32_t g = symbol->data[textureIndex + 1] << 8;
                uint32_t b = symbol->data[textureIndex + 2];

                uint32_t pixelX;
                uint32_t pixelY = (position.y + y * fontHeightRatio);

                switch (horizontalAlignment) {
                    case TEXT_H_ALIGN_LEFT:
                        pixelX = offsetFromStart + x * fontHeightRatio;
                        break;
                    case TEXT_H_ALIGN_CENTER:
                        pixelX = getWindowSize().x / 2 - pixelOffsetFromBorder / 2 + offsetFromStart + x * fontHeightRatio;
                        break;
                    case TEXT_H_ALIGN_RIGHT:
                        pixelX = getWindowSize().x - pixelOffsetFromBorder + offsetFromStart + x * fontHeightRatio;
                        break;
                    default:
                    case TEXT_H_ALIGN_NONE:
                        pixelX = position.x + offsetFromStart + x * fontHeightRatio;
                        break;
                }

                if (fontBackgroundColor == -1) {
                    setWindowPixel(pixelX, pixelY, r | g | b);
                    continue;
                }

                if ((r | g | b) == fontBackgroundColor)
                    continue;

                setWindowPixel(pixelX, pixelY, color);
            }
        }
        offsetFromStart += symbol->width;
    }
}

void setFontBackgroundColor(uint32_t color) {
    fontBackgroundColor = color;
}