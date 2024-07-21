#include "TRT_Engine.h"

static uint32_t fontBackgroundColor = 0;

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

void TRT_loadSymbols(char *directory) {
    if (!allSymbolsLen)
        allSymbolsLen = MIN(MAX_SYMBOLS, strlen(allSymbols));

    for (int i = 0; i < allSymbolsLen; ++i) {
        char path[512];
        char *filename = symbolToFilename(allSymbols[i]);

        sprintf(path, "%s%s", directory, filename);

        symbols[i] = TRT_getImage(path);
        free(filename);
    }
}

//
// First element of the array is the total size, the rest are the sizes of each line in order
//
Vec2 *textSize(char *text, uint32_t *nLines) {
    Vec2 *result = calloc(FONT_MAX_LINES, sizeof(Vec2));
    uint32_t currentLine = 1;

    for (uint32_t i = 0; i < strlen(text); ++i) {
        if (text[i] == '\n') {
            result[0].x = MAX(result[0].x, result[currentLine].x);
            result[0].y += result[currentLine].y + FONT_LINE_OFFSET_MIN;

            currentLine++;
            continue;
        }

        if (text[i] == ' ' || text[i] == '\t') {
            result[currentLine].x += FONT_SPACE_MIN_WIDTH;
            continue;
        }

        Image *symbol = symbols[symbolIndex(text[i])];
        result[currentLine].x += symbol->width + FONT_LETTER_SPACING;
        result[currentLine].y = MAX(result[currentLine].y, symbol->height);
    }

    result[0].x = MAX(result[0].x, result[currentLine].x);
    result[0].y += result[currentLine].y + FONT_LINE_OFFSET_MIN;

    *nLines = currentLine;
    return result;
}

//
// Calculates the font height ratio based on all the lines. The first element of the array is the total size, the rest are the sizes of each line in order
//
float getFontHeightRatio(Vec2 lines[], uint32_t nLines, uint32_t height) {
    float heightRatio = -1;
    for (int i = 1; i <= nLines; ++i) {
        float currentRatio = (float) height / (float) lines[i].y;
        if (heightRatio == -1 || currentRatio < heightRatio)
            heightRatio = currentRatio;
    }

    return heightRatio;
}

void TRT_drawText(char *text, Vec2 position, uint32_t height, uint32_t color, TextAlignment horizontalAlignment,
                  TextAlignment verticalAlignment) {
    uint32_t lastSymbolWidth = -1;
    uint32_t offsetWidthFromStart = 0;

    uint32_t lineNumber = 1;

    uint32_t totalLines;
    Vec2 *linesSizes = textSize(text, &totalLines);
    float fontHeightRatio = getFontHeightRatio(linesSizes, totalLines, height);

    switch (verticalAlignment) {
        case TEXT_ALIGN_START:
            position.y = 0;
            break;
        case TEXT_ALIGN_CENTER:
            position.y = (int32_t) ((float) TRT_getWindowSize().y / 2 + (float) linesSizes[0].y * fontHeightRatio / 2);
            break;
        case TEXT_ALIGN_END:
            position.y = (int32_t) ((float) TRT_getWindowSize().y + (float) linesSizes[0].y * fontHeightRatio);
            break;
        default:
        case TEXT_ALIGN_NONE:
            break;
    }

    for (uint32_t i = 0; i < strlen(text); ++i) {
        if (lineNumber > FONT_MAX_LINES)
            break;

        if (i == 0 || text[i - 1] == '\n') {
            switch (horizontalAlignment) {
                case TEXT_ALIGN_START:
                    position.x = 0;
                    break;
                case TEXT_ALIGN_CENTER:
                    position.x = (int32_t) ((float) TRT_getWindowSize().x / 2 - (float) linesSizes[lineNumber].x * fontHeightRatio / 2);
                    break;
                case TEXT_ALIGN_END:
                    position.x = (int32_t) ((float) TRT_getWindowSize().x - (float) linesSizes[lineNumber].x * fontHeightRatio);
                    break;
                default:
                case TEXT_ALIGN_NONE:
                    break;
            }
        }

        if (text[i] == '\n') {
            lineNumber++;
            lastSymbolWidth = -1;
            offsetWidthFromStart = 0;
            position.y -= (int32_t) (height + FONT_LINE_OFFSET_MIN);
            continue;
        } else if (text[i] == ' ') {
            if (lastSymbolWidth == -1)
                continue;

            position.x += (int32_t) (MAX(FONT_SPACE_MIN_WIDTH, (int32_t) lastSymbolWidth) * fontHeightRatio);
            continue;
        } else {
            offsetWidthFromStart += FONT_LETTER_SPACING;
        }

        Image *symbol = symbols[symbolIndex(text[i])];

        if (symbol == NULL)
            continue;

        lastSymbolWidth = (uint32_t) ((float) symbol->width * fontHeightRatio);

        for (uint32_t textureX = 0; textureX < symbol->width; textureX++) {
            for (uint32_t textureY = 0; textureY < symbol->height; textureY++) {
                uint32_t actualX = (uint32_t) ((float) textureX * fontHeightRatio);
                uint32_t actualY = (uint32_t) ((float) textureY * fontHeightRatio);

                uint32_t textureIndex = (textureY * symbol->width + textureX) * 3;

                uint32_t r = symbol->data[textureIndex + 0] << 16;
                uint32_t g = symbol->data[textureIndex + 1] << 8;
                uint32_t b = symbol->data[textureIndex + 2];

                if (fontBackgroundColor != -1 && (r | g | b) == fontBackgroundColor)
                    continue;

                TRT_setWindowPixel(position.x + actualX + offsetWidthFromStart, position.y + actualY, color);
            }
        }

        offsetWidthFromStart += lastSymbolWidth;
    }

    free(linesSizes);
}

void TRT_setFontBackgroundColor(uint32_t color) {
    fontBackgroundColor = color;
}