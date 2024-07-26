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
        sprintf(filename, "upper_%c.png", symbol);
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

Vec2 *TRT_text_size(char *text, uint32_t *nLines, uint32_t textHeight, uint32_t spaceWidth, uint32_t letterSpacing, uint32_t lineOffset) {
    Vec2 *result = calloc(FONT_MAX_LINES, sizeof(Vec2));
    uint32_t currentLine = 1;

    uint32_t textLen = strlen(text);

    for (uint32_t i = 0; i <= textLen; ++i) {
        if (text[i] == '\n' || i == textLen) {
            result[0].x = MAX(result[0].x, result[currentLine].x);
            result[0].y += (int32_t)textHeight + (int32_t)lineOffset;

            if (nLines != NULL)
                *nLines = currentLine;

            currentLine++;
            continue;
        }

        if (text[i] == ' ' || text[i] == '\t') {
            result[currentLine].x += (int32_t)spaceWidth;
            continue;
        }

        Image *currentSymbol = symbols[symbolIndex(text[i])];
        if (currentSymbol == NULL)
            continue;

        float letterHeightRatio = (float) textHeight / (float) currentSymbol->height;
        result[currentLine].x += (int32_t)((float) currentSymbol->width * letterHeightRatio) + (int32_t)letterSpacing;
        result[currentLine].y = MAX(result[currentLine].y, currentSymbol->height * letterHeightRatio);
    }

    return result;
}

void TRT_text_draw(char *text, Vec2 position, uint32_t height, uint32_t color, TextAlignment textAlignment) {
    uint32_t nLines;
    Vec2 *elementSize = TRT_text_size(text, &nLines, height, spaceWidth, letterSpacing, lineOffset);

    bool isYCentered = position.y == ELEMENT_ALIGN_CENTER;
    TRT_window_interpretatePosition(&position,
                                    (Vec2){elementSize[0].x, elementSize[0].y},
                                    false);

    if (isYCentered)
        position.y += elementSize[0].y;

    uint32_t currentLine = 0;
    uint32_t xOffsetFromLeft = 0;

    for (uint32_t currentLetterIndex = 0; currentLetterIndex < strlen(text); ++currentLetterIndex) {
        char letter = text[currentLetterIndex];

        if (currentLetterIndex == 0 || letter == '\n') {
            currentLine++;

            switch (textAlignment) {
                case TEXT_ALIGN_LEFT:
                    xOffsetFromLeft = 0;
                    break;
                case TEXT_ALIGN_CENTER:
                    xOffsetFromLeft = (elementSize[0].x - elementSize[currentLine].x) / 2;
                    break;
                case TEXT_ALIGN_RIGHT:
                    xOffsetFromLeft = (elementSize[0].x - elementSize[currentLine].x);
                    break;
                default:
                    break;
            }

            if (letter == '\n') {
                position.y -= (int32_t) (height + lineOffset);
                continue;
            }
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
                yOffsetFromTop = (uint32_t)(4.0f * letterHeightRatio);
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

                TRT_window_setPixel(position.x + actualX + xOffsetFromLeft, position.y + actualY - yOffsetFromTop - height, color);
            }
        }

        xOffsetFromLeft += (uint32_t)((float)symbol->width * letterHeightRatio) + letterSpacing;
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