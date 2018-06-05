#ifndef LIB_FIBER_INCLUDE_H
#define LIB_FIBER_INCLUDE_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32) || defined (_WIN64)
# include <winsock2.h>

typedef long ssize_t;

# define	FIBER_ETIMEDOUT		WSAETIMEDOUT
# define	FIBER_ENOMEM		WSAENOBUFS
# define	FIBER_EINVAL		WSAEINVAL
# define	FIBER_ECONNREFUSED	WSAECONNREFUSED
# define	FIBER_ECONNRESET	WSAECONNRESET
# define	FIBER_EHOSTDOWN		WSAEHOSTDOWN
# define	FIBER_EHOSTUNREACH	WSAEHOSTUNREACH
# define	FIBER_EINTR		WSAEINTR
# define	FIBER_ENETDOWN		WSAENETDOWN
# define	FIBER_ENETUNREACH	WSAENETUNREACH
# define	FIBER_ENOTCONN		WSAENOTCONN
# define	FIBER_EISCONN		WSAEISCONN
# define	FIBER_EWOULDBLOCK	WSAEWOULDBLOCK
# define	FIBER_EAGAIN		FIBER_EWOULDBLOCK	/* xxx */
# define	FIBER_ENOBUFS		WSAENOBUFS
# define	FIBER_ECONNABORTED	WSAECONNABORTED
# define	FIBER_EINPROGRESS	WSAEINPROGRESS

#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <poll.h>
#include <sys/select.h>

# define	FIBER_ETIMEDOUT		ETIMEDOUT
# define	FIBER_ENOMEM		ENOMEM
# define	FIBER_EINVAL		EINVAL
# define	FIBER_ECONNREFUSED	ECONNREFUSED
# define	FIBER_ECONNRESET	ECONNRESET
# define	FIBER_EHOSTDOWN		EHOSTDOWN
# define	FIBER_EHOSTUNREACH	EHOSTUNREACH
# define	FIBER_EINTR		EINTR
# define	FIBER_EAGAIN		EAGAIN
# define	FIBER_ENETDOWN		ENETDOWN
# define	FIBER_ENETUNREACH	ENETUNREACH
# define	FIBER_ENOTCONN		ENOTCONN
# define	FIBER_EISCONN		EISCONN
# define	FIBER_EWOULDBLOCK	EWOULDBLOCK
# define	FIBER_ENOBUFS		ENOBUFS
# define	FIBER_ECONNABORTED	ECONNABORTED
# define	FIBER_EINPROGRESS	EINPROGRESS

#endif

#ifdef FIBER_LIB
# ifndef FIBER_API
#  define FIBER_API
# endif
#elif defined(FIBER_DLL) || defined(_WINDLL)
# if defined(FIBER_EXPORTS) || defined(fiber_EXPORTS)
#  ifndef FIBER_API
#   define FIBER_API __declspec(dllexport)
#  endif
# elif !defined(FIBER_API)
#  define FIBER_API __declspec(dllimport)
# endif
#elif !defined(FIBER_API)
# define FIBER_API
#endif

/**
 * Э�̽ṹ����
 */
typedef struct ACL_FIBER ACL_FIBER;

/**
 * �����Ƿ���Ҫ hook ϵͳ�е� IO ��ص� API���ڲ�ȱʡֵΪ 1
 * @param onoff {int} �Ƿ���Ҫ hook
 */
FIBER_API void acl_fiber_hook_api(int onoff);

/**
 * ����һ��Э��
 * @param fn {void (*)(ACL_FIBER*, void*)} Э������ʱ�Ļص�������ַ
 * @param arg {void*} �ص� fn ����ʱ�ĵڶ�������
 * @param size {size_t} ������Э����ռջ�ռ��С
 * @return {ACL_FIBER*}
 */
FIBER_API ACL_FIBER* acl_fiber_create(void (*fn)(ACL_FIBER*, void*),
	void* arg, size_t size);

/**
 * ���ص�ǰ�߳��д�������״̬��Э����
 * @retur {int}
 */
FIBER_API int acl_fiber_ndead(void);

FIBER_API void acl_fiber_check_timer(size_t max);

/**
 * ���ص�ǰ�������е�Э�̶���
 * @retur {ACL_FIBER*} ���� NULL ��ʾ��ǰû���������е�Э��
 */
FIBER_API ACL_FIBER* acl_fiber_running(void);

/**
 * �������Э�̵�Э�� ID ��
 * @param fiber {const ACL_FIBER*} acl_fiber_create ������Э�̶��󣬱���ǿ�
 * @return {unsigned int} Э�� ID ��
 */
