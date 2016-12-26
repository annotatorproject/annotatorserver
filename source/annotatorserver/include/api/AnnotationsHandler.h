// Copyright (c) 2016 Annotator Team
#pragma once

#include <AnnotatorLib/Annotation.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <string>

class AnnotationsHandler : public Poco::Net::HTTPRequestHandler {
 public:
  AnnotationsHandler();

  void handleRequest(Poco::Net::HTTPServerRequest& request,
                     Poco::Net::HTTPServerResponse& response);

 protected:
  void handleGETRequest(Poco::Net::HTTPServerRequest& request,
                        Poco::Net::HTTPServerResponse& response);
  void handlePUTRequest(Poco::Net::HTTPServerRequest& request,
                        Poco::Net::HTTPServerResponse& response);
  void handleDELETERequest(Poco::Net::HTTPServerRequest& request,
                           Poco::Net::HTTPServerResponse& response);
  void handlePOSTRequest(Poco::Net::HTTPServerRequest& request,
                         Poco::Net::HTTPServerResponse& response);

  Poco::JSON::Object::Ptr getAnnotations(unsigned long id);
  Poco::JSON::Object::Ptr getAnnotation(unsigned long id);
  Poco::JSON::Object::Ptr getAnnotation(
      std::shared_ptr<AnnotatorLib::Annotation> annotation);
};
