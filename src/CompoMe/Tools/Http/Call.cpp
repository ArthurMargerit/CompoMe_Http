#include "CompoMe/Tools/Http/Call.hpp"
#include "CompoMe/Tools/Call.hpp"
#include "CompoMe/Tools/Http/atomizes.hpp"

namespace CompoMe {
  namespace Tools {
    namespace Http {
namespace {
atomizes::HTTPMessage get_rep(int Status_code, std::string mess) {
   atomizes::HTTPMessage response;
   response.SetStatusCode(Status_code)
     .SetHeader("Content-Type", "text/plain")
     .SetHeader("Access-Control-Allow-Origin", "*")
     .SetHeader("Connection", "keep-alive")
     .SetMessageBody(mess);
   return response;
}

CompoMe::Tools::Http::call_result call_r(Caller_stream* c, atomizes::HTTPMessage request) {

  CompoMe::Tools::call_result result_of_call;
  std::string body(request.GetMessageBody().begin(), request.GetMessageBody().end());

  if(request.GetMessageBody()[0] == '?'){
    std::stringstream ss;
    c->introspection(ss);
    result_of_call = {true, ss.str()};
  } else {
    result_of_call = CompoMe::Tools::call(c, body);
  }

  return {result_of_call.first,
    get_rep((result_of_call.first)?200:400,
            result_of_call.second).ToString()};
}
}

CompoMe::Tools::Http::call_result call(Caller_stream* c, char *cmd) {
  if (cmd == nullptr) {
    return {false, get_rep(400,"CMD is empty, no call").ToString()};
  }

  atomizes::HTTPMessageParser parser;
  atomizes::HTTPMessage       request;
  parser.Parse(&request, cmd);
  return call_r(c, request);
}

CompoMe::Tools::Http::call_result call(std::map<std::string,Caller_stream*>& c, char *cmd) {
  if (cmd == nullptr) {
    return {false, get_rep(400,"CMD is empty, no call").ToString()};
  }

  atomizes::HTTPMessageParser parser;
  atomizes::HTTPMessage       request;
  parser.Parse(&request, cmd);

  if (request.GetMethod() != atomizes::MessageMethod::POST) {
     return {false, get_rep(400,"The request is not a POST method").ToString()};
  }

  if (request.GetPath() == "/" && request.GetMessageBody()[0] == '?') {
    std::stringstream ss;
    for (auto& kv : c) {
      ss << "<Interface name=\"" << kv.first<< "\">\n";
      kv.second->introspection(ss);
      ss << "</Interface>\n";
    }
    return {true, get_rep(200, ss.str()).ToString()};
  }

  auto r = c.find(request.GetPath());
  if (r == c.end()) {
    return {false, get_rep(400,"No interface for " + request.GetPath()).ToString()};
  }

  return call_r((*r).second, request);
}

    }
  }
}
