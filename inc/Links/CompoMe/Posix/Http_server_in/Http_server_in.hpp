#pragma once

#include "Data/CompoMe_Http.hpp"

#include "Links/Link.hpp"
struct pollfd;

// TYPES

#include "Types/CompoMe/String.hpp"

#include "Types/i32.hpp"

#include "Types/ui32.hpp"
// STRUCT

// PORT

#include "Ports/CompoMe/Stream/in.hpp"

#include "Ports/CompoMe/Stream/map_in.hpp"

namespace CompoMe {

namespace Posix {

class Http_server_in : public CompoMe::Link {
public:
  Http_server_in();
  virtual ~Http_server_in();

  void step() override;
  void main_connect() override;
  void main_disconnect() override;
  
  // one connect
  void one_connect(CompoMe::Require_helper &, CompoMe::String c)override;
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
  ui32 get_max_client() const;
  void set_max_client(const ui32 max_client);
  ui32 &a_max_client();
  ui32 get_max_request_size() const;
  void set_max_request_size(const ui32 max_request_size);
  ui32 &a_max_request_size();

  // Get Port /////////////////////////////////////////////////////////////

  CompoMe::Stream::in &get_main();
  CompoMe::Stream::map_in &get_many();

public:
  // Function
  // ///////////////////////////////////////////////////////////////////

private:
  int listening_socket;
  struct pollfd *fds;
  ui32 i_fds;
  char *buff;

  // DATA ////////////////////////////////////////////////////////////////////
  CompoMe::String addr;
  i32 port;
  ui32 max_client;
  ui32 max_request_size;

  // PORT ////////////////////////////////////////////////////////////////////
  CompoMe::Stream::in main;
  CompoMe::Stream::map_in many;
};

} // namespace Posix

} // namespace CompoMe
