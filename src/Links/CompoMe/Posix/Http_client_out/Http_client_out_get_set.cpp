#include "Links/CompoMe/Posix/Http_client_out/Http_client_out.hpp"

namespace CompoMe {

namespace Posix {

//============================= addr =============================
CompoMe::String Http_client_out::get_addr() const { return this->addr; }

void Http_client_out::set_addr(const CompoMe::String p_addr) {
  this->addr = p_addr;
}

CompoMe::String &Http_client_out::a_addr() { return this->addr; }
//--------------------------------------------------------------------------
//============================= port =============================
i32 Http_client_out::get_port() const { return this->port; }

void Http_client_out::set_port(const i32 p_port) { this->port = p_port; }

i32 &Http_client_out::a_port() { return this->port; }
//--------------------------------------------------------------------------
//============================= to =============================
CompoMe::String Http_client_out::get_to() const { return this->to; }

void Http_client_out::set_to(const CompoMe::String p_to) { this->to = p_to; }

CompoMe::String &Http_client_out::a_to() { return this->to; }
//--------------------------------------------------------------------------

// Get Port /////////////////////////////////////////////////////////////

CompoMe::Stream::out &Http_client_out::get_main() { return this->main; }
CompoMe::Stream::map_out &Http_client_out::get_many() { return this->many; }

} // namespace Posix

} // namespace CompoMe
