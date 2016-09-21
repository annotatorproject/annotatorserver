// Copyright (c) 2016 Annotator Team
#pragma once

#include <AnnotatorLib/Project.h>

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

class ImageHandler : public Poco::Net::HTTPRequestHandler {
 public:
  ImageHandler(std::shared_ptr<AnnotatorLib::Project> project);

  void handleRequest(Poco::Net::HTTPServerRequest &request,
                     Poco::Net::HTTPServerResponse &response);

 protected:
  std::shared_ptr<AnnotatorLib::Project> project;
};
