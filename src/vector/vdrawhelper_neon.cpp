#if defined(__ARM_NEON__)

#include "vdrawhelper.h"

#if !defined(__APPLE__)
extern "C" void pixman_composite_src_n_8888_asm_neon(int32_t w, int32_t h,
                                                     uint32_t *dst,
                                                     int32_t   dst_stride,
                                                     uint32_t  src);

extern "C" void pixman_composite_over_n_8888_asm_neon(int32_t w, int32_t h,
                                                      uint32_t *dst,
                                                      int32_t   dst_stride,
                                                      uint32_t  src);

void memfill32(uint32_t *dest, uint32_t value, int length)
{
    pixman_composite_src_n_8888_asm_neon(length, 1, dest, length, value);
}

static void color_SourceOver(uint32_t *dest, int length,
                                      uint32_t color,
                                      uint32_t const_alpha)
{
    if (const_alpha != 255) color = BYTE_MUL(color, const_alpha);

    pixman_composite_over_n_8888_asm_neon(length, 1, dest, length, color);
}

#else
void memfill32(uint32_t *dest, uint32_t value, int length)
{
    memset(dest, length, value);
}

static void color_SourceOver(uint32_t *dest, int length,
                                      uint32_t color,
                                      uint32_t const_alpha)
{
    if (const_alpha != 255) color = BYTE_MUL(color, const_alpha);

    int ialpha, i;
    ialpha = 255 - vAlpha(color);
    for (i = 0; i < length; ++i) dest[i] = color + BYTE_MUL(dest[i], ialpha);
}

#endif  // !defined(__APPLE__)

void RenderFuncTable::neon()
{
    updateColor(BlendMode::Src , color_SourceOver);
}
#endif  // defined(__ARM_NEON__)
