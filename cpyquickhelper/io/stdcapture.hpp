#pragma once

// http://stackoverflow.com/questions/5419356/redirect-stdout-stderr-to-a-string

#ifdef _MSC_VER
#include <io.h>
#define popen _popen 
#define pclose _pclose
#define stat _stat 
#define dup _dup
#define dup2 _dup2
#define fileno _fileno
#define close _close
#define pipe _pipe
#define read _read
#define eof _eof
#else
#include <unistd.h>
#endif

#include <fcntl.h>
#include <stdio.h>
#include <mutex>
#include <vector>

class StdCaptureStatic
{
public:
	static void Init();

	static void BeginCapture();
	static bool IsCapturing();
	static void EndCapture();
	static void GetCapture(std::vector<unsigned char>& out, std::vector<unsigned char>& err);
private:
	enum PIPES { READ, WRITE };

	static void InitNoLock();
	static int secure_dup(int src);
	static void secure_pipe(int * pipes);
	static void secure_dup2(int src, int dest);

	static void secure_close(int & fd);

	static int m_pipe[2];
	static int m_oldStdOut;
	static int m_oldStdErr;
	static bool m_capturing;
	static std::mutex m_mutex;
	static std::vector<unsigned char> m_captured;
	static std::vector<unsigned char> m_captured_err;
	static bool initialized;
};
