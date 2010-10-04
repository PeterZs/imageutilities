/*
 * Copyright (c) ICG. All rights reserved.
 *
 * Institute for Computer Graphics and Vision
 * Graz University of Technology / Austria
 *
 *
 * This software is distributed WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE.  See the above copyright notices for more information.
 *
 *
 * Project     : ImageUtilities
 * Module      : IO
 * Class       : VideoCapture
 * Language    : C++
 * Description : Definition of VideoCapture that uses OpenCV to read either from a video file or a camera.
 *
 * Author     : Manuel Werlberger
 * EMail      : werlberger@icg.tugraz.at
 *
 */

#ifndef IU_VIDEOCAPTURE_H
#define IU_VIDEOCAPTURE_H

#include <iudefs.h>

// forward declarations
namespace iuprivate {
class VideoCapture;
}

namespace iu {

class IU_DLLAPI VideoCapture
{
public:

  /** Default constructor. */
  VideoCapture();

  /** Constructor that opens a video file. */
  VideoCapture(std::string& filename);

  /** Constructor that opens a camera. */
  VideoCapture(int device);

  /** Default destructor. */
  ~VideoCapture();

  /** Retrieves cpu image (8-bit; 1-channel). */
  virtual bool retrieve(iu::ImageCpu_8u_C1* image);

  /** Retrieves cpu image (32-bit; 1-channel). */
  virtual bool retrieve(iu::ImageCpu_32_C1* image);

  /** Retrieves gpu image (32-bit; 1-channel). */
  virtual bool retrieve(iu::ImageGpu_32_C1* image);


private:
  iuprivate::VideoCapture vidcap_;

};

} // namespace iu

#endif // IU_VIDEOCAPTURE_H
