// Copyright (c) 2016 Annotator Team
#pragma once

#include <AnnotatorLib/Project.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
 public:
  RequestHandlerFactory(std::shared_ptr<AnnotatorLib::Project> project,
                        const std::string& docroot, const std::string& resroot);

  Poco::Net::HTTPRequestHandler* createRequestHandler(
      const Poco::Net::HTTPServerRequest& request);

 private:
  const std::string& _docroot;
  const std::string& _resroot;

  std::shared_ptr<AnnotatorLib::Project> _project;
};
