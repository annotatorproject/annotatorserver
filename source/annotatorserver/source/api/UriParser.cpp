// Copyright (c) 2016 Annotator Team
#include "api/UriParser.h"

UriParser::UriParser(std::string uri) : uri(uri) {}

bool UriParser::isAnnotations() { return uri.getPath() == "/api/annotations"; }

bool UriParser::isAnnotation() { return uri.getPath() == "/api/annotation"; }

std::string UriParser::getParameter(std::string key, std::string defaultValue) {
  for (auto parameter : uri.getQueryParameters()) {
    if (parameter.first == key) return parameter.second;
  }
  return defaultValue;
}
