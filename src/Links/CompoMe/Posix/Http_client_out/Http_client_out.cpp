#include "Links/CompoMe/Posix/Http_client_out/Http_client_out.hpp"
#include "CompoMe/Log.hpp"
#include "Interfaces/Interface.hpp"
#include "Links/atomizes.hpp"
#include <errno.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
namespace cstd {
#include <arpa/inet.h>
#include <sys/socket.h>
} // namespace cstd

namespace CompoMe {

namespace Posix {

Http_client_out::Http_client_out()
    : CompoMe::Link(), main(), many(), addr("127.0.0.1"), port(80), to("/"),
      fake_many() {
  this->main.set_link(*this);
  this->many.set_link(*this);
}

Http_client_out::~Http_client_out() {}

void Http_client_out::step() { Link::step(); }

void Http_client_out::main_connect() {
  Link::main_connect();
  cstd::sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_port = cstd::htons(this->get_port());
  addr.sin_addr.s_addr = cstd::inet_addr(this->get_addr().str.c_str());

  this->sock = cstd::socket(AF_INET, cstd::SOCK_STREAM, cstd::IPPROTO_TCP);
  if (sock == -1) {
    C_ERROR_TAG("http,client", "Socket creation error: ", strerror(errno));
    return;
  }

  auto r = cstd::connect(this->sock, (cstd::sockaddr *)&addr, sizeof(addr));
  if (r == -1) {
    C_ERROR_TAG("http,client", "Connection error: ", strerror(errno));
    this->main_disconnect();
    return;
  }
}

void Http_client_out::main_disconnect() {
  Link::main_disconnect();
  if (this->sock != -1) {
    close(this->sock);
    this->sock = -1;

    // delete this->f.f;
    // this->f.f = nullptr;
  }
}

// one connect
void Http_client_out::one_connect(CompoMe::Require_helper &p_r,
                                  CompoMe::String p_key) {

  auto& nc = this->fake_many[p_key];

  nc.fss.set_func_send([this, p_key](CompoMe::String_d &d) {
    std::stringstream path;
    path << "/";//p_key.str;

    std::stringstream host;
    host << this->get_addr().str << ":" << this->get_port();

    atomizes::HTTPMessage request;
    request.SetMethod(atomizes::MessageMethod::POST)
        .SetPath(path.str())
        .SetHeader("User-Agent", "Test Agent")
        .SetHeader("Connection", "keep-alive")
        .SetHeader("Host", host.str())
        .SetMessageBody(d.str);

    std::string req_s = request.ToString();

    auto r = cstd::send(this->sock, req_s.c_str(), req_s.size(), 0);
    if (r == -1) {
      C_ERROR_TAG("http,client,send", "Send Error : ", strerror(errno));
      this->main_disconnect();
      throw "connection Error";
    }
    return true;
  });

  nc.rss.set_func_recv([this](CompoMe::String_d &d) {
    char l_buffer[1024 + 2];
    auto e = read(this->sock, l_buffer, 1024);
    if (e == -1) {
      C_ERROR_TAG("http,client", "Receive error");
      this->main_disconnect();
      return false;
    }

    if (e == 0) {
      C_ERROR_TAG("http,client", "Socket close");
      this->main_disconnect();
      return false;
    }

    l_buffer[e] = ' ';
    l_buffer[e + 1] = '\0';

    atomizes::HTTPMessageParser parser;
    atomizes::HTTPMessage reponse;
    parser.Parse(&reponse, l_buffer);

    auto mb = reponse.GetMessageBody();
    std::string str(mb.begin(), mb.end());
    d.str = str;

    return true;
  });
  nc.f = p_r.fake_stream_it(nc.fss, nc.rss);

  std::cout << p_key << "is now connected" << "\n";
}

void Http_client_out::one_connect(CompoMe::Interface &p_i,
                                  CompoMe::String p_key) {}

// one disconnect
void Http_client_out::one_disconnect(CompoMe::Require_helper &p_r,
                                     CompoMe::String p_key) {}

void Http_client_out::one_disconnect(CompoMe::Interface &p_i,
                                     CompoMe::String p_key) {}

} // namespace Posix

} // namespace CompoMe