FIBER_API unsigned int acl_fiber_id(const ACL_FIBER* fiber);

/**
 * ��õ�ǰ�����е�Э�̵� ID ��
 * @return {unsigned int} ��ǰ����Э�̵� ID ��
 */
FIBER_API unsigned int acl_fiber_self(void);

/**
 * ��������Э�̵Ĵ����
 * @param fiber {ACL_FIBER*} ָ����Э�̶���Ϊ NULL ��ʹ�õ�ǰ���е�Э��
 * @param errnum {int} �����
 */
FIBER_API void acl_fiber_set_errno(ACL_FIBER* fiber, int errnum);

/**
 * ���ָ��Э�̵Ĵ����
 * @param fiber {ACL_FIBER*} ָ����Э�̶�����Ϊ NULL ��ʹ�õ�ǰЭ�̶���
 * @return {int} ����Э�̴����
 */
FIBER_API int acl_fiber_errno(ACL_FIBER* fiber);

/**
 * ��õ�ǰϵͳ���� errno ��
 * @return {int}
 */
FIBER_API int acl_fiber_sys_errno(void);

/**
 * ���õ�ǰϵͳ�� errno ��
 * @param errnum {int}
 */
FIBER_API void acl_fiber_sys_errno_set(int errnum);

/**
 * �Ƿ񱣳���ָ��Э�̵Ĵ���ţ�������Ϊ�����֡������Э�̽����ֵ�ǰ״̬�µ�
 * ����ţ�֮���Э�̵Ĵ���� errno �����ٸı䣬�ߵ��ٴε��ñ�����ȡ������
 * @param fiber {ACL_FIBER*} ָ����Э�̶���Ϊ NULL ��ʹ�õ�ǰ���е�Э��
 * @param yesno {int} �Ƿ񱣳�
 */
FIBER_API void acl_fiber_keep_errno(ACL_FIBER* fiber, int yesno);

/**
 * ���ָ��Э�̵ĵ�ǰ״̬
 * @param fiber {const ACL_FIBER*} ָ����Э�̶���Ϊ NULL ��ʹ�õ�ǰЭ��
 * @return {int} Э��״̬
 */
FIBER_API int acl_fiber_status(const ACL_FIBER* fiber);

/**
 * ֪ͨ��������״̬��Э���˳� 
 * @param fiber {const ACL_FIBER*} ָ����Э�̶��󣬱���� NULL
 */
FIBER_API void acl_fiber_kill(ACL_FIBER* fiber);

/**
 * ��鱾Э���Ƿ�����Э��֪ͨ�˳�
 * @param fiber {const ACL_FIBER*} ָ����Э�̶�����Ϊ NULL ���Զ�ʹ�õ�ǰ
 *  �������е�Э��
 * @return {int} ����ֵΪ 0 ��ʾû�б�֪ͨ�˳����� 0 ��ʾ��֪ͨ�˳�
 */
FIBER_API int acl_fiber_killed(ACL_FIBER* fiber);

/**
 * ������ IO ��ԭ�������ߵ�Э��
 * @param fiber {const ACL_FIBER*} Э�̶��󣬱���� NULL
 * @param signum {int} SIGINT, SIGKILL, SIGTERM ... �ο�ϵͳ�� bits/signum.h
 */
FIBER_API void acl_fiber_signal(ACL_FIBER* fiber, int signum);

/**
 * �������Э�̷��͸�ָ��Э�̵��ź�ֵ
 * @param fiber {const ACL_FIBER*} ָ����Э�̶���Ϊ NULL ʱ��ʹ�õ�ǰЭ��
 * @retur {int} ����ָ��Э���յ����ź�ֵ
 */
FIBER_API int acl_fiber_signum(ACL_FIBER* fiber);

/**
 * ����ǰ���е�Э�̹����ɵ�����ѡ����һ����Ҫ���е�Э��
 * @return {int}
 */
FIBER_API int acl_fiber_yield(void);

/**
 * ��ָ��Э�̶�����������ж�����
 * @param fiber {ACL_FIBER*} ָ��Э�̣������ NULL
 */
FIBER_API void acl_fiber_ready(ACL_FIBER* fiber);

/**
 * ����ǰ���е�Э�̹���ͬʱִ�еȴ�������һ�������е�Э��
 */
FIBER_API void acl_fiber_switch(void);

