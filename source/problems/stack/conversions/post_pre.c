#include "post_pre.h"
#include "stack.h"
#include "logger.h"
#include "common.h"

typedef struct string {
    char buffer[100];
} string;

void post_pre(const char* post_exp, char* per_exp) {

    stack* stk = stack_create(string);

    for (i32 i = 0; post_exp[i] != '\0'; ++i) {
        if ((post_exp[i] >= 'a' && post_exp[i] <= 'z') || (post_exp[i] >= 'A' && post_exp[i] <= 'Z')) {

            string str = {0};
            str.buffer[0] = post_exp[i];
            stack_push(stk, &str);

        } else if (post_exp[i] == '+' || post_exp[i] == '-' || post_exp[i] == '*' || post_exp[i] == '/' || post_exp[i] == '^') {
            string op2 = *(string*)stack_top(stk);
            stack_pop(stk);

            string op1 = *(string*)stack_top(stk);
            stack_pop(stk);

            string str = {0};
            log_buffer(str.buffer, 100, "%c%s%s", post_exp[i], op1.buffer, op2.buffer);

            stack_push(stk, &str);
        }
    }

    string* str = (string*)stack_top(stk);

    for (i32 i = 0; str->buffer[i] != '\0'; ++i) {
        per_exp[i] = str->buffer[i];
    }

    stack_destroy(stk);
}

void post_pre_run() {

    const char* results[] = {
        "-+a*b^-^cde+f*ghi",
        "*+pq-mn",
        "*+ab-cd",
        "+*AB/CD",
        "-^x+yzw",
        "+^/MNO*PQ",
        "-+*rs/tuv",
        "/-XY+ZW",
        "-+A*BC/DE",
        "*+fg^-hij",
        "-/*PQ+RST",
        "+-UV*/WXY",
        "-^+k*lmn/op",
        "/*^QR+STU",
        "-+/ab*cde",
        "/*F+GH-IJ",
        "/-x*yz+wv",
        "+-*C^DEF/GH",
        "/*+pq-rst",
        "*^K-LM+NO",
        "-+ab^/*cdef",
        "-*/+ABCDE",
    };

    const char* exps[] = {
        "abcd^e-fgh*+^*+i-",
        "pq+mn-*",
        "ab+cd-*",
        "AB*CD/+",
        "xyz+^w-",
        "MN/O^PQ*+",
        "rs*tu/+v-",
        "XY-ZW+/",
        "ABC*+DE/-",
        "fg+hi-j^*",
        "PQ*RS+/T-",
        "UV-WX/Y*+",
        "klm*+n^op/-",
        "QR^ST+*U/",
        "ab/cd*+e-",
        "FGH+*IJ-/",
        "xyz*-wv+/",
        "CDE^*F-GH/+",
        "pq+rs-*t/",
        "KLM-^NO+*",
        "ab+cd*e/f^-",
        "AB+C/D*E-",
    };

    i32 exps_len = sizeof(exps) / sizeof(exps[0]);
    i32 results_len = sizeof(results) / sizeof(results[0]);

    if (exps_len != results_len) {
        LOGE("results are not eqqual to expressions");
        return;
    }

    i32 failed = 0;
    for (i32 i = 0; i < exps_len; ++i) {

        char buffer[200] = {0};
        post_pre(exps[i], buffer);

        if (string_compare(buffer, results[i]) != 0) {
            LOGE("infix = %s , expected = %s , result = %s ", exps[i], results[i], buffer);
            failed += 1;
        } else {
            LOGI("infix = %s , postfix = %s ", exps[i], buffer);
        }
    }
    LOGT("infix to postfix -> test cases failed = %d", failed);
}
