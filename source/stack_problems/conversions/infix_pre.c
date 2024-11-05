#include "infix_pre.h"
#include "stack.h"
#include "common.h"

i32 get_priority_pre(char op) {

    if (op == '+' || op == '-') {
        return 0;
    }
    if (op == '*' || op == '/') {
        return 1;
    }
    if (op == '^') {
        return 2;
    }

    return -1;
}

void infix_pre(const char* infix_exp, char* pre_exp) {

    stack* stk = stack_create(char);

    i32 str_len = string_length(infix_exp);
    i32 j = 0;

    char buffer[200] = {0};

    for (i32 i = str_len - 1; i >= 0; --i) {

        if ((infix_exp[i] >= 'a' && infix_exp[i] <= 'z') || (infix_exp[i] >= 'A' && infix_exp[i] <= 'Z')) {

            buffer[j++] = infix_exp[i];

        } else if (infix_exp[i] == ')') {
            stack_push(stk, &infix_exp[i]);

        } else if (infix_exp[i] == '+' || infix_exp[i] == '-' || infix_exp[i] == '*' || infix_exp[i] == '/' || infix_exp[i] == '^') {

            while (stack_size(stk) != 0 && get_priority_pre(*(char*)stack_top(stk)) > get_priority_pre(infix_exp[i])) {
                buffer[j++] = *(char*)stack_top(stk);
                stack_pop(stk);
            }
            stack_push(stk, &infix_exp[i]);

        } else if (infix_exp[i] == '(') {

            while (stack_size(stk) != 0 && *(char*)stack_top(stk) != ')') {
                buffer[j++] = *(char*)stack_top(stk);
                stack_pop(stk);
            }
            stack_pop(stk);
        }
    }

    while (stack_size(stk) != 0) {
        buffer[j++] = *(char*)stack_top(stk);
        stack_pop(stk);
    }

    i32 buffer_len = string_length(buffer);
    j = 0;

    while (buffer_len--) {
        pre_exp[j++] = buffer[buffer_len];
    }

    stack_destroy(stk);
}

void infix_pre_run() {

    const char* exps[] = {
        "a + b * (c ^ d - e) ^ (f + g * h) - i",
        "(p + q) * (m - n)",
        "(a + b) * (c - d)",
        "A * B + C / D",
        "x ^ (y + z) - w",
        "(M / N) ^ O + P * Q",
        "r * s + t / u - v",
        "(X - Y) / (Z + W)",
        "A + (B * C) - D / E",
        "(f + g) * (h - i) ^ j",
        "(P * Q) / (R + S) - T",
        "U - V + (W / X) * Y",
        "(k + l * m) ^ n - o / p",
        "Q ^ R * (S + T) / U",
        "(a / b) + (c * d) - e",
        "F * (G + H) / (I - J)",
        "(x - y * z) / (w + v)",
        "C * (D ^ E) - F + G / H",
        "(p + q) * (r - s) / t",
        "K ^ (L - M) * (N + O)",
        "a + b - (c * d / e) ^ f",
        "(A + B) / C * D - E",
    };

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

    i32 exps_len = sizeof(exps) / sizeof(exps[0]);
    i32 results_len = sizeof(results) / sizeof(results[0]);

    if (exps_len != results_len) {
        LOGE("results are not eqqual to expressions");
        return;
    }

    i32 failed = 0;
    for (i32 i = 0; i < exps_len; ++i) {

        char buffer[200] = {0};
        infix_pre(exps[i], buffer);

        if (string_compare(buffer, results[i]) != 0) {
            LOGE("infix = %s , expected = %s , result = %s ", exps[i], results[i], buffer);
            failed += 1;
        } else {
            LOGI("infix = %s , prefix = %s ", exps[i], buffer);
        }
    }
    LOGT("infix to prefix -> test cases failed = %d", failed);
}
