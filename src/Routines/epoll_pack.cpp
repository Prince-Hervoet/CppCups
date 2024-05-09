#include "epoll_pack.hpp"

#include <unistd.h>

#include <cstddef>
#include <iostream>

namespace let_me_see {
int SetFdNonBlock(int fd) {
  if (fd < 0) {
    std::cerr << "Error opening file!" << std::endl;
    return -1;
  }
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    std::cerr << "Error getting file flags!" << std::endl;
    close(fd);
    return -1;
  }
  flags |= O_NONBLOCK;
  if (fcntl(fd, F_SETFL, flags) == -1) {
    std::cerr << "Error setting non-blocking mode!" << std::endl;
    close(fd);
    return -1;
  }
  return 1;
}

bool IsNonBlock(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    std::cerr << "Error getting file flags!" << std::endl;
    return false;
  }
  return (flags & O_NONBLOCK) != 0;
}

ssize_t NonBlockRead(int fd, char *buffer, size_t size) {
  if (!IsNonBlock(fd)) {
    if (SetFdNonBlock(fd) < 0) return -1;
  }
  return read(fd, buffer, size);
}

ssize_t NonBlockWrite(int fd, char *data, size_t size) {
  if (!IsNonBlock(fd)) {
    if (SetFdNonBlock(fd) < 0) return -1;
  }
  return write(fd, data, size);
}
}  // namespace let_me_see
