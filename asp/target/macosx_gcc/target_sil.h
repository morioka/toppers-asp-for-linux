/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2004-2010 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  �嵭����Ԥϡ��ʲ���(1)��(4)�ξ������������˸¤ꡤ�ܥ��եȥ���
 *  �����ܥ��եȥ���������Ѥ�����Τ�ޤࡥ�ʲ�Ʊ���ˤ���ѡ�ʣ������
 *  �ѡ������ۡʰʲ������ѤȸƤ֡ˤ��뤳�Ȥ�̵���ǵ������롥
 *  (1) �ܥ��եȥ������򥽡��������ɤη������Ѥ�����ˤϡ��嵭������
 *      ��ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ��꤬�����Τޤޤη��ǥ���
 *      ����������˴ޤޤ�Ƥ��뤳�ȡ�
 *  (2) �ܥ��եȥ������򡤥饤�֥������ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ�����Ǻ����ۤ�����ˤϡ������ۤ�ȼ���ɥ�����ȡ�����
 *      �ԥޥ˥奢��ʤɡˤˡ��嵭�����ɽ�����������Ѿ�浪��Ӳ���
 *      ��̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *  (3) �ܥ��եȥ������򡤵�����Ȥ߹���ʤɡ�¾�Υ��եȥ�������ȯ�˻�
 *      �ѤǤ��ʤ����Ǻ����ۤ�����ˤϡ����Τ����줫�ξ�����������
 *      �ȡ�
 *    (a) �����ۤ�ȼ���ɥ�����ȡ����Ѽԥޥ˥奢��ʤɡˤˡ��嵭����
 *        �ɽ�����������Ѿ�浪��Ӳ�����̵�ݾڵ����Ǻܤ��뤳�ȡ�
 *    (b) �����ۤη��֤��̤�������ˡ�ˤ�äơ�TOPPERS�ץ��������Ȥ�
 *        ��𤹤뤳�ȡ�
 *  (4) �ܥ��եȥ����������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������뤤���ʤ�»
 *      ������⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ����դ��뤳�ȡ�
 *      �ޤ����ܥ��եȥ������Υ桼���ޤ��ϥ���ɥ桼������Τ����ʤ���
 *      ͳ�˴�Ť����ᤫ��⡤�嵭����Ԥ����TOPPERS�ץ��������Ȥ�
 *      ���դ��뤳�ȡ�
 * 
 *  �ܥ��եȥ������ϡ�̵�ݾڤ��󶡤���Ƥ����ΤǤ��롥�嵭����Ԥ�
 *  ���TOPPERS�ץ��������Ȥϡ��ܥ��եȥ������˴ؤ��ơ�����λ�����Ū
 *  ���Ф���Ŭ������ޤ�ơ������ʤ��ݾڤ�Ԥ�ʤ����ޤ����ܥ��եȥ���
 *  �������Ѥˤ��ľ��Ū�ޤ��ϴ���Ū�������������ʤ�»���˴ؤ��Ƥ⡤��
 *  ����Ǥ�����ʤ���
 * 
 *  @(#) $Id: target_sil.h 1849 2010-07-22 06:34:30Z ertl-hiro $
 */

/*
 *		sil.h�Υ������åȰ�¸����Mac OS X�ѡ�
 *
 *  ���Υ��󥯥롼�ɥե�����ϡ�sil.h����Ƭ�ǥ��󥯥롼�ɤ���롥¾�Υե�
 *  ���뤫���ľ�ܥ��󥯥롼�ɤ��뤳�ȤϤʤ������Υե�����򥤥󥯥롼
 *  �ɤ������ˡ�t_stddef.h�����󥯥롼�ɤ����Τǡ������˰�¸���Ƥ�
 *  �褤��
 */

#ifndef TOPPERS_TARGET_SIL_H
#define TOPPERS_TARGET_SIL_H

#ifndef TOPPERS_MACRO_ONLY

/*
 *  ɸ��Υ��󥯥롼�ɥե�����
 */
#include <signal.h>

/*
 *  NMI��������٤Ƥγ���ߤζػ�
 */
Inline void
TOPPERS_dissig(sigset_t *p_sigmask)
{
	extern sigset_t	_kernel_sigmask_intlock;

	sigprocmask(SIG_BLOCK, &_kernel_sigmask_intlock, p_sigmask);
}

/*
 *  �����ͥ���٥ޥ���������ɽ���ˤθ����ͤ�����
 */
Inline void
TOPPERS_setsig(sigset_t *p_sigmask)
{
	sigprocmask(SIG_SETMASK, p_sigmask, NULL);
}

/*
 *  ������ߥ��å����֤�����
 */
#define SIL_PRE_LOC		sigset_t TOPPERS_sigmask
#define SIL_LOC_INT()	(TOPPERS_dissig(&TOPPERS_sigmask))
#define SIL_UNL_INT()	(TOPPERS_setsig(&TOPPERS_sigmask))

/*
 *  ���������Ԥ�
 */
Inline void
sil_dly_nse(ulong_t dlytim)
{
	/*
	 *  ���ߥ�졼�����Ķ��Ǥϰ�̣���ʤ����ᡤ���⤷�ʤ���
	 */
}

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  �ץ����å��Υ���ǥ�����
 */
#if defined(__ppc__)
#define SIL_ENDIAN_BIG				/* �ӥå�����ǥ����� */
#elif defined(__i386__) || defined(__x86_64__)
#define SIL_ENDIAN_LITTLE			/* ��ȥ륨��ǥ����� */
#endif

#endif /* TOPPERS_TARGET_SIL_H */