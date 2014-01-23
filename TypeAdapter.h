#pragma once
#include <vector>
#include <stdint.h>
#include "RenderContextImpl.h"

#pragma warning (push)
#pragma warning (disable : 4018)

void adapters_init();

template <typename T0, typename T1>
class OneWayAdapter {
    friend void adapters_init();
public:
    static T1  To(T0  t0) {
        adapters_init();
        return g_t1[t0];
    }

private:
    static std::vector<T1>  g_t1;
    static OneWayAdapter*   g_instance;

    OneWayAdapter() {}

    static void add(T0 t0, T1 t1) {
        if (t0 + 1 > g_t1.size()) g_t1.resize(t0 + 1);
        g_t1[t0] = t1;
    }
};

template <typename T0, typename T1>
std::vector<T1> OneWayAdapter<T0, T1>::g_t1;

template <typename T0, typename T1>
class TwoWayAdapter {
    friend void adapters_init();
public:
    static T0  To(T1 t1) {
        adapters_init();
        return g_t0[t1];
    }

    static T1  To(T0 t0) {
        adapters_init();
        return g_t1[t0];
    }

private:
    static std::vector<T0>  g_t0;
    static std::vector<T1>  g_t1;

    TwoWayAdapter() {}

    static void add(T0 t0, T1 t1) {
        if (t0 + 1 > g_t1.size()) g_t1.resize(t0 + 1);
        if (t1 + 1 > g_t0.size()) g_t0.resize(t1 + 1);
        g_t0[t1] = t0;
        g_t1[t0] = t1;
    }
};

template <typename T0, typename T1>
std::vector<T0> TwoWayAdapter<T0, T1>::g_t0;
template <typename T0, typename T1>
std::vector<T1> TwoWayAdapter<T0, T1>::g_t1;

#ifdef _WIN32
typedef TwoWayAdapter<TEX_FORMAT, D3DFORMAT> adapter_dx_fmt;
#endif //_WIN32
typedef OneWayAdapter<TEX_FORMAT, uint32_t> adapter_fmt_size;

void adapters_init() {
    static bool once = true;
    if (!once) return;
    // Formats
#ifdef _WIN32
    adapter_dx_fmt::add(TEX_FORMAT_A8R8G8B8, D3DFMT_A8R8G8B8);
    adapter_dx_fmt::add(TEX_FORMAT_X8R8G8B8, D3DFMT_X8R8G8B8);
    adapter_dx_fmt::add(TEX_FORMAT_A8, D3DFMT_A8);
#endif //_WIN32
    // Pixel sizes
    adapter_fmt_size::add(TEX_FORMAT_A8R8G8B8, 32);
    adapter_fmt_size::add(TEX_FORMAT_X8R8G8B8, 32);
    adapter_fmt_size::add(TEX_FORMAT_A8, 8);
    // Texture creation flags
    once = false;    
}

#pragma warning (pop)