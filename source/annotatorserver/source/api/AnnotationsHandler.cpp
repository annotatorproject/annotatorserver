// Copyright (c) 2016 Annotator Team
#include "api/AnnotationsHandler.h"
#include "api/UriParser.h"

#include <strstream>

#include <AnnotatorLib/Algo/InterpolateAnnotation.h>
#include <AnnotatorLib/Class.h>
#include <AnnotatorLib/Object.h>

#include <Poco/Dynamic/Var.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/RegularExpression.h>
#include <Poco/URI.h>

using namespace std;
using Poco::JSON::Stringifier;
using Poco::JSON::Object;
using Poco::JSON::Array;

#include "AccessDeniedHandler.h"
#include "NotFoundHandler.h"
#include "annotatorserver.h"

AnnotationsHandler::AnnotationsHandler() {}

void AnnotationsHandler::handleRequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {
  if (request.getMethod() == "GET")
    handleGETRequest(request, response);
  else if (request.getMethod() == "PUT")
    handlePUTRequest(request, response);
  else if (request.getMethod() == "DELETE")
    handleDELETERequest(request, response);
  else if (request.getMethod() == "POST")
    handlePOSTRequest(request, response);
}

void AnnotationsHandler::handleGETRequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {
  UriParser uri(request.getURI());
  Poco::JSON::Object::Ptr send;
  if (uri.isAnnotations()) {
    std::string frame = uri.getParameter("frame", "");
    if (frame.empty())
      send = getAnnotations(-1);
    else
      send = getAnnotations(std::stoi(frame));
  } else if (uri.isAnnotation()) {
    std::string id = uri.getParameter("id", "");
    if (id.empty())
      send = getAnnotations(-1);
    else
      send = getAnnotation(std::stoi(id));
  }

  if (send->size() == 0) {
    response.setStatusAndReason(Poco::Net::HTTPResponse::HTTP_NOT_FOUND,
                                "No Plugin with this ID!");
    send->set("error", "No Plugin with this ID!");
  } else {
    response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
  }
  response.setContentType("application/json");
  send->stringify(response.send(), 2);
}

void AnnotationsHandler::handlePUTRequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {}

void AnnotationsHandler::handleDELETERequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {}

void AnnotationsHandler::handlePOSTRequest(
    Poco::Net::HTTPServerRequest &request,
    Poco::Net::HTTPServerResponse &response) {
  return;
}

Object::Ptr AnnotationsHandler::getAnnotations(unsigned long id) {
  Object::Ptr pAnnotations = new Object;
  if (id < 1) return pAnnotations;
  std::shared_ptr<AnnotatorLib::Frame> frame =
      AnnotatorServer::getProject()->getSession()->getFrame(id);
  std::shared_ptr<AnnotatorLib::Session> session =
      AnnotatorServer::getProject()->getSession();

  Array::Ptr pArr = new Array;
  if (frame) {
    for (shared_ptr<AnnotatorLib::Annotation> annotation :
         AnnotatorLib::Algo::InterpolateAnnotation::getInterpolations(session,
                                                                      frame)) {
      pArr->add(getAnnotation(annotation));
    }
  }
  pAnnotations->set("annotatoins", pArr);
  return pAnnotations;
}

Object::Ptr AnnotationsHandler::getAnnotation(unsigned long id) {
  std::shared_ptr<AnnotatorLib::Annotation> annotation =
      AnnotatorServer::getProject()->getSession()->getAnnotation(id);
  return getAnnotation(annotation);
}

Object::Ptr AnnotationsHandler::getAnnotation(
    std::shared_ptr<AnnotatorLib::Annotation> annotation) {
  Object::Ptr pAnnotation = new Object;

  if (annotation) {
    if (annotation->isTemporary())
      pAnnotation->set("id", 0);
    else
      pAnnotation->set("id", annotation->getId());

    pAnnotation->set("name", annotation->getObject()->getName());
    pAnnotation->set("x", annotation->getX());
    pAnnotation->set("y", annotation->getY());
    pAnnotation->set("w", annotation->getWidth());
    pAnnotation->set("h", annotation->getHeight());
  }
  return pAnnotation;
}
