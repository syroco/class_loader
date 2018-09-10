#ifndef LIBDYLIBXX_H
#define LIBDYLIBXX_H

#include <string>

#include "libdylib.h"

namespace libdylib {
    class dylib {
    protected:
        dylib_ref handle;
    public:
        dylib(const char *path = NULL, bool locate = false);
        ~dylib();

        bool open(const char *path, bool locate = false);
        inline bool open(std::string path, bool locate = false) { return open(path.c_str(), locate); }
        bool open_list(const char *path, ...);
        bool open_locate(const char *name);
        bool close();

        void *lookup(const char *symbol);
        inline void *lookup(std::string symbol) { return lookup(symbol.c_str()); }
        bool find(const char *symbol);
        inline bool find(std::string symbol) { return find(symbol.c_str()); }

        // Note that these functions use the 'handle' member internally regardless
        // of the first argument passed here (which is needed to pass through
        // all remaining arguments properly)
        bool find_any(dylib_ref unused, ...);
        bool find_all(dylib_ref unused, ...);

        template<typename T>
        bool bind(const char *symbol, T* &dest) {
            dest = (T*)lookup(symbol);
            return dest != NULL;
        }
        template<typename T>
        bool bind(std::string symbol, T* &dest) {
            return bind<T>(symbol.c_str(), dest);
        }
        #define DYLIB_BINDNAME(lib, name) lib.bind(#name, name)

        inline dylib_ref &get_handle() { return handle; }
        inline const char *get_path() { return LIBDYLIB_NAME(get_path)(handle); }
        inline const void *get_raw_handle() { return LIBDYLIB_NAME(get_handle)(handle); }
        inline bool is_open() { return handle != NULL; }
    };
}

#endif /* LIBDYLIBXX_H */
