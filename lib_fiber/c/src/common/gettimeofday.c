#include "stdafx.h"
#include "pthread_patch.h"
#include "msg.h"
#include "init.h"
#include "gettimeofday.h"

#ifdef SYS_WIN

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <time.h>

struct timezone {
	int tz_minuteswest; /**< minutes W of Greenwich */
	int tz_dsttime;     /**< type of dst correction */
};

# ifndef __GNUC__
#   define EPOCHFILETIME (116444736000000000i64)
# else
#  define EPOCHFILETIME (116444736000000000LL)
# endif

static void dummy(void *ptr fiber_unused)
{
}

static void free_tls(void *ptr)
{
	free(ptr);
}

static void *__tls = NULL;
static void main_free_tls(void)
{
	if (__tls) {
		free(__tls);
		__tls = NULL;
	}
}

static pthread_key_t  once_key;
static void once_init(void)
{
	if (__pthread_self() == main_thread_self()) {
		pthread_key_create(&once_key, dummy);
		atexit(main_free_tls);
	} else
		pthread_key_create(&once_key, free_tls);
}

static pthread_once_t once_control = PTHREAD_ONCE_INIT;
static void *tls_calloc(size_t len)
{
	void *ptr;

	(void) pthread_once(&once_control, once_init);
	ptr = (void*) pthread_getspecific(once_key);
	if (ptr == NULL) {
		ptr = calloc(1, len);
		pthread_setspecific(once_key, ptr);
		if (__pthread_self() == main_thread_self())
			__tls = ptr;
	}
	return ptr;
}

typedef struct {
	time_t last_init;
	struct timeval tvbase;
	LARGE_INTEGER frequency;
	LARGE_INTEGER stamp;
	int tzflag;
} TIME_CTX_T;

int gettimeofday(struct timeval *tv, struct timezone *tz)
{
	FILETIME        ft;
	LARGE_INTEGER   li;
	__int64         t;
	int             nnested = 0;
	LARGE_INTEGER stamp;
	time_t now;
	TIME_CTX_T *ctx = tls_calloc(sizeof(TIME_CTX_T));

	/* ÿ���̵߳��ô˺���ʱ����Ҫ���г�ʼ������Ϊ�˷�ֹ����ʱ��̫��
	 * �����ʱ�Ӽ����������������ÿ�� 1 ��У��һ�λ�׼ʱ��
	 */
#define DAY_SEC	(3600 * 24)

	time(&now);
	if (now - ctx->last_init > DAY_SEC) {
		ctx->last_init = now;

		/* ���CPU��ʱ��Ƶ�� */
		if (!QueryPerformanceFrequency(&ctx->frequency))
			msg_fatal("%s(%d): Unable to get System Frequency(%s)",
				__FILE__, __LINE__, last_serror());
		/* ���ϵͳʱ��(�� 1970 ����) */
		GetSystemTimeAsFileTime(&ft);
		li.LowPart  = ft.dwLowDateTime;
		li.HighPart = ft.dwHighDateTime;
		t  = li.QuadPart;       /* In 100-nanosecond intervals */
		t -= EPOCHFILETIME;     /* Offset to the Epoch time */
		t /= 10;                /* In microseconds */

		/* ת���ɱ��ο�����Ļ�׼ʱ�� */
		ctx->tvbase.tv_sec  = (long)(t / 1000000);
		ctx->tvbase.tv_usec = (long)(t % 1000000);

		/* ��ñ��ο��������ڵ�ʱ�Ӽ��� */
		if (!QueryPerformanceCounter(&ctx->stamp))
			msg_fatal("%s(%d): unable to get System time(%s)",
				__FILE__, __LINE__, last_serror());
	}

	/* ��ʼ������ڵ�ʱ��� */

	if (tv) {
		/* ��ñ��ο����������ڵ�ʱ�Ӽ���  */
		if (!QueryPerformanceCounter(&stamp))
			msg_fatal("%s(%d): unable to get System time(%s)",
				__FILE__, __LINE__, last_serror());

		/* ���㵱ǰ��ȷʱ��� */
		t = (stamp.QuadPart - ctx->stamp.QuadPart) * 1000000 / ctx->frequency.QuadPart;
		tv->tv_sec = ctx->tvbase.tv_sec + (long)(t / 1000000);
		tv->tv_usec = ctx->tvbase.tv_usec + (long)(t % 1000000);
	}

	if (tz) {
		if (!ctx->tzflag) {
			_tzset();
			ctx->tzflag++;
		}
		tz->tz_minuteswest = _timezone / 60;
		tz->tz_dsttime = _daylight;
	}

	return (0);
}

#endif
