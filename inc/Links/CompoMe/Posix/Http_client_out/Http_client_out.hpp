#pragma once

#include "Data/CompoMe_Http.hpp"

#include "Links/Link.hpp"

#include "Links/CompoMe/Posix/Fake_pack.hpp"

// TYPES

#include "Types/CompoMe/String.hpp"

#include "Types/i32.hpp"
// STRUCT

// PORT

#include "Ports/CompoMe/Stream/out.hpp"

#include "Ports/CompoMe/Stream/map_out.hpp"

#include <iostream>

namespace CompoMe {

namespace Posix {

class Http_client_out : public CompoMe::Link {
public:
  Http_client_out();
  virtual ~Http_client_out();

  void step() override;
  void main_connect() override;
  void main_disconnect() override;

  // one connect
  void one_connect(CompoMe::Require_helper &, CompoMe::String c) override;
  void one_connect(CompoMe::Interface &, CompoMe::String) override;

  // one disconnect
  void one_disconnect(CompoMe::Require_helper &, CompoMe::String) override;
  void one_disconnect(CompoMe::Interface &, CompoMe::String) override;

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
  std::map<CompoMe::String, struct CompoMe::Posix::Fake_pack> fake_many;

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
