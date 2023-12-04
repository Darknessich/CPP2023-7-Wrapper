#pragma once
#include <string>
#include <exception>
#include <unordered_map>
#include <functional>

struct WrapperErr : std::exception {
  using std::exception::exception;
};

class WrapperBase {
public:
  using ArgsList = std::unordered_map<std::string, int>;
  virtual int operator()(ArgsList const&) = 0;
  virtual ~WrapperBase() = default;
};

template<typename Type, typename... Args>
class Wrapper : public WrapperBase {
public:
  using Method = int (Type::*)(Args...);

  Wrapper(Type* instance, Method method, ArgsList const& defMap);
  int operator()(ArgsList const&) override;

private:
  void setDefault();
  bool validate(ArgsList const& args);
  void setValues(ArgsList const& args);

  template<size_t... Idx>
  int call(std::index_sequence<Idx...>);

  ArgsList m_args;
  int m_values[sizeof...(Args)];
  std::function<int(Args...)> m_function;
};

// ******************************* Constructor ********************************
template<typename Type, typename... Args>
Wrapper<Type, Args...>::
Wrapper(Type* instance, Method method, ArgsList const& args)
  : m_args(args)
{
  if (sizeof...(Args) != args.size())
    throw WrapperErr("number of arguments and number of default parameters do not match");

  this->setDefault();
  m_function = [instance, method](Args... args) -> int {
    return (instance->*method)(args...);
  };
}

// ******************************** operator() ********************************
template<typename Type, typename... Args>
int Wrapper<Type, Args...>::
operator()(ArgsList const& args) {
  if (sizeof...(Args) < args.size())
    throw WrapperErr("too many arguments");

  if (!validate(args))
    throw WrapperErr("Invalid arguments");

  setDefault();
  setValues(args);

  return call(std::make_index_sequence<sizeof...(Args)>{});
}

// *************************** secondary functions ****************************
template<typename Type, typename... Args>
void Wrapper<Type, Args...>::
setDefault() {
  int idx = 0;
  for (auto&& [name, val] : m_args)
    m_values[idx++] = val;
}

template<typename Type, typename... Args>
bool Wrapper<Type, Args...>::
validate(ArgsList const& args) {
  for (auto&& [key, val] : args)
    if (m_args.find(key) == m_args.end())
      return false;
  return true;
}

template<typename Type, typename... Args>
void Wrapper<Type, Args...>::
setValues(ArgsList const& args) {
  int idx = 0;
  for (auto&& [key, val] : m_args)
    m_values[idx++] = (args.find(key) == args.end()) ? val : args.at(key);
}

template<typename Type, typename... Args>
template<size_t... Idx>
int Wrapper<Type, Args...>::
call(std::index_sequence<Idx...>) {
  return m_function(m_values[Idx]...);
}