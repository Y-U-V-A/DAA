#include "pre_infix.h"
#include "stack.h"
#include "logger.h"
#include "common.h"

typedef struct string {
    char buffer[100];
} string;

void pre_infix(const char* pre_exp, char* infix_exp) {

    stack* stk = stack_create(string);

    i32 len = string_length(pre_exp);

    for (i32 i = len - 1; i >= 0; --i) {
        if ((pre_exp[i] >= 'a' && pre_exp[i] <= 'z') || (pre_exp[i] >= 'A' && pre_exp[i] <= 'Z')) {

            string str = {0};
            str.buffer[0] = pre_exp[i];
            stack_push(stk, &str);

        } else if (pre_exp[i] == '+' || pre_exp[i] == '-' || pre_exp[i] == '*' || pre_exp[i] == '/' || pre_exp[i] == '^') {
            string op2 = *(string*)stack_top(stk);
            stack_pop(stk);

            string op1 = *(string*)stack_top(stk);
            stack_pop(stk);

            string str = {0};
            log_buffer(str.buffer, 100, "(%s %c %s)", op2.buffer, pre_exp[i], op1.buffer);

            stack_push(stk, &str);
        }
    }

    string* str = (string*)stack_top(stk);

    for (i32 i = 0; str->buffer[i] != '\0'; ++i) {
        infix_exp[i] = str->buffer[i];
    }

    stack_destroy(stk);
}

void pre_infix_run() {

    const char* results[] = {
        "((a + (b * (((c ^ d) - e) ^ (f + (g * h))))) - i)",
        "((p + q) * (m - n))",
        "((a + b) * (c - d))",
        "((A * B) + (C / D))",
        "((x ^ (y + z)) - w)",
        "(((M / N) ^ O) + (P * Q))",
        "(((r * s) + (t / u)) - v)",
        "((X - Y) / (Z + W))",
        "((A + (B * C)) - (D / E))",
        "((f + g) * ((h - i) ^ j))",
        "(((P * Q) / (R + S)) - T)",
        "((U - V) + ((W / X) * Y))",
        "(((k + (l * m)) ^ n) - (o / p))",
        "(((Q ^ R) * (S + T)) / U)",
        "(((a / b) + (c * d)) - e)",
        "((F * (G + H)) / (I - J))",
        "((x - (y * z)) / (w + v))",
        "(((C * (D ^ E)) - F) + (G / H))",
        "(((p + q) * (r - s)) / t)",
        "((K ^ (L - M)) * (N + O))",
        "((a + b) - (((c * d) / e) ^ f))",
        "((((A + B) / C) * D) - E)",
    };

    const char* exps[] = {
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
        pre_infix(exps[i], buffer);

        if (string_compare(buffer, results[i]) != 0) {
            LOGE("infix = %s , expected = %s , result = %s ", exps[i], results[i], buffer);
            failed += 1;
        } else {
            LOGI("infix = %s , postfix = %s ", exps[i], buffer);
        }
    }
    LOGT("infix to postfix -> test cases failed = %d", failed);
}
