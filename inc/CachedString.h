#include <string>
class CachedString {
    const char *path;
    std::string str;
    bool saveable;

public:
    CachedString(const char *path, const char *default_string);
    std::string get() const;
    void set(std::string str);
    bool reload();
};