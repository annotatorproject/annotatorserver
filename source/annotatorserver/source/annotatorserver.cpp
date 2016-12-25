// Copyright (c) 2016 Annotator Team
#include "annotatorserver.h"

#include "RequestHandlerFactory.h"
#include "annotatorserver/annotatorserver-version.h"

#include <chrono>
#include <iostream>
#include <thread>

#ifdef __linux__
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#endif

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Util/HelpFormatter.h>
#include <Poco/Util/Option.h>

using namespace Poco::Util;

using Poco::Util::Option;
using Poco::Util::HelpFormatter;
using Poco::Net::HTTPServerParams;
using Poco::Net::ServerSocket;
using Poco::Net::HTTPServer;

#ifndef RESOURCES_ROOT
constexpr const char *ResourceRoot = "resources";
#else
constexpr const char *ResourceRoot = RESOURCES_ROOT;
#endif
constexpr const unsigned int DefaultPort = 8080;


static std::shared_ptr<AnnotatorLib::Project> project;

std::shared_ptr<AnnotatorLib::Project> AnnotatorServer::getProject()
{
    return project;
}

void AnnotatorServer::initialize(Application &self) {
  loadConfiguration();  // load default configuration files, if present
  ServerApplication::initialize(self);
}

void AnnotatorServer::uninitialize() {
  std::cout << "Shutting Down Annotator Server" << std::endl;
  ServerApplication::uninitialize();
}

int AnnotatorServer::main(const std::vector<std::string> &args) {
  if (_helpRequested) return Application::EXIT_USAGE;

  try {
    project = AnnotatorLib::Project::load(
        config().getString("project-file", "annotatorserver.conf"));

    unsigned short port = (unsigned short)config().getInt("port", DefaultPort);
    _resroot = config().getString("resroot", ResourceRoot);

    HTTPServerParams *params = new HTTPServerParams;
    params->setServerName(ANNOTATORSERVER_PROJECT_NAME);
    params->setSoftwareVersion(ANNOTATORSERVER_VERSION);

    // set-up a server socket
    ServerSocket svs(port);

    // set-up a HTTPServer instance
    HTTPServer srv(new RequestHandlerFactory(project, _docroot, _resroot), svs,
                   params);
    // start the HTTPServer
    srv.start();
    std::cout << "Serving on Port: " << std::to_string(srv.port()) << std::endl;
    std::cout << "Press CTRL-C to quit." << std::endl;
    // wait for CTRL-C or kill
    waitForTerminationRequest();
    // Stop the HTTPServer
    srv.stop();
  } catch (Poco::Exception &e) {
    std::cerr << e.what() << std::endl;
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "unknown Exception" << std::endl;
  }

  return Application::EXIT_OK;
}

void AnnotatorServer::defineOptions(OptionSet &options) {
  Application::defineOptions(options);
  options.addOption(
      Option("help", "h", "Display help information on command line arguments")
          .required(false)
          .repeatable(false)
          .callback(OptionCallback<AnnotatorServer>(
              this, &AnnotatorServer::handleHelp)));
  options.addOption(Option("port", "p", "Port to serve")
                        .required(false)
                        .repeatable(false)
                        .argument("number")
                        .callback(OptionCallback<AnnotatorServer>(
                            this, &AnnotatorServer::handleConfig)));
  options.addOption(Option("user", "u", "Name to login.")
                        .required(false)
                        .repeatable(false)
                        .argument("name")
                        .callback(OptionCallback<AnnotatorServer>(
                            this, &AnnotatorServer::handleConfig)));
  options.addOption(Option("password", "pw", "Password to login.")
                        .required(false)
                        .repeatable(false)
                        .argument("pass")
                        .callback(OptionCallback<AnnotatorServer>(
                            this, &AnnotatorServer::handleConfig)));
  options.addOption(Option("docroot", "d", "Document Root directory")
                        .required(false)
                        .repeatable(false)
                        .argument("path")
                        .callback(OptionCallback<AnnotatorServer>(
                            this, &AnnotatorServer::handleConfig)));
  options.addOption(Option("resroot", "r", "Resources directory")
                        .required(false)
                        .repeatable(false)
                        .argument("path")
                        .callback(OptionCallback<AnnotatorServer>(
                            this, &AnnotatorServer::handleConfig)));
  options.addOption(Option("project-file", "f", "load project from a file")
                        .required(false)
                        .repeatable(false)
                        .argument("file")
                        .callback(OptionCallback<AnnotatorServer>(
                            this, &AnnotatorServer::handleConfig)));
}

void AnnotatorServer::handleHelp(const std::string &name,
                                 const std::string &value) {
  _helpRequested = true;
  displayHelp();
  stopOptionsProcessing();
}

void AnnotatorServer::handleConfig(const std::string &name,
                                   const std::string &value) {
  if (name == "port" || name == "p") {
    std::cout << "Port given: " << value << std::endl;
    config().setInt("port", stoi(value));
  } else if (name == "user" || name == "u") {
    std::cout << "User given: " << value << std::endl;
    config().setString("user", value);
  } else if (name == "password" || name == "pw") {
    std::cout << "Password given: " + value << std::endl;
    config().setString("password", value);
  } else if (name == "docroot" || name == "d") {
    std::cout << "Docroot given: " << value << std::endl;
    _docroot = value;
  } else if (name == "resroot" || name == "r") {
    std::cout << "Resroot given: " << value << std::endl;
    _resroot = value;
  } else if (name == "project-file" || name == "f") {
    std::cout << "Loading Config from File: " << value << std::endl;
    config().setString("project-file", value);
  }
}

void AnnotatorServer::displayHelp() {
  HelpFormatter helpFormatter(options());
  helpFormatter.setCommand(commandName());
  helpFormatter.setUsage("OPTIONS");
  helpFormatter.setHeader("Annotator Server ...");
  helpFormatter.format(std::cout);
}

POCO_SERVER_MAIN(AnnotatorServer)
