#pragma once
#include "fiber_cpp_define.hpp"

#if !defined(_WIN32) && !defined(_WIN64)

struct ACL_FIBER_EVENT;

namespace acl {

/**
 * ������Э��֮�䡢�߳�֮���Լ�Э�����߳�֮�䣬ͨ���¼��ȴ�/֪ͨ��ʽ����ͬ����
 * ���¼������
 */
class FIBER_CPP_API fiber_event
{
public:
	fiber_event(void);
	~fiber_event(void);

	/**
	 * �ȴ��¼���
	 * @return {bool} ���� true ��ʾ�����ɹ��������ʾ�ڲ�����
	 */
	bool wait(void);

	/**
	 * ���Եȴ��¼���
	 * @return {bool} ���� true ��ʾ�����ɹ��������ʾ�����ڱ�ռ��
	 */
	bool trywait(void);

	/**
	 * �¼���ӵ�����ͷ��¼�����֪ͨ�ȴ���
	 * @return {bool} ���� true ��ʾ֪ͨ�ɹ��������ʾ�ڲ�����
	 */
	bool notify(void);

private:
	ACL_FIBER_EVENT* event_;
};

} // namespace acl

#endif