/**
 * ���õ�ǰ�̵߳�Э�̵������Ƿ�Ϊ������ģʽ��������ģʽ��������Э�̺�����Ƿ���Э��
 * ����״̬�������Э�̵���״̬�����Զ���������������Э�̹��̣��粻���ñ����������ڲ�ȱʡ
 * Ϊ���Զ�����ģʽ��
 */
FIBER_API void acl_fiber_schedule_init(int on);

/**
 * ���ñ���������Э�̵ĵ��ȹ���
 */
FIBER_API void acl_fiber_schedule(void);

/**
 * ����ָ�����¼���������Э�̵��������ڲ�ȱʡ���¼�����Ϊ FIBER_EVENT_KERNEL
 * @param event_mode {int} �¼��������ͣ��μ���FIBER_EVENT_XXX
 */
#define FIBER_EVENT_KERNEL	0	/* epoll/kqueue	*/
#define FIBER_EVENT_POLL	1	/* poll		*/
#define FIBER_EVENT_SELECT	2	/* select	*/
#define FIBER_EVENT_WMSG	3	/* win message	*/
FIBER_API void acl_fiber_schedule_with(int event_mode);

/**
 * ����Э�̵���ʱ���¼���������
 * @param event_mode {int} �¼��������ͣ��μ���FIBER_EVENT_XXX
 */
FIBER_API void acl_fiber_schedule_set_event(int event_mode);

/**
 * ���ñ�������⵱ǰ�߳��Ƿ���Э�̵���״̬
 * @return {int} 0 ��ʾ��Э��״̬���� 0 ��ʾ����Э�̵���״̬
 */
FIBER_API int acl_fiber_scheduled(void);

/**
 * ֹͣЭ�̹���
 */
FIBER_API void acl_fiber_schedule_stop(void);

/**
 * ʹ��ǰ���е�Э������ָ��������
 * @param milliseconds {unsigned int} ָ��Ҫ���ߵĺ�����
 * @return {unsigned int} ��Э�����ߺ��ٴα����Ѻ�ʣ��ĺ�����
 */
FIBER_API unsigned int acl_fiber_delay(unsigned int milliseconds);

/**
 * ʹ��ǰ���е�Э������ָ������
 * @param seconds {unsigned int} ָ��Ҫ���ߵ�����
 * @return {unsigned int} ��Э�����ߺ��ٴα����Ѻ�ʣ�������
 */
FIBER_API unsigned int acl_fiber_sleep(unsigned int seconds);

/**
 * ����һ��Э��������ʱ��
 * @param milliseconds {unsigned int} ��������ʱ�������ѵĺ�����
 * @param size {size_t} ������Э�̵�ջ�ռ��С
 * @param fn {void (*)(ACL_FIBER*, void*)} ��ʱ��Э�̱�����ʱ�Ļص�����
 * @param ctx {void*} �ص� fn ����ʱ�ĵڶ�������
 * @return {ACL_FIBER*} �´����Ķ�ʱ��Э��
 */
FIBER_API ACL_FIBER* acl_fiber_create_timer(unsigned int milliseconds,
	size_t size, void (*fn)(ACL_FIBER*, void*), void* ctx);

/**
 * �ڶ�ʱ��Э��δ������ǰ������ͨ�����������ø�Э�̱����ѵ�ʱ��
 * @param timer {ACL_FIBER*} �� acl_fiber_create_timer �����Ķ�ʱ��Э��
 * @param milliseconds {unsigned int} ָ���ö�ʱ��Э�̱����ѵĺ�����
 */
FIBER_API void acl_fiber_reset_timer(ACL_FIBER* timer, unsigned int milliseconds);

/**
 * ���������� DNS �������ĵ�ַ
 * @param ip {const char*} DNS ������ IP ��ַ
 * @param port {int} DNS �������Ķ˿�
 */
FIBER_API void acl_fiber_set_dns(const char* ip, int port);

/* for fiber specific */

/**
 * �趨��ǰЭ�̵ľֲ�����
 * @param key {int*} Э�ֲ̾��������������ĵ�ַ����ʼʱ��ֵӦ <= 0���ڲ����Զ�
 *  ����һ�� > 0 ���������������õ�ַ��ֵ�������Э�̿��Ը��ø�ֵ���ø��Ե�
 *  �ֲ���������ָ������ NULL
 * @param ctx {void *} Э�ֲ̾�����
 * @param free_fn {void (*)(void*)} ��Э���˳�ʱ����ô˺����ͷ�Э�ֲ̾�����
 * @return {int} ���������õ�Э�ֲ̾������ļ�ֵ������ -1 ��ʾ��ǰЭ�̲�����
 */
