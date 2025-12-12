#include <CachedString.h>
#include <fstream>
#include <iostream>
#include <ostream>
using namespace std;
CachedString::CachedString(const char *path, const char *default_string) {
    this->path = path;

    ifstream test(path);
    if (!test) {
        ofstream c(path, std::ios::binary);
        saveable = false;
        if (c) {
            saveable = true;
        } else {
            saveable = true;
        }
        set(default_string);
    } else {
        saveable = true;
        reload();
    }
}
std::string CachedString::get() const { return str; }

bool CachedString::reload() {
    if (!saveable)
        return false;
    ifstream file(path);
    if (!file)
        return false;
    char buffer[128];
    std::string new_string;
    while (file.read(buffer, 128) || file.gcount() > 0) {
        new_string.append(buffer, file.gcount());
    }
    str = new_string;
    return true;
}

void CachedString::set(std::string str) {
    this->str = str;
    if (!saveable)
        return;

    ofstream out(path, std::ios::binary);
    if (out) {
        out.write(str.data(), str.size() + 1);
    }
}