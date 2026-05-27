#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <vector>
#include "tl/expected.hpp"
#include <system_error>
#include <errno.h>

#define CHECK(expected) \
    do { \
        auto ex ## __LINE__ = expected; \
        if (!ex ## __LINE__.has_value()) { \
            return tl::unexpected(ex ## __LINE__.error()); \
        } \
    } while(0); \

#define AUTO(var, expected) \
    do { \
        auto ex##__FILE__##__LINE__; \
        if (!ex.has_value()) { \
            return tl::unexpected(ex.error()); \
        } \
        else {\
            var = ex.value(); \
        }\
    } while(0); \


namespace sws {

}

#endif