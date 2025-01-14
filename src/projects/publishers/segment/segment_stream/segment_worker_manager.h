//==============================================================================
//
//  OvenMediaEngine
//
//  Created by Jaejong Bong
//  Copyright (c) 2019 AirenSoft. All rights reserved.
//
//==============================================================================
#pragma once

#include <base/ovlibrary/ovlibrary.h>
#include <base/ovlibrary/semaphore.h>
#include <modules/http/server/http_connection.h>

#include <queue>
#include <string>

using SegmentProcessHandler = std::function<bool(const std::shared_ptr<http::svr::HttpConnection> &connection)>;
//====================================================================================================
// SegmentWorker
//====================================================================================================
class SegmentWorker
{
public:
	SegmentWorker();
	~SegmentWorker();

	bool Start(const SegmentProcessHandler &process_handler);
	bool Stop();

	bool PushConnection(const std::shared_ptr<http::svr::HttpConnection> &connection);
	std::shared_ptr<http::svr::HttpConnection> PopConnection();

private:
	void WorkerThread();

private:
	std::queue<std::shared_ptr<http::svr::HttpConnection>> _connection_list_to_process;
	std::mutex _work_info_guard;
	ov::Semaphore _queue_event;

	bool _stop_thread_flag;
	std::thread _worker_thread;

	SegmentProcessHandler _process_handler;
};

//====================================================================================================
// SegmentWorkerManager
//====================================================================================================
class SegmentWorkerManager
{
public:
	SegmentWorkerManager() = default;
	~SegmentWorkerManager() = default;

public:
	bool Start(int worker_count, const SegmentProcessHandler &process_handler);
	bool Stop();
	bool PushConnection(const std::shared_ptr<http::svr::HttpConnection> &connection);

private:
	int _worker_index = 0;

	std::vector<std::shared_ptr<SegmentWorker>> _workers;
};