FIBER_API int acl_fiber_set_specific(int* key, void* ctx, void (*free_fn)(void*));

/**
 * ��õ�ǰЭ�ֲ̾�����
 * @param key {int} �� acl_fiber_set_specific ���صļ�ֵ
 * @retur {void*} ���� NULL ��ʾ������
 */
FIBER_API void* acl_fiber_get_specific(int key);

/* fiber locking */

/**
 * Э�̻��������̷߳ǰ�ȫ��ֻ������ͬһ�߳���
 */
typedef struct ACL_FIBER_MUTEX ACL_FIBER_MUTEX;

/**
 * Э�̶�д�����̷߳ǰ�ȫ��ֻ������ͬһ�߳���
 */
typedef struct ACL_FIBER_RWLOCK ACL_FIBER_RWLOCK;

/**
 * ����Э�̻��������̷߳ǰ�ȫ��ֻ������ͬһ�߳���
 * @return {ACL_FIBER_MUTEX*}
 */
FIBER_API ACL_FIBER_MUTEX* acl_fiber_mutex_create(void);

/**
 * �ͷ�Э�̻�����
 * @param l {ACL_FIBER_MUTEX*} �� acl_fiber_mutex_create ������Э�̻�����
 */
FIBER_API void acl_fiber_mutex_free(ACL_FIBER_MUTEX* l);

/**
 * ��Э�̻�������������ʽ��������������ɹ��򷵻أ�����������
 * @param l {ACL_FIBER_MUTEX*} �� acl_fiber_mutex_create ������Э�̻�����
 */
FIBER_API void acl_fiber_mutex_lock(ACL_FIBER_MUTEX* l);

/**
 * ��Э�̻����������Խ��м����������Ƿ�ɹ�����������������
 * @param l {ACL_FIBER_MUTEX*} �� acl_fiber_mutex_create ������Э�̻�����
 * @return {int} ��������ɹ��򷵻� 0 ֵ�����򷵻� -1
 */
FIBER_API int acl_fiber_mutex_trylock(ACL_FIBER_MUTEX* l);

/**
 * �����ɹ���Э�̵��ñ��������н��������ñ�������Э�̱����Ǹ���������������
 * �ڲ����������
 * @param l {ACL_FIBER_MUTEX*} �� acl_fiber_mutex_create ������Э�̻�����
 */
FIBER_API void acl_fiber_mutex_unlock(ACL_FIBER_MUTEX* l);

/**
 * ����Э�̶�д�����̷߳ǰ�ȫ��ֻ������ͬһ�߳���
 * @return {ACL_FIBER_RWLOCK*}
 */
FIBER_API ACL_FIBER_RWLOCK* acl_fiber_rwlock_create(void);

/**
 * �ͷ�Э�̶�д��
 * @param l {ACL_FIBER_RWLOCK*} �� acl_fiber_rwlock_create �����Ķ�д��
 */
FIBER_API void acl_fiber_rwlock_free(ACL_FIBER_RWLOCK* l);

/**
 * ��Э�̶�д���Ӷ��������������ǰ��������Э�̼��˶�������Э����Ȼ����
 * �����Ӷ��������������ǰ��������Э�̼���д������Э�̽�������״̬��ֱ��
 * д���ͷ�
 * @param l {ACL_FIBER_RWLOCK*} �� acl_fiber_rwlock_create �����Ķ�д��
 */
FIBER_API void acl_fiber_rwlock_rlock(ACL_FIBER_RWLOCK* l);

/**
 * ��Э�̶�д�������ԼӶ��������������Ƿ�ɹ�������������
 * @param l {ACL_FIBER_RWLOCK*} �� acl_fiber_rwlock_create �����Ķ�д��
 * @retur {int} ���� 1 ��ʾ�����ɹ������� 0 ��ʾ����ʧ��
 */
FIBER_API int acl_fiber_rwlock_tryrlock(ACL_FIBER_RWLOCK* l);

/**
 * ��Э�̶�д����д����ֻ�е�����δ���κ�Э�̼Ӷ�/д��ʱ�Ż᷵�أ�����������
 * ֱ�������ɼ�д��
 * @param l {ACL_FIBER_RWLOCK*} �� acl_fiber_rwlock_create �����Ķ�д��
 */
FIBER_API void acl_fiber_rwlock_wlock(ACL_FIBER_RWLOCK* l);

/**
 * ��Э�̶�д�������Լ�д���������Ƿ�����ɹ�������������
 * @param l {ACL_FIBER_RWLOCK*} �� acl_fiber_rwlock_create �����Ķ�д��
 * @return {int} ���� 1 ��ʾ��д���ɹ������� 0 ��ʾ����ʧ��
 */
