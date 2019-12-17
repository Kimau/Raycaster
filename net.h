#pragma once

struct netThread;

netThread* run_client(const char *host, unsigned short port);
bool getMessage(netThread* t, char* buffer, const size_t buflen);