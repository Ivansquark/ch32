#include "my_http.h"

Http::Http() { init(); }

Http::ParseState Http::parse(const uint8_t* data, uint16_t len) {
    if (data[0] == 'G' && data[1] == 'E' && data[2] == 'T') {
        if (data[4] == '/' && data[5] == 0x20) {
            // GET /
            return ParseState::GET_HTML;
        } else if (data[4] == '/' && data[5] == 'f') {
            // GET /facicon.ico
            return ParseState::GET_ICO;
        } else if (data[4] == '/' && data[5] == 's' && data[6] == 't') {
            // GET /style.css
            return ParseState::GET_CSS;
        } else if (data[4] == '/' && data[5] == 'j' && data[6] == 'c') {
            // GET /script.js
            return ParseState::GET_JS;
        }
    } else {
        return ParseState::NOT;
    }
    return ParseState::NOT;
}

void Http::init() {}
