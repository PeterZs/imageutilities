#pragma once

//
//  W A R N I N G
//  -------------
//
// This file is not part of the IU API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//

#include "coredefs.h"
#include "memorydefs.h"

namespace iuprivate {

// [host] 2D bit depth conversion; 32f_C1 -> 8u_C1;
void convert_32f8u_C1(const iu::ImageCpu_32f_C1* src, iu::ImageCpu_8u_C1 *dst,
                      float mul_constant=255.0f, float add_constant=0.0f);

// [host] 2D bit depth conversion; 16u_C1 -> 32f_C1;
void convert_16u32f_C1(const iu::ImageCpu_16u_C1* src, iu::ImageCpu_32f_C1 *dst,
                       float mul_constant, float add_constant);

 // [device] 2D bit depth conversion; 32f_C1 -> 8u_C1;
void convert_32f8u_C1(const iu::ImageGpu_32f_C1* src, iu::ImageGpu_8u_C1 *dst,
                      float mul_constant=255.0f, unsigned char add_constant=0);

// [device] 2D bit depth conversion; 32f_C4 -> 8u_C4;
void convert_32f8u_C4(const iu::ImageGpu_32f_C4* src, iu::ImageGpu_8u_C4 *dst,
                     float mul_constant=255.0f, unsigned char add_constant=0);

// [device] 2D bit depth conversion; 8u_C1 -> 32f_C1;
void convert_8u32f_C1(const iu::ImageGpu_8u_C1* src, iu::ImageGpu_32f_C1 *dst,
                       float mul_constant=1/255.0f, float add_constant=0.0f);

// [device] 2D bit depth conversion; 32u_C1 -> 32f_C1;
void convert_32u32f_C1(const iu::ImageGpu_32u_C1* src, iu::ImageGpu_32f_C1 *dst,
                       float mul_constant, float add_constant=0.0f);

void convert_32s32f_C1_lin(iu::LinearDeviceMemory_32s_C1 *src, iu::LinearDeviceMemory_32f_C1 *dest);

// [host] 2D bit depth conversion; 32u_C1 -> 32f_C1;
void convert_32u32f_C1(const iu::ImageCpu_32u_C1* src, iu::ImageCpu_32f_C1 *dst,
                       float mul_constant, float add_constant=0.0f);


void convert_8u32f_C3C4(const iu::ImageGpu_8u_C3* src, iu::ImageGpu_32f_C4 *dst,
                       float mul_constant=1/255.0f, float add_constant=0.0f);


// 2D conversion; device; 32-bit 3-channel -> 32-bit 4-channel
void convert(const iu::ImageGpu_32f_C3* src, iu::ImageGpu_32f_C4* dst);

// 2D conversion; device; 32-bit 4-channel -> 32-bit 3-channel
void convert(const iu::ImageGpu_32f_C4* src, iu::ImageGpu_32f_C3* dst);

// [device] 2D Color conversion from RGB to HSV (32-bit 4-channel)
void convertRgbHsv(const iu::ImageGpu_32f_C4* src, iu::ImageGpu_32f_C4* dst, bool normalize);

// [device] 2D Color conversion from HSV to RGB (32-bit 4-channel)
void convertHsvRgb(const iu::ImageGpu_32f_C4* src, iu::ImageGpu_32f_C4* dst, bool denormalize);

// [device] 2D Color conversion from RGB to CIELAB (32-bit 4-channel)
void convertRgbLab(const iu::ImageGpu_32f_C4* src, iu::ImageGpu_32f_C4* dst, bool isNormalized);

// [device] 2D Color conversion from CIELAB to RGB (32-bit 4-channel)
void convertLabRgb(const iu::ImageGpu_32f_C4* src, iu::ImageGpu_32f_C4* dst);

//double summation(iu::ImageGpu_32f_C1* src);

} // namespace iuprivate

