#include "TRT_Engine.h"

static uint32_t fontBackgroundColor = 0;

static Image *symbols[FONT_MAX_SYMBOLS];
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

void TRT_text_loadFont(char *directory) {
    if (!allSymbolsLen)
        allSymbolsLen = MIN(FONT_MAX_SYMBOLS, strlen(allSymbols));

    for (int i = 0; i < allSymbolsLen; ++i) {
        char path[512];
        char *filename = symbolToFilename(allSymbols[i]);

        sprintf(path, "%s%s", directory, filename);

        symbols[i] = TRT_image_get(path);
        free(filename);
    }
}

LineData *TRT_text_size(char *text, uint32_t *nLines, uint32_t textHeight) {
    struct LineData *result = calloc(FONT_MAX_LINES, sizeof(struct LineData));
    uint32_t currentLine = 1;

    result[0].lineFontHeightRatio = 3.402823466e+38F; // FLOAT_MAX_VALUE

    for (uint32_t i = 0; i < strlen(text); ++i) {
        if (text[i] == '\n') {
            if (result[0].width + result[0].nLetters + result[0].nSpaces < result[currentLine].width + result[currentLine].nLetters + result[currentLine].nSpaces) {
                result[0].width = result[currentLine].width;
                result[0].nLetters = result[currentLine].nLetters;
                result[0].nSpaces = result[currentLine].nSpaces;
            }

            result[0].height += result[currentLine].height;
            result[0].lineFontHeightRatio = MAX(result[0].lineFontHeightRatio, result[currentLine].lineFontHeightRatio);

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
        result[currentLine].lineFontHeightRatio = (float) textHeight / (float) symbol->height;
    }

    if (result[0].width + result[0].nLetters + result[0].nSpaces < result[currentLine].width + result[currentLine].nLetters + result[currentLine].nSpaces) {
        result[0].width = result[currentLine].width;
        result[0].nLetters = result[currentLine].nLetters;
        result[0].nSpaces = result[currentLine].nSpaces;
    }

    result[0].height += result[currentLine].height;
    result[0].lineFontHeightRatio = MIN(result[0].lineFontHeightRatio, result[currentLine].lineFontHeightRatio);

    for (uint32_t i = 1; i <= currentLine; ++i) {
        result[i].width = (uint32_t) ((float) result[i].width * result[0].lineFontHeightRatio);
        result[i].height = (uint32_t) ((float) result[i].height * result[0].lineFontHeightRatio);
    }

    result[0].width = (uint32_t) ((float) result[0].width * result[0].lineFontHeightRatio);
    result[0].height = (uint32_t) ((float) result[0].height * result[0].lineFontHeightRatio);

    *nLines = currentLine;
    return result;
}

void TRT_text_draw(char *text, Vec2 position, uint32_t height, uint32_t color, ElementAlignment horizontalAlignment, ElementAlignment verticalAlignment, TextAlignment textAlignment) {
    uint32_t nLines;
    struct LineData *elementSize = TRT_text_size(text, &nLines, height);
    Vec2 windowSize = TRT_window_getSize();

    elementSize[0].width = (uint32_t) ((float) elementSize[0].width + elementSize[0].nSpaces * spaceWidth + (elementSize[0].nLetters - 1) * letterSpacing);
    elementSize[0].height = (uint32_t) ((float) elementSize[0].height + (nLines - 1) * lineOffset);

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

            position.y -= (int32_t) (height + lineOffset);

            switch (textAlignment) {
                case TEXT_ALIGN_LEFT:
                    xOffsetFromLeft = 0;
                    break;
                case TEXT_ALIGN_CENTER:
                    xOffsetFromLeft = (elementSize[0].width - elementSize[currentLine].width -
                                       elementSize[currentLine].nSpaces * spaceWidth -
                                       (elementSize[currentLine].nLetters - 1) * letterSpacing) / 2;
                    break;
                case TEXT_ALIGN_RIGHT:
                    xOffsetFromLeft = (elementSize[0].width - elementSize[currentLine].width -
                                       elementSize[currentLine].nSpaces * spaceWidth -
                                       (elementSize[currentLine].nLetters - 1) * letterSpacing);
                    break;
                default:
                    break;
            }

            if (letter == '\n')
                continue;
        }

        if (letter == ' ') {
            xOffsetFromLeft += spaceWidth;
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

                TRT_window_setPixel(position.x + actualX + xOffsetFromLeft, position.y + actualY - yOffsetFromTop,
                                    color);
            }
        }

        xOffsetFromLeft += symbol->width * elementSize[0].lineFontHeightRatio + letterSpacing;
    }

    free(elementSize);
}

void TRT_text_setBackgroundColor(uint32_t color) {
    fontBackgroundColor = color;
}

void TRT_text_setSpaceWidth(uint32_t width) {
    spaceWidth = width;
}

void TRT_text_setLineOffset(uint32_t offset) {
    lineOffset = offset;
}

void TRT_text_setLetterSpacing(uint32_t spacing) {
    letterSpacing = spacing;
}