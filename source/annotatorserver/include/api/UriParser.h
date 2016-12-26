// Copyright (c) 2016 Annotator Team
#pragma once

#include <string>

#include <Poco/URI.h>

class UriParser {
 public:
  UriParser(std::string uri);

  bool isAnnotations();
  bool isAnnotation();

  std::string getParameter(std::string key, std::string defaultValue);

 protected:
  Poco::URI uri;
};
