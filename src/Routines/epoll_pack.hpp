#ifndef _EPOLL_PACK_H_
#define _EPOLL_PACK_H_
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstddef>
#include <cstdint>
#include <cstdio>

namespace let_me_see {
class EpollPack {
 public:
  using EpollEventType = struct epoll_event;
  using EpollEventPtr = struct epoll_event *;
  using EpollEventDataType = epoll_data_t;
  using EpollEventDataPtr = epoll_data_t *;

 private:
  int epoll_fd;
  bool is_closed;

 public:
  EpollPack() : is_closed(true), epoll_fd(-1) {}

  int GetEpollFd() const { return epoll_fd; }

  bool GetIsClosed() { return is_closed; }

  int EpollCreate() {
    epoll_fd = epoll_create1(0);
    if (epoll_fd > 0)
      is_closed = false;
    else
      perror("epoll_create1");
    return epoll_fd;
  }

  int EpollCtl(int op, int fd, EpollEventPtr event) {
    return epoll_ctl(epoll_fd, op, fd, event);
  }

  int EpollWait(EpollEventPtr events, int max_events, int timeout) {
    return epoll_wait(epoll_fd, events, max_events, timeout);
  }

  void CloseEpoll() {
    if (epoll_fd < 0) return;
    close(epoll_fd);
    epoll_fd = -1;
    is_closed = true;
  }

  static EpollEventType MakeEvent(uint32_t ev_type, EpollEventDataType &data) {
    EpollEventType event;
    event.events = ev_type;
    event.data = data;
    return event;
  }
};

int SetFdNonBlock(int fd);
bool IsNonBlock(int fd);
ssize_t NonBlockRead(int fd, char *buffer, size_t size);
ssize_t NonBlockWrite(int fd, char *data, size_t size);

using EpollEventType = struct epoll_event;
using EpollEventPtr = struct epoll_event *;
using EpollEventDataType = epoll_data_t;
using EpollEventDataPtr = epoll_data_t *;
using EpollPackType = EpollPack;
using EpollPackPtr = EpollPack *;

}  // namespace let_me_see

#endif