FIBER_API int acl_fiber_rwlock_trywlock(ACL_FIBER_RWLOCK* l);

/**
 * ��Э�̶�д���ɹ��Ӷ�����Э�̵��ñ�����������������߱�����֮ǰ�ѳɹ��Ӷ�
 * ���ɹ���Э��
 * @param l {ACL_FIBER_RWLOCK*} �� acl_fiber_rwlock_create �����Ķ�д��
 */
FIBER_API void acl_fiber_rwlock_runlock(ACL_FIBER_RWLOCK* l);
/**
 * ��Э�̶�д���ɹ���д����Э�̵��ñ�������д���������߱�����֮ǰ�ѳɹ���д
 * ���ɹ���Э��
 * @param l {ACL_FIBER_RWLOCK*} �� acl_fiber_rwlock_create �����Ķ�д��
 */
FIBER_API void acl_fiber_rwlock_wunlock(ACL_FIBER_RWLOCK* l);

/* fiber_event.c */

/* �̰߳�ȫ��Э�������������ڲ�ͬ�̵߳�Э��֮�估��ͬ�߳�֮��Ļ��� */
typedef struct ACL_FIBER_EVENT ACL_FIBER_EVENT;

/**
 * ���������¼���Э��/�̻߳����
 * @return {ACL_FIBER_EVENT *}
 */
FIBER_API ACL_FIBER_EVENT *acl_fiber_event_create(void);

/**
 * �ͷ��¼���
 * @param {ACL_FIBER_EVENT *}
 */
FIBER_API void acl_fiber_event_free(ACL_FIBER_EVENT *event);

/**
 * �ȴ��¼�������
 * @param {ACL_FIBER_EVENT *}
 * @return {int} ���� 0 ��ʾ�ɹ���-1 ��ʾ����
 */
FIBER_API int acl_fiber_event_wait(ACL_FIBER_EVENT *event);

/**
 * ���Եȴ��¼�������
 * @param {ACL_FIBER_EVENT *}
 * @return {int} ���� 0 ��ʾ�ɹ���-1 ��ʾ����ռ��
 */
FIBER_API int acl_fiber_event_trywait(ACL_FIBER_EVENT *event);

/**
 * �¼���ӵ����֪ͨ�ȴ����¼������ã���ȴ����յ�֪ͨ����ɻ���¼���
 * @param {ACL_FIBER_EVENT *}
 * @return {int} ���� 0 ��ʾ�ɹ���-1 ��ʾ����
 */
FIBER_API int acl_fiber_event_notify(ACL_FIBER_EVENT *event);

/* fiber semaphore */

typedef struct ACL_FIBER_SEM ACL_FIBER_SEM;

/**
 * ����Э���ź�����ͬʱ�ڲ��Ὣ��ǰ�߳�����ź�����
 * @param num {int} �ź�����ʼֵ������ >= 0��
 * @return {ACL_FIBER_SEM *}
 */
FIBER_API ACL_FIBER_SEM* acl_fiber_sem_create(int num);

/**
 * �ͷ�Э���ź���
 * @param {ACL_FIBER_SEM *}
 */
FIBER_API void acl_fiber_sem_free(ACL_FIBER_SEM* sem);

/**
 * ��õ�ǰЭ���ź������󶨵��߳� ID
 * @param sem {ACL_FIBER_SEM*} Э���ź�������
 * @return {unsigned long}
 */
#if !defined(_WIN32) && !defined(_WIN64)
FIBER_API unsigned long acl_fiber_sem_get_tid(ACL_FIBER_SEM* sem);
#endif

/**
 * ����ָ��Э���ź����ĵ��߳� ID�����ı䱾Э���ź����������߳�ʱ����ȴ���Э��
 * ���ݷ� 0 ���ڲ��Զ� fatal������Э���ź����ϵȴ�Э�̷ǿ�ʱ��ֹ���ñ�����
 * @param sem {ACL_FIBER_SEM*} Э���ź�������
 * @param {unsigned long} �߳� ID
 */
FIBER_API void acl_fiber_sem_set_tid(ACL_FIBER_SEM* sem, unsigned long tid);

/**
 * ��Э���ź��� > 0 ʱʹ�ź����� 1������ȴ��ź��� > 0
 * @param sem {ACL_FIBER_SEM *}
 * @retur {int} �����ź�����ǰֵ��������� -1 ������ǰ�߳���Э���ź��������߳�
 *  ����ͬһ�̣߳���ʱ�÷������ȴ���������
 */
