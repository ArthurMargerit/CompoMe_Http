#pragma once

#include "Interfaces/Caller_stream.hpp"
#include <map>
#include <string>

namespace CompoMe {
  namespace Tools {
    namespace Http {
      using call_result = std::pair<bool, std::string>;
      CompoMe::Tools::Http::call_result call(Caller_stream *c, char* cmd);
      CompoMe::Tools::Http::call_result call(std::map<std::string, Caller_stream *> &c, char* cmd);
    } //namespace http
  } // namespace Tools
} // namespace CompoMe
