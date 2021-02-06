#include "Links/CompoMe/Posix/Http_client_out/Http_client_out.hpp"
#include "CompoMe/Log.hpp"
#include "Interfaces/Interface.hpp"
#include "Links/atomizes.hpp"
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

namespace cstd {
#include <arpa/inet.h>
#include <sys/socket.h>
}  // cstd>
#include <sstream>
#include <string>

namespace CompoMe {

namespace Posix {

Http_client_out::Http_client_out()
    : CompoMe::Link(), fss(*this), rsr(*this), f(nullptr) {}

Http_client_out::~Http_client_out() {}

void Http_client_out::step() { Link::step(); }

void Http_client_out::connect() {

  Link::connect();
  cstd::sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_port = cstd::htons(this->get_port());
  addr.sin_addr.s_addr = cstd::inet_addr(this->get_addr().str.c_str());

  this->sock = cstd::socket(AF_INET, cstd::SOCK_STREAM, cstd::IPPROTO_TCP);
  if (sock == -1) {
    C_ERROR_TAG("http,client", "Socket creation error: ", strerror(errno));
    return;
  }

  auto r =
    cstd::connect(this->sock, (cstd::sockaddr *)&addr, sizeof(addr));
  if (r == -1) {
    C_ERROR_TAG("http,client", "Connection error: ", strerror(errno));
    this->disconnect();
    return;
  }

  this->f = this->a_re->fake_stream_it(fss, rsr);
}

void Http_client_out::disconnect() {
  Link::disconnect();
  if (this->sock != -1) {
    close(this->sock);
    this->sock = -1;

    delete this->f;
    this->f = nullptr;
  }
}

// Get and set /////////////////////////////////////////////////////////////

CompoMe::String Http_client_out::get_addr() const { return this->addr; }

void Http_client_out::set_addr(const CompoMe::String p_addr) {
  this->addr = p_addr;
}

CompoMe::String &Http_client_out::a_addr() { return this->addr; }
i32 Http_client_out::get_port() const { return this->port; }

void Http_client_out::set_port(const i32 p_port) { this->port = p_port; }

i32 &Http_client_out::a_port() { return this->port; }
CompoMe::String Http_client_out::get_to() const { return this->to; }

void Http_client_out::set_to(const CompoMe::String p_to) { this->to = p_to; }

CompoMe::String &Http_client_out::a_to() { return this->to; }

namespace Http_client_out_ns {
// stream
Return_string_stream_recv::Return_string_stream_recv(Http_client_out &p_l)
    : CompoMe::Return_stream_recv(), a_l(p_l) {}

void Return_string_stream_recv::pull() {
  char l_buffer[1024 + 2];
  auto e = read(this->a_l.get_sock(), l_buffer, 1024);
  if (e == -1) {
    C_ERROR_TAG("http,client", "Receive error");
    this->a_l.disconnect();
    return;
  }

  if (e == 0) {
    C_ERROR_TAG("http,client", "Socket close");
    this->a_l.disconnect();
    return;
  }

  l_buffer[e] = ' ';
  l_buffer[e + 1] = '\0';

  atomizes::HTTPMessageParser parser;
  atomizes::HTTPMessage reponse;
  parser.Parse(&reponse, l_buffer);
  C_DEBUG_TAG("http,client,recv", "answer: ", reponse.GetMessageBody());
  auto mb = reponse.GetMessageBody();
  std::string str(mb.begin(),mb.end());
  this->a_ss.str(str);
}

void Return_string_stream_recv::end() { this->a_ss.str(""); }

Function_string_stream_send::Function_string_stream_send(Http_client_out &p_l)
    : a_l(p_l) {}

void Function_string_stream_send::start() {
  this->a_ss.str("");

}

void Function_string_stream_send::send() {
  C_DEBUG_TAG("http,client,send", "call: ", this->a_ss.str());

  atomizes::HTTPMessage request;

{
  std::stringstream path;
  if (this->a_l.get_to().str == "") {
    path << "/" ;
  }else{
    path << this->a_l.get_to().str;
  }

  std::stringstream host;
  host  << this->a_l.get_addr().str<<":"<<this->a_l.get_port();

  request.SetMethod(atomizes::MessageMethod::POST)
    .SetPath(path.str())
    .SetHeader("User-Agent", "Test Agent")
    .SetHeader("Connection", "keep-alive")
    .SetHeader("Host", host.str())
    .SetMessageBody(this->a_ss.str());

}

 std::string req_s = request.ToString();

 auto r = cstd::send(this->a_l.get_sock(), req_s.c_str(),
                      req_s.size(), 0);
  if (r == -1) {
    C_ERROR_TAG("http,client,send", "Send Error : ", strerror(errno));
    this->a_l.disconnect();
    throw "connection Error";
  }
}
} // namespace Http_client_out_ns
} // namespace Posix

} // namespace CompoMe
