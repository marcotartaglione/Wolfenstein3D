#include "TRT_Engine.h"

static int32_t fontBackgroundColor = -1;

static Image *symbols[MAX_SYMBOLS];
char *allSymbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789!@#$%^&*()_+-=[]{};':,.<>?/|\\\"";
static uint32_t allSymbolsLen = 0;

int32_t symbolIndex(char symbol) {
    for (int i = 0; i < allSymbolsLen; ++i) {
        if (allSymbols[i] == symbol)
            return i;
    }

    return -1;
}

char *symbolToFilename(char symbol) {
    char *filename = malloc(64);

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

void loadSymbols(char *directory) {
    if (!allSymbolsLen)
        allSymbolsLen = MIN(MAX_SYMBOLS, strlen(allSymbols));

    for (int i = 0; i < allSymbolsLen; ++i) {
        char path[512];
        char *filename = symbolToFilename(allSymbols[i]);

        sprintf(path, "%s%s", directory, filename);


        symbols[i] = getImage(path);
        free(filename);
    }
}

void drawText(char *text, Vec2 position, uint32_t height, uint32_t color, TextAlignment horizontalAlignment, TextAlignment verticalAlignment) {
    uint32_t pixelOffsetFromSideBorder = 0;
    uint32_t pixelOffsetFromTopBorder = 0;
    uint32_t currentPixelOffsetFromSideBorder = 0;
    uint32_t lastLetterWidth = -1;

    if (horizontalAlignment != TEXT_ALIGN_NONE || verticalAlignment != TEXT_ALIGN_NONE) {
        for (uint32_t i = 0; i < strlen(text); ++i) {
            if (text[i] == ' ') {
                if (lastLetterWidth == -1)
                    continue;

                currentPixelOffsetFromSideBorder += lastLetterWidth;
            } else if (text[i] == '\n') {
                pixelOffsetFromTopBorder += height + 1;
                pixelOffsetFromSideBorder = MAX(pixelOffsetFromSideBorder, currentPixelOffsetFromSideBorder);
                currentPixelOffsetFromSideBorder = 0;
            } else {
                lastLetterWidth = symbols[symbolIndex(text[i])]->width;
                currentPixelOffsetFromSideBorder += lastLetterWidth;
            }
        }
    }
    pixelOffsetFromSideBorder = MAX(pixelOffsetFromSideBorder, currentPixelOffsetFromSideBorder);

    uint32_t xOffsetFromStart = 0;
    uint32_t yOffsetFromStart = 0;
    lastLetterWidth = -1;

    float fontHeightRatio;

    for (uint32_t i = 0; i < strlen(text); ++i) {
        if (text[i] == '\n') {
            yOffsetFromStart -= height + 1;
            xOffsetFromStart = 0;
            continue;
        }
        if (text[i] == ' ') {
            if (lastLetterWidth == -1)
                continue;

            xOffsetFromStart += 22 * fontHeightRatio;
            continue;
        }

        Image *symbol = symbols[symbolIndex(text[i])];

        fontHeightRatio = (float) height / (float) symbol->height;
        lastLetterWidth = symbol->width;

        for (uint32_t x = 0; x < lastLetterWidth; ++x) {
            for (uint32_t y = 0; y < symbol->height; ++y) {
                uint32_t textureIndex = (y * lastLetterWidth + x) * 3;

                uint32_t r = symbol->data[textureIndex + 0] << 16;
                uint32_t g = symbol->data[textureIndex + 1] << 8;
                uint32_t b = symbol->data[textureIndex + 2];

                uint32_t pixelX;
                uint32_t pixelY;

                switch (horizontalAlignment) {
                    case TEXT_ALIGN_LEFT:
                        pixelX = xOffsetFromStart + x * fontHeightRatio;
                        break;
                    case TEXT_ALIGN_CENTER:
                        pixelX = getWindowSize().x / 2 - pixelOffsetFromSideBorder / 2 + xOffsetFromStart +
                                 x * fontHeightRatio;
                        break;
                    case TEXT_ALIGN_RIGHT:
                        pixelX = getWindowSize().x - pixelOffsetFromSideBorder + xOffsetFromStart + x * fontHeightRatio;
                        break;
                    default:
                    case TEXT_ALIGN_NONE:
                        pixelX = position.x + xOffsetFromStart + x * fontHeightRatio;
                        break;
                }

                switch (verticalAlignment) {
                    case TEXT_ALIGN_LEFT:
                        pixelY = yOffsetFromStart + y * fontHeightRatio;
                        break;
                    case TEXT_ALIGN_CENTER:
                        pixelY = getWindowSize().y / 2 - pixelOffsetFromTopBorder / 2 + yOffsetFromStart +
                                 y * fontHeightRatio;
                        break;
                    case TEXT_ALIGN_RIGHT:
                        pixelY = getWindowSize().y - pixelOffsetFromTopBorder + yOffsetFromStart + y * fontHeightRatio;
                        break;
                    default:
                    case TEXT_ALIGN_NONE:
                        pixelY = position.y + yOffsetFromStart + y * fontHeightRatio;
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
        xOffsetFromStart += symbol->width * fontHeightRatio;
    }
}

void setFontBackgroundColor(uint32_t color) {
    fontBackgroundColor = color;
}