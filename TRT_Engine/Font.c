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

struct LineData {
    uint32_t width;
    uint32_t height;
    uint32_t nLetters;
    uint32_t nSpaces;
};

//
// First element of the array is the total size, the rest are the sizes of each line in order
//
struct LineData *textSize(char *text, uint32_t *nLines) {
    struct LineData *result = calloc(FONT_MAX_LINES, sizeof(struct LineData));
    uint32_t currentLine = 1;

    for (uint32_t i = 0; i < strlen(text); ++i) {
        if (text[i] == '\n') {
            result[0].width = MAX(result[0].width, result[currentLine].width);
            result[0].height += result[currentLine].height;
            result[0].nLetters = MAX(result[0].nLetters, result[currentLine].nLetters);
            result[0].nSpaces = MAX(result[0].nSpaces, result[currentLine].nSpaces);

            currentLine++;
            continue;
        }

        if (text[i] == ' ' || text[i] == '\t') {
            result[currentLine].nSpaces++;
            continue;
        }

        Image *symbol = symbols[symbolIndex(text[i])];

        result[currentLine].width += symbol->width;
        result[currentLine].height = MAX(result[currentLine].height, symbol->height);
        result[currentLine].nLetters++;
    }

    result[0].width = MAX(result[0].width, result[currentLine].width);
    result[0].height += result[currentLine].height;
    result[0].nLetters = MAX(result[0].nLetters, result[currentLine].nLetters);
    result[0].nSpaces = MAX(result[0].nSpaces, result[currentLine].nSpaces);

    *nLines = currentLine;
    return result;
}

//
// Calculates the font height ratio based on all the lines. The first element of the array is the total size, the rest are the sizes of each line in order
//
float getBiggestElementFontHeightRatio(struct LineData lines[], uint32_t nLines, uint32_t height) {
    float heightRatio = -1;
    for (int i = 1; i <= nLines; ++i) {
        float currentRatio = (float) height / (float) lines[i].height;
        if (heightRatio == -1 || currentRatio < heightRatio)
            heightRatio = currentRatio;
    }

    return heightRatio;
}

void
TRT_windowDrawText(char *text, Vec2 position, uint32_t height, uint32_t color, ElementAlignment horizontalAlignment,
                   ElementAlignment verticalAlignment, TextAlignment textAlignment) {
    uint32_t nLines;
    struct LineData *elementSize = textSize(text, &nLines);
    Vec2 windowSize = TRT_getWindowSize();

    float biggestElementFontHeightRatio = getBiggestElementFontHeightRatio(elementSize, nLines, height);

    elementSize[0].width = (uint32_t) ((float) elementSize[0].width * biggestElementFontHeightRatio +
                                       elementSize[0].nSpaces * FONT_SPACE_WIDTH +
                                       (elementSize[0].nLetters - 1) * FONT_LETTER_SPACING);

    elementSize[0].height = (uint32_t) ((float) elementSize[0].height * biggestElementFontHeightRatio +
                                        (nLines - 1) * FONT_LINE_OFFSET_MIN);

    switch (verticalAlignment) {
        case ELEMENT_ALIGN_START:
            position.y = (int32_t) elementSize[0].height;
            break;
        case ELEMENT_ALIGN_CENTER:
            position.y = (int32_t) ((float) windowSize.y + (float) elementSize[0].height) / 2;
            break;
        case ELEMENT_ALIGN_END:
            position.y = (int32_t) ((float) windowSize.y - (float) elementSize[0].height);
            break;
        default:
        case ELEMENT_ALIGN_NONE:
            break;
    }

    switch (horizontalAlignment) {
        case ELEMENT_ALIGN_START:
            position.x = 0;
            break;
        case ELEMENT_ALIGN_CENTER:
            position.x =
                    (int32_t) ((float) windowSize.x - (float) elementSize[0].width) / 2;
            break;
        case ELEMENT_ALIGN_END:
            position.x = (int32_t) ((float) windowSize.x - (float) elementSize[0].width);
            break;
        default:
        case ELEMENT_ALIGN_NONE:
            break;
    }

    uint32_t currentLine = 0;
    uint32_t xOffsetFromLeft = 0;

    for (uint32_t currentLetterIndex = 0; currentLetterIndex < strlen(text); ++currentLetterIndex) {
        char letter = text[currentLetterIndex];

        if (currentLetterIndex == 0 || letter == '\n') {
            currentLine++;

            position.y -= (int32_t) (height + FONT_LINE_OFFSET_MIN);

            switch (textAlignment) {
                case TEXT_ALIGN_LEFT:
                    xOffsetFromLeft = 0;
                    break;
                case TEXT_ALIGN_CENTER:
                    xOffsetFromLeft = (elementSize[0].width - elementSize[currentLine].width * biggestElementFontHeightRatio - elementSize[currentLine].nSpaces * FONT_SPACE_WIDTH - (elementSize[currentLine].nLetters - 1) * FONT_LETTER_SPACING) / 2;
                    break;
                case TEXT_ALIGN_RIGHT:
                    xOffsetFromLeft = (elementSize[0].width - elementSize[currentLine].width * biggestElementFontHeightRatio - elementSize[currentLine].nSpaces * FONT_SPACE_WIDTH - (elementSize[currentLine].nLetters - 1) * FONT_LETTER_SPACING);
                    break;
                default:
                    break;
            }

            if (letter == '\n')
                continue;
        }

        if (letter == ' ') {
            xOffsetFromLeft += FONT_SPACE_WIDTH;
            continue;
        }

        Image *symbol = symbols[symbolIndex(letter)];
        if (symbol == NULL)
            continue;

        float letterHeightRatio = (float) height / (float) symbol->height;

        uint32_t yOffsetFromTop;

        switch (letter) {
            case 'g':
            case 'j':
            case 'p':
            case 'q':
            case 'y':
                yOffsetFromTop = 4 * letterHeightRatio;
                break;
            default:
                yOffsetFromTop = 0;
                break;
        }

        for (uint32_t x = 0; x < symbol->width; ++x) {
            for (uint32_t y = 0; y < symbol->height; ++y) {
                uint32_t actualX = (uint32_t) ((float) x * letterHeightRatio);
                uint32_t actualY = (uint32_t) ((float) y * letterHeightRatio);

                uint32_t textureIndex = (y * symbol->width + x) * 3;

                uint32_t r = symbol->data[textureIndex + 0] << 16;
                uint32_t g = symbol->data[textureIndex + 1] << 8;
                uint32_t b = symbol->data[textureIndex + 2];

                if (fontBackgroundColor != -1 && (r | g | b) == fontBackgroundColor) {
                    continue;
                }

                TRT_setWindowPixel(position.x + actualX + xOffsetFromLeft, position.y + actualY - yOffsetFromTop,
                                   color);
            }
        }

        xOffsetFromLeft += symbol->width * letterHeightRatio + FONT_LETTER_SPACING;
    }

    free(elementSize);
}

void TRT_setFontBackgroundColor(uint32_t color) {
    fontBackgroundColor = color;
}