#include "pgrsource.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"  // We have no control over FlyCapture2.h
#include <FlyCapture2.h>
#pragma GCC diagnostic pop
#include "pgrcameradata.h"


namespace iu {

PGRSource::PGRSource(unsigned int camId, bool gray)
{
  data_ = new iuprivate::PGRCameraData();
  gray_ = gray;

  // initializes the camera, set width and height member variables
  if (!this->init(camId))
  {
    width_ = 0;
    height_ = 0;
    std::cerr << "PGRSource: Error initializing camera" << std::endl;
  }
  frameNr_ = 0;

}

PGRSource::~PGRSource()
{
  if(data_->cam_->IsConnected())
  {
    FlyCapture2::Error error = data_->cam_->StopCapture();
    if (error != FlyCapture2::PGRERROR_OK)
      this->printError(&error);

    error = data_->cam_->Disconnect();
    if (error != FlyCapture2::PGRERROR_OK)
      this->printError(&error);
  }

  delete data_;
}



bool PGRSource::init(unsigned int camId)
{
  unsigned int numCams;
  FlyCapture2::Error error = data_->bus_mgr_->GetNumOfCameras(&numCams);
  if (error != FlyCapture2::PGRERROR_OK)
  {
    this->printError(&error);
    return false;
  }

  if (numCams <= 0)
    return false;

  if(camId >= numCams)
  {
    std::cerr << "PGRSource::init(): camId " << camId << " exceeds number of cameras, " <<
                 "use camera 0" << std::endl;
    camId = 0;
  }

  if (!this->connectToCamera(camId))
    return false;

  // this sets width and height member variables
  if (!this->startCapture())
    return false;

  return true;
}


cv::Mat PGRSource::getImage()
{
  this->grab();
    if (gray_)
        return cv::Mat(data_->raw_image_->GetRows(), data_->raw_image_->GetCols(),
                 CV_8UC1, data_->raw_image_->GetData(),
                 data_->raw_image_->GetStride());
    else
        return cv::Mat(data_->raw_image_->GetRows(), data_->raw_image_->GetCols(),
                 CV_8UC3, data_->raw_image_->GetData(),
                 data_->raw_image_->GetStride());

}



bool PGRSource::connectToCamera(unsigned int camId)
{
  FlyCapture2::PGRGuid guid;
  FlyCapture2::Error error = data_->bus_mgr_->GetCameraFromIndex(camId, &guid);
  if (error != FlyCapture2::PGRERROR_OK)
  {
    this->printError(&error);
    return false;
  }

  error = data_->cam_->Connect(&guid);
  if (error != FlyCapture2::PGRERROR_OK)
  {
    this->printError(&error);
    return false;
  }

  // set format & framerate
  if (gray_)
      error = data_->cam_->SetVideoModeAndFrameRate(FlyCapture2::VIDEOMODE_640x480Y8, FlyCapture2::FRAMERATE_30);
  else
      error = data_->cam_->SetVideoModeAndFrameRate(FlyCapture2::VIDEOMODE_640x480RGB, FlyCapture2::FRAMERATE_30);


  if (error != FlyCapture2::PGRERROR_OK)
  {
    this->printError(&error);
    return false;
  }
  return true;
}




bool PGRSource::startCapture()
{
  FlyCapture2::Error error = data_->cam_->StartCapture();
  if (error != FlyCapture2::PGRERROR_OK)
  {
    this->printError(&error);
    return false;
  }

  error = data_->cam_->RetrieveBuffer(data_->raw_image_);
  if (error != FlyCapture2::PGRERROR_OK)
    printf( "PGRSource::startCapture(): grab error\n  %s\n", error.GetDescription() );

  width_ = data_->raw_image_->GetCols();
  height_ = data_->raw_image_->GetRows();

  return true;
}



void PGRSource::grab()
{
  // grab camera image
  FlyCapture2::Error error = data_->cam_->RetrieveBuffer(data_->raw_image_);
  if (error != FlyCapture2::PGRERROR_OK)
    printf( "PGRSource::grab(): %s\n", error.GetDescription() );

  frameNr_++;
}





void PGRSource::printError(FlyCapture2::Error *error)
{
  printf("PGRSource Error: %s\n", error->GetDescription());
}


} // namespace iu