FIBER_API int acl_fiber_sem_wait(ACL_FIBER_SEM* sem);

/**
 * ����ʹЭ���ź����� 1
 * @param sem {ACL_FIBER_SEM *}
 * @retur {int} �ɹ��� 1 ʱ����ֵ >= 0������ -1 ��ʾ��ǰ�ź��������ã���ǰ
 *  �������߳���Э���ź��������̲߳���ͬһ�߳�
 */
FIBER_API int acl_fiber_sem_trywait(ACL_FIBER_SEM* sem);

/**
 * ʹЭ���ź����� 1
 * @param sem {ACL_FIBER_SEM *}
 * @retur {int} �����ź�����ǰֵ������ -1 ��ʾ��ǰ�������߳���Э���ź�������
 *  �̲߳���ͬһ�߳�
 */
FIBER_API int acl_fiber_sem_post(ACL_FIBER_SEM* sem);

/**
 * ���ָ��Э���ź����ĵ�ǰֵ����ֵ��ӳ��Ŀǰ�ȴ����ź���������
 * @param sem {ACL_FIBER_SEM*}
 * @retur {int}
 */
FIBER_API int acl_fiber_sem_num(ACL_FIBER_SEM* sem);

/* channel communication */

/**
 * Э�̼�ͨ�ŵĹܵ�
 */
typedef struct ACL_CHANNEL ACL_CHANNEL;

/**
 * ����Э��ͨ�Źܵ�
 * @param elemsize {int} �� ACL_CHANNEL ���д���Ķ���Ĺ̶��ߴ��С���ֽڣ�
 * @param bufsize {int} ACL_CHANNEL �ڲ���������С�������Ի��� elemsize �ߴ��С
 *  ����ĸ���
 * @return {CHANNNEL*}
 */
FIBER_API ACL_CHANNEL* acl_channel_create(int elemsize, int bufsize);

/**
 * �ͷ��� acl_channel_create ������Э��ͨ�Źܵ�����
 * @param c {ACL_CHANNEL*} �� acl_channel_create �����Ĺܵ�����
 */
FIBER_API void acl_channel_free(ACL_CHANNEL* c);

/**
 * ����ʽ��ָ�� ACL_CHANNEL �з���ָ���Ķ����ַ
 * @param c {ACL_CHANNEL*} �� acl_channel_create �����Ĺܵ�����
 * @param v {void*} �����͵Ķ����ַ
 * @return {int} ����ֵ >= 0
 */
FIBER_API int acl_channel_send(ACL_CHANNEL* c, void* v);

/**
 * ������ʽ��ָ�� ACL_CHANNEL �з���ָ���Ķ����ڲ������ acl_channel_create ��ָ��
 * �� elemsize �����С�������ݿ���
 * @param c {ACL_CHANNEL*} �� acl_channel_create �����Ĺܵ�����
 * @param v {void*} �����͵Ķ����ַ
 */
FIBER_API int acl_channel_send_nb(ACL_CHANNEL* c, void* v);

/**
 * ��ָ���� ACL_CHANNEL ������ʽ��ȡ����
 * @param c {ACL_CHANNEL*} �� acl_channel_create �����Ĺܵ�����
 * @param v {void*} ��Ž������
 * @return {int} ����ֵ >= 0 ��ʾ�ɹ���������
 */
FIBER_API int acl_channel_recv(ACL_CHANNEL* c, void* v);

/**
 * ��ָ���� ACL_CHANNEL �з�����ʽ��ȡ���������Ƿ�������ݶ�����������
 * @param c {ACL_CHANNEL*} �� acl_channel_create �����Ĺܵ�����
 * @param v {void*} ��Ž������
 * @return {int} ����ֵ >= 0 ��ʾ�ɹ��������ݣ������ʾδ��������
 */
FIBER_API int acl_channel_recv_nb(ACL_CHANNEL* c, void* v);

/**
 * ��ָ���� ACL_CHANNEL ������ʽ����ָ������ĵ�ַ
 * @param c {ACL_CHANNEL*} �� acl_channel_create �����Ĺܵ�����
 * @param v {void*} �����Ͷ���ĵ�ַ
 * @return {int} ����ֵ >= 0
 */
FIBER_API int acl_channel_sendp(ACL_CHANNEL* c, void* v);

/**
 * ��ָ���� CHANNLE ������ʽ������ acl_channel_sendp ���͵Ķ���ĵ�ַ
 * @param c {ACL_CHANNEL*} �� acl_channel_create �����Ĺܵ�����
 * @return {void*} ���ط� NULL��ָ�����յ��Ķ���ĵ�ַ
 */
