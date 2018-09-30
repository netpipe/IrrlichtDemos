
#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <string>

struct ServerConfig {
  ServerConfig() : port(6667), priority(5), ssl(false), ssl_verify(true), nickName("ming"), userName("ming"),
      fullName("ming"), autoConnect(false), connectTimeout(10), pongTimeout(30) {}

  std::string toString() const;

  std::string host;
  uint16_t port;
  uint8_t priority;
  bool ssl;
  bool ssl_verify;

  std::string name;
  std::string channel;
  std::string displayName;
  std::string nickName;
  std::string userName;
  std::string fullName;
  std::string password;

  bool autoConnect;
  time_t connectTimeout;
  time_t pongTimeout;
};

#endif
