// Copyright (c) 2016 Annotator Team
#pragma once

#include <AnnotatorLib/Project.h>

#include <Poco/Util/OptionSet.h>
#include <Poco/Util/ServerApplication.h>

class AnnotatorServer : public Poco::Util::ServerApplication {
 public:
  AnnotatorServer() {}

  ~AnnotatorServer() {}

  static std::shared_ptr<AnnotatorLib::Project> getProject();

 protected:
  void initialize(Application &self);
  void uninitialize();
  int main(const std::vector<std::string> &args);
  void defineOptions(Poco::Util::OptionSet &options);
  void handleHelp(const std::string &name, const std::string &value);
  void handleExampleFile(const std::string &name, const std::string &value);
  void handleConfig(const std::string &name, const std::string &value);
  void displayHelp();

 private:
  bool _helpRequested = false;
  bool _exampleRequested = false;
  std::string _docroot;
  std::string _resroot;
};