FIBER_API void *acl_channel_recvp(ACL_CHANNEL* c);

/**
 * ��ָ���� ACL_CHANNEL �з�����ʽ����ָ������ĵ�ַ
 * @param c {ACL_CHANNEL*} �� acl_channel_create �����Ĺܵ�����
 * @param v {void*} �����Ͷ���ĵ�ַ
 * @return {int} ����ֵ >= 0
 */
FIBER_API int acl_channel_sendp_nb(ACL_CHANNEL* c, void* v);

/**
 * ��ָ���� CHANNLE ������ʽ������ acl_channel_sendp ���͵Ķ���ĵ�ַ
 * @param c {ACL_CHANNEL*} �� acl_channel_create �����Ĺܵ�����
 * @return {void*} ���ط� NULL��ָ�����յ��Ķ���ĵ�ַ��������� NULL ��ʾ
 *  û�ж����κζ���
 */
FIBER_API void *acl_channel_recvp_nb(ACL_CHANNEL* c);

/**
 * ��ָ���� ACL_CHANNEL �з����޷��ų�������ֵ
 * @param c {ACL_CHANNEL*} �� acl_channel_create �����Ĺܵ�����
 * @param val {unsigned long} Ҫ���͵���ֵ
 * @return {int} ����ֵ >= 0
 */
FIBER_API int acl_channel_sendul(ACL_CHANNEL* c, unsigned long val);

/**
 * ��ָ���� ACL_CHANNEL �н����޷��ų�������ֵ
 * @param c {ACL_CHANNEL*} �� acl_channel_create �����Ĺܵ�����
 * @return {unsigned long}
 */
FIBER_API unsigned long acl_channel_recvul(ACL_CHANNEL* c);

/**
 * ��ָ���� ACL_CHANNEL ���Է�������ʽ�����޷��ų�������ֵ
 * @param c {ACL_CHANNEL*} �� acl_channel_create �����Ĺܵ�����
 * @param val {unsigned long} Ҫ���͵���ֵ
 * @return {int} ����ֵ >= 0
 */
FIBER_API int acl_channel_sendul_nb(ACL_CHANNEL* c, unsigned long val);

/**
 * ��ָ���� ACL_CHANNEL ���Է�������ʽ�����޷��ų�������ֵ
 * @param c {ACL_CHANNEL*} �� acl_channel_create �����Ĺܵ�����
 * @return {unsigned long}
 */
FIBER_API unsigned long acl_channel_recvul_nb(ACL_CHANNEL* c);

/**************************** fiber wrapper *********************************/
#if defined(_WIN32) || defined(_WIN64)
typedef SOCKET socket_t;
typedef int socklen_t;
#else
typedef int socket_t;
#define INVALID_SOCKET	-1
#endif

FIBER_API socket_t acl_fiber_socket(int domain, int type, int protocol);
FIBER_API int acl_fiber_listen(socket_t, int backlog);

#if defined(_WIN32) || defined(_WIN64)
FIBER_API int WINAPI acl_fiber_close(socket_t fd);
FIBER_API socket_t WINAPI acl_fiber_accept(
	socket_t, struct sockaddr *, socklen_t *);
FIBER_API int WINAPI acl_fiber_connect(
	socket_t , const struct sockaddr *, socklen_t );

FIBER_API int WINAPI acl_fiber_recv(
	socket_t, char* buf, int len, int flags);
FIBER_API int WINAPI acl_fiber_recvfrom(socket_t, char* buf, size_t len,
	int flags, struct sockaddr* src_addr, socklen_t* addrlen);

FIBER_API int WINAPI acl_fiber_send(socket_t, const char* buf,
	int len, int flags);
FIBER_API int WINAPI acl_fiber_sendto(socket_t, const char* buf, size_t len,
	int flags, const struct sockaddr* dest_addr, socklen_t addrlen);

FIBER_API int WINAPI acl_fiber_select(int nfds, fd_set *readfds,
	fd_set *writefds, fd_set *exceptfds, const struct timeval *timeout);
FIBER_API int WINAPI acl_fiber_poll(struct pollfd *fds,
	unsigned long nfds, int timeout);
