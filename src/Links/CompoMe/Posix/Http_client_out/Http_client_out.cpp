#include "Links/CompoMe/Posix/Http_client_out/Http_client_out.hpp"

#include "Interfaces/Interface.hpp"

namespace CompoMe {

namespace Posix {

Http_client_out::Http_client_out() : CompoMe::Link(), main(), many() {
  this->main.set_link(*this);
  this->many.set_link(*this);
}

Http_client_out::~Http_client_out() {}

void Http_client_out::step() { Link::step(); }

void Http_client_out::main_connect() { Link::main_connect(); }

void Http_client_out::main_disconnect() { Link::main_disconnect(); }

// one connect
void Http_client_out::one_connect(CompoMe::Require_helper &p_r,
                                  CompoMe::String p_key) {}

void Http_client_out::one_connect(CompoMe::Interface &p_i,
                                  CompoMe::String p_key) {}

// one disconnect
void Http_client_out::one_disconnect(CompoMe::Require_helper &p_r,
                                     CompoMe::String p_key) {}

void Http_client_out::one_disconnect(CompoMe::Interface &p_i,
                                     CompoMe::String p_key) {}

} // namespace Posix

} // namespace CompoMe
