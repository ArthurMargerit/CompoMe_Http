#include "Links/CompoMe/Posix/Http_server_in/Http_server_in.hpp"
#include "CompoMe/Log.hpp"
#include "CompoMe/Tools/Http/Call.hpp"
#include "Links/atomizes.hpp"
#include "Interfaces/Interface.hpp"
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/poll.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

namespace CompoMe {

namespace Posix {

Http_server_in::Http_server_in()
    : CompoMe::Link(), listening_socket(-1), fds(nullptr), i_fds(0),
      buff(nullptr) {
  this->listening_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (listening_socket == -1) {
    C_ERROR("socket() failed");
    return;
  }

  int enable = 1;
  setsockopt(this->listening_socket, SOL_SOCKET, SO_REUSEADDR, &enable,
             sizeof(int));
}

Http_server_in::~Http_server_in() { this->disconnect(); }

void Http_server_in::step() {
  Link::step();

  int ret = poll(fds, this->i_fds, 0);

  if (ret == 0) {
    // C_DEBUG("Timeout Poll() on socket");
    return;
  }

  if (ret == -1) {
    C_ERROR("Error in Poll() on socket");
    return;
  }

  // socket message
  // if it's the listening_socket manage the new connection
  if (this->fds[0].revents & POLLIN) {
    if (this->i_fds == this->get_max_client()) {
      auto socket_err = accept(this->fds[0].fd, nullptr, nullptr);
      write(socket_err, "!!!MAX!!! CONNECTION", 20);
      close(socket_err);
    } else {
      fds[this->i_fds].fd = accept(this->fds[0].fd, nullptr, nullptr);
      fds[this->i_fds].events = POLLIN | POLLHUP | POLLERR;
      fds[this->i_fds].revents = 0;

      fds[0].revents = fds[0].revents - POLLIN; // remove it
      this->i_fds++;
      C_INFO_TAG("http,server,recv", "new client");
    }
  }

  if (this->fds[0].revents & POLLHUP) {
    this->disconnect();
  }

  for (int i = 1; i < this->i_fds; i++) {


    if (this->fds[i].revents & POLLERR) {
      C_ERROR("Error in Poll()");
      continue;
    }

    if (fds[i].revents & POLLHUP) {
      // close socket
      close(this->fds[i].fd);
      this->fds[i].events = 0;
      this->fds[i].revents = 0;

      // swap it with the lastone
      this->fds[i].fd = this->fds[this->i_fds - 1].fd;
      this->fds[i].events = this->fds[this->i_fds - 1].events;
      this->fds[i].revents = this->fds[this->i_fds - 1].revents;

      this->i_fds--;
      i--;
      continue;
    }

    if (this->fds[i].revents & POLLIN) {
      ssize_t readden = recv(this->fds[i].fd,
                             this->buff,
                             this->get_max_request_size(), 0);
      if(readden == 0 || readden == 1 ) {
        continue;
      }
      buff[readden] = '\0';

      auto result = CompoMe::Tools::Http::call(&this->get_caller_stream(),
                                               buff);

      int r = send(fds[i].fd, result.second.c_str(),
               result.second.length(), MSG_NOSIGNAL);
      if (r == -1) {
        C_ERROR_TAG("http,server,recv", "respond sending failled",
                    strerror(errno));
      }
      
    }


  }
}

void Http_server_in::connect() {
  Link::connect();

  struct sockaddr_in addr = {0};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(this->get_port());
  addr.sin_addr.s_addr = inet_addr(this->get_addr().str.c_str());
  {
    auto r = bind(listening_socket, (const struct sockaddr *)&addr,
                  sizeof(struct sockaddr));

    if (0 > r) {
      C_ERROR("bind at ", this->get_addr(), ":", this->get_port(),
              " failed :", strerror(errno));
      return;
    }
  }

  {
    auto r = listen(this->listening_socket, 0);
    if (-1 == r) {
      C_ERROR("Listen failed : ", strerror(errno));
      return;
    }
  }

  {
    // +1 due to listening socket in this->fds[0]
    this->fds = (struct pollfd *)malloc((this->get_max_client() + 1) *
                                        sizeof(*this->fds));
    if (this->fds == nullptr) {
      C_ERROR("Malloc failed for fds size asked is ",
              (this->get_max_client() + 1) * sizeof(*this->fds));
      this->disconnect();
      return;
    }
  }

  {
    this->buff = (char *)malloc(sizeof(char) * this->get_max_request_size());
    if (this->buff == nullptr) {
      this->disconnect();
      C_ERROR("malloc failled for buff");
    }
  }

  {
    this->fds[0].fd = this->listening_socket;
    this->fds[0].events = POLLIN | POLLERR | POLLHUP;
    this->fds[0].revents = 0;
    this->i_fds = 1; // listening socket
  }
}

void Http_server_in::disconnect() {
  Link::disconnect();
  if (this->listening_socket != -1) {
    close(this->listening_socket);
    this->listening_socket = -1;
  }

  if (this->fds != nullptr) {
    for (int i = 1; i < this->i_fds; i++) {
      close(this->fds[i].fd);
    }
    free(this->fds);
    this->fds = nullptr;
  }

  free(this->buff);
  this->buff = nullptr;
}

// Get and set /////////////////////////////////////////////////////////////

CompoMe::String Http_server_in::get_addr() const { return this->addr; }

void Http_server_in::set_addr(const CompoMe::String p_addr) {
  this->addr = p_addr;
}
i32 Http_server_in::get_port() const { return this->port; }

void Http_server_in::set_port(const i32 p_port) { this->port = p_port; }
ui32 Http_server_in::get_max_client() const { return this->max_client; }

void Http_server_in::set_max_client(const ui32 p_max_client) {
  this->max_client = p_max_client;
}
ui32 Http_server_in::get_max_request_size() const {
  return this->max_request_size;
}

void Http_server_in::set_max_request_size(const ui32 p_max_request_size) {
  this->max_request_size = p_max_request_size;
}

} // namespace Posix

} // namespace CompoMe
