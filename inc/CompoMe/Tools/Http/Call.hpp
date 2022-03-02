#pragma once

#include "Interfaces/Caller_stream.hpp"
#include "Types/CompoMe/String.hpp"
#include <map>
#include <string>


namespace CompoMe {
  class Interface;
  
namespace Tools {
namespace Http {
using call_result = std::pair<bool, std::string>;
CompoMe::Tools::Http::call_result call(Caller_stream *c, char *cmd);
CompoMe::Tools::Http::call_result
call(const std::map<std::tuple<CompoMe::String>, CompoMe::Interface *> &c,
     char *cmd);
CompoMe::Tools::Http::call_result
call(CompoMe::Interface &def,
     const std::map<std::tuple<CompoMe::String>, CompoMe::Interface *> &c,
     char *cmd);
} // namespace Http
} // namespace Tools
} // namespace CompoMe
