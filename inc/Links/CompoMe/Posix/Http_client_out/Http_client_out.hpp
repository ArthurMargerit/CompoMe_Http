#pragma once

#include "Links/Link.hpp"

// TYPES

#include "Types/CompoMe/String.hpp"

#include "Types/i32.hpp"
// STRUCT

namespace CompoMe {
class Function_stream;
class Return_stream;
class Interface;
} // namespace CompoMe

#include "Data/CompoMe_Http.hpp"

namespace CompoMe {

namespace Posix {
class Http_client_out;
namespace Http_client_out_ns {
class Function_string_stream_send : public CompoMe::Function_stream_send {
private:
  std::stringstream a_ss;
  Http_client_out &a_l;

public:
  Function_string_stream_send(Http_client_out &p_l);
  void start() final;
  void send() final;
  std::ostream &get_so() override { return this->a_ss; }
};

class Return_string_stream_recv : public CompoMe::Return_stream_recv {
private:
  std::stringstream a_ss;
  Http_client_out &a_l;

public:
  Return_string_stream_recv(Http_client_out &p_l);
  void pull() final;
  void end() final;
  std::istream &get_si() override { return this->a_ss; }
};
} // namespace Http_client_out_ns

class Http_client_out : public CompoMe::Link, public CompoMe::Link_out {
public:
  Http_client_out();
  virtual ~Http_client_out();

  void step() override;
  void connect() override;
  void disconnect() override;

  // Get and set /////////////////////////////////////////////////////////////

  virtual CompoMe::String get_addr() const;
  virtual void set_addr(const CompoMe::String addr);
  CompoMe::String &a_addr();
  virtual i32 get_port() const;
  virtual void set_port(const i32 port);
  i32 &a_port();
  virtual CompoMe::String get_to() const;
  virtual void set_to(const CompoMe::String to);
  CompoMe::String &a_to();

public:
  int get_sock() { return this->sock; }
  // Function
  // ///////////////////////////////////////////////////////////////////
private:
  int sock;
  Http_client_out_ns::Function_string_stream_send fss;
  Http_client_out_ns::Return_string_stream_recv rsr;
  CompoMe::Fake_stream *f;

  // DATA ////////////////////////////////////////////////////////////////////

  CompoMe::String addr;

  i32 port;

  CompoMe::String to;
};

} // namespace Posix

} // namespace CompoMe