#else
FIBER_API int acl_fiber_close(socket_t fd);
FIBER_API socket_t acl_fiber_accept(socket_t , struct sockaddr *, socklen_t *);
FIBER_API int acl_fiber_connect(socket_t , const struct sockaddr *, socklen_t );
FIBER_API ssize_t acl_fiber_read(socket_t, void* buf, size_t count);
FIBER_API ssize_t acl_fiber_readv(socket_t, const struct iovec* iov, int iovcnt);
FIBER_API ssize_t acl_fiber_recvmsg(socket_t, struct msghdr* msg, int flags);

FIBER_API ssize_t acl_fiber_write(socket_t, const void* buf, size_t count);
FIBER_API ssize_t acl_fiber_writev(socket_t, const struct iovec* iov, int iovcnt);
FIBER_API ssize_t acl_fiber_sendmsg(socket_t, const struct msghdr* msg, int flags);

FIBER_API ssize_t acl_fiber_recv(socket_t, void* buf, size_t len, int flags);
FIBER_API ssize_t acl_fiber_recvfrom(socket_t, void* buf, size_t len, int flags,
	struct sockaddr* src_addr, socklen_t* addrlen);

FIBER_API ssize_t acl_fiber_send(socket_t, const void* buf, size_t len, int flags);
FIBER_API ssize_t acl_fiber_sendto(socket_t, const void* buf, size_t len, int flags,
	const struct sockaddr* dest_addr, socklen_t addrlen);

FIBER_API int acl_fiber_select(int nfds, fd_set *readfds, fd_set *writefds,
	fd_set *exceptfds, struct timeval *timeout);
FIBER_API int acl_fiber_poll(struct pollfd *fds, nfds_t nfds, int timeout);
#endif

/****************************************************************************/

/**
 * �ڽ�д��־����־�ļ�ǰ�ص��û��Զ���ĺ������ҽ���־��Ϣ���ݸ��ú�����
 * ֻ�е��û�ͨ�� msg_pre_write �������ú����Ч
 * @param ctx {void*} �û����Զ������
 * @param fmt {const char*} ��ʽ����
 * @param ap {va_list} ��ʽ�����б�
 */
typedef void (*FIBER_MSG_PRE_WRITE_FN)(void *ctx, const char *fmt, va_list ap);

/**
 * Ӧ��ͨ���˺������Ϳ����Զ�����־��¼��������Ӧ���ڴ���־ǰ����
 * msg_register ע�����Զ����¼��������Ӧ��д��־ʱ���ô��Զ���
 * ������¼��־��������ȱʡ����־��¼����
 * @param ctx {void*} Ӧ�ô��ݽ�ȥ�Ĳ���
 * @param fmt {const char*} ��ʽ����
 * @param ap {va_list} �����б�
 */
typedef void (*FIBER_MSG_WRITE_FN) (void *ctx, const char *fmt, va_list ap);

/**
 * �ڴ���־ǰ���ô˺���ע��Ӧ���Լ�����־��¼����
 * @param write_fn {MSG_WRITE_FN} �Զ�����־��¼����
 * @param ctx {void*} �Զ������
 */
FIBER_API void acl_fiber_msg_register(FIBER_MSG_WRITE_FN write_fn, void *ctx);

/**
 * �� msg_register ע���Զ��庯�����������ȱʡ����־������
 */
FIBER_API void acl_fiber_msg_unregister(void);

/**
 * �ڴ���־ǰ���ô˺���ע��Ӧ�õ�˽�к������ڼ�¼��־ǰ���ȼ�¼��Ϣͨ��
 * ��ע��ĺ������ݸ�Ӧ��
 * @param pre_write {MSG_PRE_WRITE_FN} ��־��¼ǰ���õĺ���
 * @param ctx {void*} �Զ������
 */
FIBER_API void acl_fiber_msg_pre_write(FIBER_MSG_PRE_WRITE_FN pre_write, void *ctx);

/**
 * ��δ���� msg_open ��ʽ����־ʱ�������� msg_info/error/fatal/warn
 * �Ĳ������Ƿ�������Ϣ�������׼�����Ļ�ϣ�ͨ���˺��������øÿ��أ��ÿ���
 * ��Ӱ���Ƿ���Ҫ����Ϣ������ն���Ļ����Ӱ���Ƿ�������ļ���
 * @param onoff {int} �� 0 ��ʾ�����������Ļ
 */
FIBER_API void acl_fiber_msg_stdout_enable(int onoff);

/**
 * ����ϴ�ϵͳ���ó���ʱ�Ĵ����
 * @return {int} �����
 */
FIBER_API int acl_fiber_last_error(void);

/**
 * �ֹ����ô����
 * @param errnum {int} �����
 */
void acl_fiber_set_error(int errnum);

/****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif
