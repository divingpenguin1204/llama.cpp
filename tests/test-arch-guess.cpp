#include "../src/llama-arch.h"

#include <cstdio>
#include <string>

struct test_case {
    const char * name;
    llm_arch     expected;
};

int main() {
    const test_case cases[] = {
        // hyphenated VL names used to match the shorter prefix "qwen2"
        { "Qwen2-VL-2B-Instruct-bartowski-Q4AM_PB32.adla", LLM_ARCH_QWEN2VL },
        { "Qwen2-VL-2B-Instruct",                          LLM_ARCH_QWEN2VL },
        { "qwen2-vl",                                      LLM_ARCH_QWEN2VL },
        { "qwen2_vl",                                      LLM_ARCH_QWEN2VL },
        { "qwen2vl",                                       LLM_ARCH_QWEN2VL },
        { "Qwen2.5-VL-7B-Instruct",                        LLM_ARCH_QWEN2VL },

        { "Qwen2-7B-Instruct",                             LLM_ARCH_QWEN2  },
        { "Qwen2.5-7B-Instruct",                           LLM_ARCH_QWEN2  },
        { "qwen2",                                         LLM_ARCH_QWEN2  },

        { "Qwen3-VL-8B",                                   LLM_ARCH_QWEN3VL },
        { "Qwen3.5-9B",                                    LLM_ARCH_QWEN35  },

        { "Llama-3-8B",                                    LLM_ARCH_LLAMA  },
        { "Llama-4-Scout",                                 LLM_ARCH_LLAMA4 },

        { "",                                              LLM_ARCH_UNKNOWN },
        { "not-a-real-model",                              LLM_ARCH_UNKNOWN },
    };

    int n_fail = 0;
    for (const test_case & tc : cases) {
        const llm_arch got = llm_arch_guess_from_name(tc.name);
        if (got != tc.expected) {
            fprintf(stderr, "FAIL: '%s' -> %s (expected %s)\n",
                    tc.name, llm_arch_name(got), llm_arch_name(tc.expected));
            n_fail++;
        }
    }

    if (n_fail) {
        fprintf(stderr, "%d test(s) failed\n", n_fail);
        return 1;
    }

    printf("ok\n");
    return 0;
}
