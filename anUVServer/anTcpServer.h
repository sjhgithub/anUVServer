#pragma once
#include "uv.h"
#include <atomic>
#include <thread>
#include <map>
#include <memory>
#include <functional>

class anTcpSocket;
class anMee;
class anMee2;


class anTcpServer
{
public:
	explicit anTcpServer(uv_loop_t *loop);
	~anTcpServer();

	anTcpServer() = delete;
	anTcpServer(const anTcpServer&) = delete;
	anTcpServer& operator=(const anTcpServer&) = delete;
	anTcpServer(anTcpServer&&) = delete;
	anTcpServer& operator=(anTcpServer&&) = delete;
public:
	int start(const char* addr, const unsigned short port);
	int run();
	int wait_exit();

	int push_work(anTcpSocket* socket);
private:
	int init();
	//int shutdown();

	static void on_new_connection(uv_stream_t* server, int status);
	static void alloc_buffer(uv_handle_t * handle, size_t suggested_size, uv_buf_t * buf);
	static void on_write(uv_write_t * req, int status);
	static void on_read(uv_stream_t *socket, ssize_t nread, const uv_buf_t *buf);
	static void on_notify(uv_async_t* handle);
	static void on_close(uv_handle_t* handle);
	static void on_walk(uv_handle_t* handle, void* arg);

	static void on_signal(uv_signal_t* handle, int signum);
	//static void on_shutdown(uv_shutdown_t* req, int status);

	static int get_sessionID() {
		static int sessionID = 0;
		return (++sessionID);
	}

	void clear_session(const int serssionid);
private:
	uv_tcp_t uv_server_ = { 0x00 };
	uv_loop_t * uv_loop_ = {nullptr};

	std::atomic_bool init_ = { ATOMIC_FLAG_INIT };
	std::map<int, anTcpSocket*> client_lists_;

	//std::unique_ptr<anMee> message_handler_;
	std::unique_ptr<anMee2> message_handler2_;

	uv_signal_t sig_;
};

