// Copyright (c) 2016 Annotator Team
#include "imagehandler.h"

#include <iostream>
#include <string>

#include <opencv2/opencv.hpp>

#include <Poco/Net/HTTPBasicCredentials.h>
#include <Poco/Net/MessageHeader.h>
#include <Poco/Net/MultipartWriter.h>

ImageHandler::ImageHandler(std::shared_ptr<AnnotatorLib::Project> project) {
  this->project = project;
}

void ImageHandler::handleRequest(Poco::Net::HTTPServerRequest &request,
                                 Poco::Net::HTTPServerResponse &response) {
  int i = std::stoi(request.getURI().substr(7));

  cv::Mat image = project->getImageSet()->getImage(i);

  response.setChunkedTransferEncoding(false);
  // response.set("Max-Age", "0");
  // response.set("Expires", "0");
  // response.set("Pragma", "no-cache");
  // response.set("Cache-Control", "no-cache");
  response.setContentType("Content-Type: image/jpeg");
  std::ostream &ostr = response.send();
  std::vector<uchar> buffer;
  cv::imencode(".jpg", image, buffer);
  ostr.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());
}
