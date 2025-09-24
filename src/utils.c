#include <utils.h>

extern Context* ctx;

void freeContext() {
    if (ctx) {
        if (ctx->data) {
            free(ctx->data);
        }
        free(ctx);
    }
}