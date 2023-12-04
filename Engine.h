#pragma once
#include <unordered_map>
#include <string>
#include "Wrapper.h"

class Engine {
public:
  void registerCommand(WrapperBase* wrapper, std::string const& name);
  int execute(std::string const& name, WrapperBase::ArgsList const& args);

private:
  std::unordered_map<std::string, WrapperBase*> m_commands;
};

void Engine::registerCommand(WrapperBase* wrapper, std::string const& name) {
  m_commands[name] = wrapper;
}

int Engine::execute(std::string const& name, WrapperBase::ArgsList const& args) {
  return (*m_commands.at(name))(args);
}