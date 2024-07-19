#include "TRT_Engine.h"

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