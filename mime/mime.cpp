#include "mime.h"
#include <magic.h>
#include <string>

std::string mime::lookup(const std::string &str) {
    const char *mime = nullptr;
    magic_t cookie;
    cookie = magic_open(MAGIC_MIME_TYPE);
    magic_load(cookie, NULL);
    mime = magic_file(cookie, str.c_str());

    if (mime == nullptr) {
        mime = "";
    }
    std::string ret = std::string(mime);
    magic_close(cookie);
    return ret;
}
