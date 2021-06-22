#pragma once

#include "Data/CompoMe_Http.hpp"

#include "Links/Link.hpp"

// TYPES

#include "Types/CompoMe/String.hpp"

#include "Types/i32.hpp"
// STRUCT

// PORT

#include "Ports/CompoMe/Stream/out.hpp"

#include "Ports/CompoMe/Stream/map_out.hpp"

#include "Interfaces/Fake_stream.hpp"
#include "Interfaces/Function_stream_send_func.hpp"
#include "Interfaces/Return_stream_recv_func.hpp"
#include <iostream>

namespace CompoMe {

namespace Posix {
struct Fake_pack {
  Fake_pack() : f(nullptr), fss(), rss() {
    std::cout << "fake build"
         << "\n";
  }

  CompoMe::Fake_stream *f;
  CompoMe::Function_stream_send_func fss;
  CompoMe::Return_stream_recv_func rss;
};

class Http_client_out : public CompoMe::Link {
public:
  Http_client_out();
  virtual ~Http_client_out();

  void step() override;
  void main_connect() override;
  void main_disconnect() override;

  // one connect
  void one_connect(CompoMe::Require_helper &, CompoMe::String c);
  void one_connect(CompoMe::Interface &, CompoMe::String);

  // one disconnect
  void one_disconnect(CompoMe::Require_helper &, CompoMe::String);
  void one_disconnect(CompoMe::Interface &, CompoMe::String);

  // Get and set /////////////////////////////////////////////////////////////

  CompoMe::String get_addr() const;
  void set_addr(const CompoMe::String addr);
  CompoMe::String &a_addr();
  i32 get_port() const;
  void set_port(const i32 port);
  i32 &a_port();
  CompoMe::String get_to() const;
  void set_to(const CompoMe::String to);
  CompoMe::String &a_to();

  // Get Port /////////////////////////////////////////////////////////////

  CompoMe::Stream::out &get_main();
  CompoMe::Stream::map_out &get_many();

public:
  // Function
  // ///////////////////////////////////////////////////////////////////

private:
  int sock;
  std::map<CompoMe::String, struct Fake_pack> fake_many;

  // DATA ////////////////////////////////////////////////////////////////////
  CompoMe::String addr;
  i32 port;
  CompoMe::String to;

  // PORT ////////////////////////////////////////////////////////////////////
  CompoMe::Stream::out main;
  CompoMe::Stream::map_out many;
};

} // namespace Posix

} // namespace CompoMe
