#define NAPI_VERSION 6 
#include <node_api.h>
#include <immintrin.h>
#ifdef _WIN32
  #include <intrin.h>
#else
  #include <cpuid.h>  
#include <stdint.h>
#include <string.h>

// RDRAND: Leaf 1, ECX bit 30
// RDSEED: Leaf 7, EBX bit 18

int check_cpu_support(uint32_t leaf, uint32_t subleaf, int reg_idx, int bit) {
    uint32_t regs[4]; // eax, ebx, ecx, edx

    #ifdef _WIN32
        __cpuidex((int*)regs, leaf, subleaf);
    #else
        __get_cpuid_count(leaf, subleaf, &regs[0], &regs[1], &regs[2], &regs[3]);
    #endif

    return (regs[reg_idx] & (1 << bit));
}

napi_value Generate(napi_env env, napi_callback_info info, int use_seed) {
    unsigned long long val = 0;
    int success = 0;
    int retry = 10;

    if (use_seed) {
        if (!check_cpu_support(7, 0, 1, 18)) {
            napi_throw_error(env, NULL, "CPU doesn't support RDSEED.");
            return NULL;
        }
    } else {
        if (!check_cpu_support(1, 0, 2, 30)) {
            napi_throw_error(env, NULL, "CPU doesn't support RDRAND.");
            return NULL;
        }
    }

    while (retry-- > 0 && !success) {
        if (use_seed) {
            success = _rdseed64_step(&val);
        } else {
            success = _rdrand64_step(&val);
        }
    }

    if (!success) {
        napi_throw_error(env, NULL, "Hardware entropy is unavailable, please try again.");
        return NULL;
    }

    napi_value result;
    napi_status status = napi_create_bigint_uint64(env, val, &result);
    if (status != napi_ok) return NULL;

    return result;
}

napi_value GetRdrand(napi_env env, napi_callback_info info) {
    return Generate(env, info, 0);
}

napi_value GetRdseed(napi_env env, napi_callback_info info) {
    return Generate(env, info, 1);
}

napi_value FillBuffer(napi_env env, napi_callback_info info) {
    size_t argc = 1;
    napi_value args[1];
    void* data;
    size_t length;

    napi_get_cb_info(env, info, &argc, args, NULL, NULL);
    napi_get_typedarray_info(env, args[0], NULL, &length, &data, NULL, NULL);

    uint8_t* buffer = (uint8_t*)data;
    unsigned long long temp_rng;

    for (size_t i = 0; i < length; i += 8) {
        if (_rdrand64_step(&temp_rng)) {
            size_t chunk = (length - i < 8) ? (length - i) : 8;
            memcpy(buffer + i, &temp_rng, chunk);
        }
    }

    return NULL;
}

napi_value Init(napi_env env, napi_value exports) {
    napi_value fn_rdrand, fn_rdseed, fn_fill_buffer;

    napi_create_function(env, NULL, 0, GetRdrand, NULL, &fn_rdrand);
    napi_set_named_property(env, exports, "getRdrand", fn_rdrand);

    napi_create_function(env, NULL, 0, GetRdseed, NULL, &fn_rdseed);
    napi_set_named_property(env, exports, "getRdseed", fn_rdseed);

    napi_create_function(env, NULL, 0, FillBuffer, NULL, &fn_fill_buffer);
    napi_set_named_property(env, exports, "fillBuffer", fn_fill_buffer);

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)