#define ZED_NET_STATIC
#define ZED_NET_IMPLEMENTATION

#include "libs/zed_net.h"
#include "libs/atomic/readerwriterqueue.h"
#include <cstdio>
#include <atomic>
#include <future>
#include <string>

//////////////////////////////////////////////////////////////////////////

bool net_is_init = false;

void net_init() {
	if (net_is_init) return;

	zed_net_init();
	net_is_init = true;
}

void net_shutdown() {
	if (!net_is_init) return;

	zed_net_shutdown();
	net_is_init = false;
}


//////////////////////////////////////////////////////////////////////////

typedef struct {
	std::atomic_bool isRunningNet = false;
	std::future<void> sockFuture;

	moodycamel::ReaderWriterQueue<std::string> netMsgQueue;
	moodycamel::ReaderWriterQueue<std::string> netMsgSend;

	zed_net_socket_t socket;
	zed_net_address_t address;

	int init(const char* host, unsigned short port) {
		isRunningNet = false;

		if (zed_net_udp_socket_open(&socket, 0, 0)) {
			printf("Error: %s\n", zed_net_get_error());
			return -1;
		}

		if (zed_net_get_address(&address, host, port) != 0) {
			printf("Error: %s\n", zed_net_get_error());

			zed_net_socket_close(&socket);
			zed_net_shutdown();

			return -1;
		}

		isRunningNet = true;
		return 0;
	}

	void loop() {
		char buffer[1024 * 8];
		sprintf_s(buffer, "Hello");
		buffer[5] = 0;

		int ret = zed_net_udp_socket_send(&socket, address, buffer, (int)strlen(buffer));

		zed_net_address_t sender_address;
		int bytes_read = ret;
		while ((isRunningNet) && (bytes_read >= 0)) {
			buffer[bytes_read] = 0;
			if (bytes_read > 0) {
				netMsgQueue.enqueue(std::string(buffer, bytes_read));
				printf("Received %d bytes from %s:%d: %s\n", bytes_read, zed_net_host_to_str(address.host), address.port, buffer);
			}
			Sleep(1);

			auto p = netMsgSend.peek();
			if (p != nullptr) {
				auto sendMsg = p->c_str();
				zed_net_udp_socket_send(&socket, address, sendMsg, (int)strlen(sendMsg));
				netMsgSend.pop();
			}

			bytes_read = zed_net_udp_socket_receive(&socket, &sender_address, &buffer, sizeof(buffer));
		}
		printf("DONE with ERROR: %s", zed_net_get_error());

		zed_net_socket_close(&socket);
	}

	void stop() {
		if (!isRunningNet)
			return;

		isRunningNet = false;
		sockFuture.wait();

		zed_net_shutdown();
	}

} netThread;

void run_loop(netThread* t) {
	t->loop();
}

netThread* run_client(const char *host, unsigned short port) {
	netThread* newNetThread = new netThread();

	zed_net_init();

	newNetThread->init(host, port);
	newNetThread->sockFuture = std::async(std::launch::async, run_loop, newNetThread);

	return newNetThread;
}
 
#pragma warning (disable : 4996)
bool getMessage(netThread* t, char* buffer, const size_t buflen) {
	auto p = t->netMsgQueue.peek();
	if (p == nullptr)
		return false;

	auto cpyLen = p->copy(buffer, buflen, 0);
	buffer[cpyLen] = 0;

	t->netMsgQueue.pop();
	return true;
}