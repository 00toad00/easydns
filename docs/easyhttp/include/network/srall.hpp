#pragma once

#include <string>

int send_all(int fd, const std::string &data);
int recv_all(int fd, std::string &data);