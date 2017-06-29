// http://stackoverflow.com/questions/5419356/redirect-stdout-stderr-to-a-string

#include "StdCapture.hpp"

#include <thread>
#include <errno.h>
#include <stdio.h>

#define STD_OUT_FD (_fileno(stdout))
#define STD_ERR_FD (_fileno(stderr))

void StdCaptureStatic::InitNoLock()
{
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	initialized = true;
}

void StdCaptureStatic::Init()
{
	// make stdout & stderr streams unbuffered
	// so that we don't need to flush the streams
	// before capture and after capture 
	// (fflush can cause a deadlock if the stream is currently being 
	std::lock_guard<std::mutex> lock(m_mutex);
	InitNoLock();
}

void StdCaptureStatic::BeginCapture()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (!initialized)
		InitNoLock();
	if (m_capturing)
		return;

	secure_pipe(m_pipe);
	m_oldStdOut = secure_dup(STD_OUT_FD);
	m_oldStdErr = secure_dup(STD_ERR_FD);
	secure_dup2(m_pipe[WRITE], STD_OUT_FD);
	secure_dup2(m_pipe[WRITE], STD_ERR_FD);
	m_capturing = true;
#ifndef _MSC_VER
	secure_close(m_pipe[WRITE]);
#endif
}

bool StdCaptureStatic::IsCapturing()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_capturing;
}

void StdCaptureStatic::EndCapture()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (!m_capturing)
		return;

	m_captured.clear();
    m_captured_err.clear();
	secure_dup2(m_oldStdOut, STD_OUT_FD);
	secure_dup2(m_oldStdErr, STD_ERR_FD);

	const int bufSize = 1025;
	unsigned char buf[bufSize];
	int bytesRead = 0;
	bool fd_blocked(false);
	do
	{
		bytesRead = 0;
		fd_blocked = false;
#ifdef _MSC_VER
		if (!eof(m_pipe[READ]))
			bytesRead = read(m_pipe[READ], buf, bufSize - 1);
#else
		bytesRead = read(m_pipe[READ], buf, bufSize - 1);
#endif
        
		if (bytesRead > 0) {
            for(int i = 0; i < bytesRead; ++i)
                m_captured.push_back(buf[i]);
		}
		else if (bytesRead < 0) {
			fd_blocked = (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR);
			if (fd_blocked)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	} while (fd_blocked || bytesRead == (bufSize - 1));

	secure_close(m_oldStdOut);
	secure_close(m_oldStdErr);
	secure_close(m_pipe[READ]);
#ifdef _MSC_VER
	secure_close(m_pipe[WRITE]);
#endif
	m_capturing = false;
}

void StdCaptureStatic::GetCapture(std::vector<unsigned char>& out, std::vector<unsigned char>& err)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	out = m_captured;
    err = m_captured_err;
}

int StdCaptureStatic::secure_dup(int src)
{
	int ret = -1;
	bool fd_blocked = false;
	do
	{
		ret = dup(src);
		fd_blocked = (errno == EINTR || errno == EBUSY);
		if (fd_blocked)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	} while (ret < 0);
	return ret;
}

void StdCaptureStatic::secure_pipe(int * pipes)
{
	int ret = -1;
	bool fd_blocked = false;
	do
	{
#ifdef _MSC_VER
		ret = pipe(pipes, 65536, O_BINARY);
#else
		ret = pipe(pipes) == -1;
#endif
		fd_blocked = (errno == EINTR || errno == EBUSY);
		if (fd_blocked)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	} while (ret < 0);
}
void StdCaptureStatic::secure_dup2(int src, int dest)
{
	int ret = -1;
	bool fd_blocked = false;
	do
	{
		ret = dup2(src, dest);
		fd_blocked = (errno == EINTR || errno == EBUSY);
		if (fd_blocked)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	} while (ret < 0);
}

void StdCaptureStatic::secure_close(int & fd)
{
	int ret = -1;
	bool fd_blocked = false;
	do
	{
		ret = close(fd);
		fd_blocked = (errno == EINTR);
		if (fd_blocked)
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
	} while (ret < 0);

	fd = -1;
}

// actually define vars.
int StdCaptureStatic::m_pipe[2];
int StdCaptureStatic::m_oldStdOut;
int StdCaptureStatic::m_oldStdErr;
bool StdCaptureStatic::m_capturing;
std::mutex StdCaptureStatic::m_mutex;
std::vector<unsigned char> StdCaptureStatic::m_captured;
std::vector<unsigned char> StdCaptureStatic::m_captured_err;
bool StdCaptureStatic::initialized = false;