/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2014 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: dataqueue.c 2639 2014-04-16 23:04:23Z ertl-hiro $
 */

/*
 *		�ǡ������塼��ǽ
 */

#include "kernel_impl.h"
#include "check.h"
#include "task.h"
#include "wait.h"
#include "dataqueue.h"

/*
 *  �ȥ졼�������ޥ����Υǥե�������
 */
#ifndef LOG_ACRE_DTQ_ENTER
#define LOG_ACRE_DTQ_ENTER(pk_cdtq)
#endif /* LOG_ACRE_DTQ_ENTER */

#ifndef LOG_ACRE_DTQ_LEAVE
#define LOG_ACRE_DTQ_LEAVE(ercd)
#endif /* LOG_ACRE_DTQ_LEAVE */

#ifndef LOG_DEL_DTQ_ENTER
#define LOG_DEL_DTQ_ENTER(dtqid)
#endif /* LOG_DEL_DTQ_ENTER */

#ifndef LOG_DEL_DTQ_LEAVE
#define LOG_DEL_DTQ_LEAVE(ercd)
#endif /* LOG_DEL_DTQ_LEAVE */

#ifndef LOG_SND_DTQ_ENTER
#define LOG_SND_DTQ_ENTER(dtqid, data)
#endif /* LOG_SND_DTQ_ENTER */

#ifndef LOG_SND_DTQ_LEAVE
#define LOG_SND_DTQ_LEAVE(ercd)
#endif /* LOG_SND_DTQ_LEAVE */

#ifndef LOG_PSND_DTQ_ENTER
#define LOG_PSND_DTQ_ENTER(dtqid, data)
#endif /* LOG_PSND_DTQ_ENTER */

#ifndef LOG_PSND_DTQ_LEAVE
#define LOG_PSND_DTQ_LEAVE(ercd)
#endif /* LOG_PSND_DTQ_LEAVE */

#ifndef LOG_IPSND_DTQ_ENTER
#define LOG_IPSND_DTQ_ENTER(dtqid, data)
#endif /* LOG_IPSND_DTQ_ENTER */

#ifndef LOG_IPSND_DTQ_LEAVE
#define LOG_IPSND_DTQ_LEAVE(ercd)
#endif /* LOG_IPSND_DTQ_LEAVE */

#ifndef LOG_TSND_DTQ_ENTER
#define LOG_TSND_DTQ_ENTER(dtqid, data, tmout)
#endif /* LOG_TSND_DTQ_ENTER */

#ifndef LOG_TSND_DTQ_LEAVE
#define LOG_TSND_DTQ_LEAVE(ercd)
#endif /* LOG_TSND_DTQ_LEAVE */

#ifndef LOG_FSND_DTQ_ENTER
#define LOG_FSND_DTQ_ENTER(dtqid, data)
#endif /* LOG_FSND_DTQ_ENTER */

#ifndef LOG_FSND_DTQ_LEAVE
#define LOG_FSND_DTQ_LEAVE(ercd)
#endif /* LOG_FSND_DTQ_LEAVE */

#ifndef LOG_IFSND_DTQ_ENTER
#define LOG_IFSND_DTQ_ENTER(dtqid, data)
#endif /* LOG_IFSND_DTQ_ENTER */

#ifndef LOG_IFSND_DTQ_LEAVE
#define LOG_IFSND_DTQ_LEAVE(ercd)
#endif /* LOG_IFSND_DTQ_LEAVE */

#ifndef LOG_RCV_DTQ_ENTER
#define LOG_RCV_DTQ_ENTER(dtqid, p_data)
#endif /* LOG_RCV_DTQ_ENTER */

#ifndef LOG_RCV_DTQ_LEAVE
#define LOG_RCV_DTQ_LEAVE(ercd, data)
#endif /* LOG_RCV_DTQ_LEAVE */

#ifndef LOG_PRCV_DTQ_ENTER
#define LOG_PRCV_DTQ_ENTER(dtqid, p_data)
#endif /* LOG_PRCV_DTQ_ENTER */

#ifndef LOG_PRCV_DTQ_LEAVE
#define LOG_PRCV_DTQ_LEAVE(ercd, data)
#endif /* LOG_PRCV_DTQ_LEAVE */

#ifndef LOG_TRCV_DTQ_ENTER
#define LOG_TRCV_DTQ_ENTER(dtqid, p_data, tmout)
#endif /* LOG_TRCV_DTQ_ENTER */

#ifndef LOG_TRCV_DTQ_LEAVE
#define LOG_TRCV_DTQ_LEAVE(ercd, data)
#endif /* LOG_TRCV_DTQ_LEAVE */

#ifndef LOG_INI_DTQ_ENTER
#define LOG_INI_DTQ_ENTER(dtqid)
#endif /* LOG_INI_DTQ_ENTER */

#ifndef LOG_INI_DTQ_LEAVE
#define LOG_INI_DTQ_LEAVE(ercd)
#endif /* LOG_INI_DTQ_LEAVE */

#ifndef LOG_REF_DTQ_ENTER
#define LOG_REF_DTQ_ENTER(dtqid, pk_rdtq)
#endif /* LOG_REF_DTQ_ENTER */

#ifndef LOG_REF_DTQ_LEAVE
#define LOG_REF_DTQ_LEAVE(ercd, pk_rdtq)
#endif /* LOG_REF_DTQ_LEAVE */

/*
 *  �ǡ������塼�ο�
 */
#define tnum_dtq	((uint_t)(tmax_dtqid - TMIN_DTQID + 1))
#define tnum_sdtq	((uint_t)(tmax_sdtqid - TMIN_DTQID + 1))

/*
 *  �ǡ������塼ID����ǡ������塼�����֥��å�����Ф�����Υޥ���
 */
#define INDEX_DTQ(dtqid)	((uint_t)((dtqid) - TMIN_DTQID))
#define get_dtqcb(dtqid)	(&(dtqcb_table[INDEX_DTQ(dtqid)]))

#ifdef TOPPERS_dtqini

/*
 *  ���Ѥ��Ƥ��ʤ��ǡ������塼�����֥��å��Υꥹ��
 */
QUEUE	free_dtqcb;

/*
 *  �ǡ������塼��ǽ�ν����
 */
void
initialize_dataqueue(void)
{
	uint_t	i, j;
	DTQCB	*p_dtqcb;
	DTQINIB	*p_dtqinib;

	for (i = 0; i < tnum_sdtq; i++) {
		p_dtqcb = &(dtqcb_table[i]);
		queue_initialize(&(p_dtqcb->swait_queue));
		p_dtqcb->p_dtqinib = &(dtqinib_table[i]);
		queue_initialize(&(p_dtqcb->rwait_queue));
		p_dtqcb->count = 0U;
		p_dtqcb->head = 0U;
		p_dtqcb->tail = 0U;
	}
	queue_initialize(&free_dtqcb);
	for (j = 0; i < tnum_dtq; i++, j++) {
		p_dtqcb = &(dtqcb_table[i]);
		p_dtqinib = &(adtqinib_table[j]);
		p_dtqinib->dtqatr = TA_NOEXS;
		p_dtqcb->p_dtqinib = ((const DTQINIB *) p_dtqinib);
		queue_insert_prev(&free_dtqcb, &(p_dtqcb->swait_queue));
	}
}

#endif /* TOPPERS_dtqini */

/*
 *  �ǡ������塼�����ΰ�ؤΥǡ����γ�Ǽ
 */
#ifdef TOPPERS_dtqenq

void
enqueue_data(DTQCB *p_dtqcb, intptr_t data)
{
	(p_dtqcb->p_dtqinib->p_dtqmb + p_dtqcb->tail)->data = data;
	p_dtqcb->count++;
	p_dtqcb->tail++;
	if (p_dtqcb->tail >= p_dtqcb->p_dtqinib->dtqcnt) {
		p_dtqcb->tail = 0U;
	}
}

#endif /* TOPPERS_dtqenq */

/*
 *  �ǡ������塼�����ΰ�ؤΥǡ����ζ�����Ǽ
 */
#ifdef TOPPERS_dtqfenq

void
force_enqueue_data(DTQCB *p_dtqcb, intptr_t data)
{
	(p_dtqcb->p_dtqinib->p_dtqmb + p_dtqcb->tail)->data = data;
	p_dtqcb->tail++;
	if (p_dtqcb->tail >= p_dtqcb->p_dtqinib->dtqcnt) {
		p_dtqcb->tail = 0U;
	}
	if (p_dtqcb->count < p_dtqcb->p_dtqinib->dtqcnt) {
		p_dtqcb->count++;
	}
	else {
		p_dtqcb->head = p_dtqcb->tail;
	}
}

#endif /* TOPPERS_dtqfenq */

/*
 *  �ǡ������塼�����ΰ褫��Υǡ����μ�Ф�
 */
#ifdef TOPPERS_dtqdeq

void
dequeue_data(DTQCB *p_dtqcb, intptr_t *p_data)
{
	*p_data = (p_dtqcb->p_dtqinib->p_dtqmb + p_dtqcb->head)->data;
	p_dtqcb->count--;
	p_dtqcb->head++;
	if (p_dtqcb->head >= p_dtqcb->p_dtqinib->dtqcnt) {
		p_dtqcb->head = 0U;
	}
}

#endif /* TOPPERS_dtqdeq */

/*
 *  �ǡ������塼�ؤΥǡ�������
 */
#ifdef TOPPERS_dtqsnd

bool_t
send_data(DTQCB *p_dtqcb, intptr_t data, bool_t *p_dspreq)
{
	TCB		*p_tcb;

	if (!queue_empty(&(p_dtqcb->rwait_queue))) {
		p_tcb = (TCB *) queue_delete_next(&(p_dtqcb->rwait_queue));
		((WINFO_DTQ *)(p_tcb->p_winfo))->data = data;
		*p_dspreq = wait_complete(p_tcb);
		return(true);
	}
	else if (p_dtqcb->count < p_dtqcb->p_dtqinib->dtqcnt) {
		enqueue_data(p_dtqcb, data);
		*p_dspreq = false;
		return(true);
	}
	else {
		return(false);
	}
}

#endif /* TOPPERS_dtqsnd */

/*
 *  �ǡ������塼�ؤΥǡ�����������
 */
#ifdef TOPPERS_dtqfsnd

bool_t
force_send_data(DTQCB *p_dtqcb, intptr_t data)
{
	TCB		*p_tcb;

	if (!queue_empty(&(p_dtqcb->rwait_queue))) {
		p_tcb = (TCB *) queue_delete_next(&(p_dtqcb->rwait_queue));
		((WINFO_DTQ *)(p_tcb->p_winfo))->data = data;
		return(wait_complete(p_tcb));
	}
	else {
		force_enqueue_data(p_dtqcb, data);
		return(false);
	}
}

#endif /* TOPPERS_dtqfsnd */

/*
 *  �ǡ������塼����Υǡ�������
 */
#ifdef TOPPERS_dtqrcv

bool_t
receive_data(DTQCB *p_dtqcb, intptr_t *p_data, bool_t *p_dspreq)
{
	TCB		*p_tcb;
	intptr_t data;

	if (p_dtqcb->count > 0U) {
		dequeue_data(p_dtqcb, p_data);
		if (!queue_empty(&(p_dtqcb->swait_queue))) {
			p_tcb = (TCB *) queue_delete_next(&(p_dtqcb->swait_queue));
			data = ((WINFO_DTQ *)(p_tcb->p_winfo))->data;
			enqueue_data(p_dtqcb, data);
			*p_dspreq = wait_complete(p_tcb);
		}
		else {
			*p_dspreq = false;
		}
		return(true);
	}
	else if (!queue_empty(&(p_dtqcb->swait_queue))) {
		p_tcb = (TCB *) queue_delete_next(&(p_dtqcb->swait_queue));
		*p_data = ((WINFO_DTQ *)(p_tcb->p_winfo))->data;
		*p_dspreq = wait_complete(p_tcb);
		return(true);
	}
	else {
		return(false);
	}
}

#endif /* TOPPERS_dtqrcv */

/*
 *  �ǡ������塼������
 */
#ifdef TOPPERS_acre_dtq

ER_UINT
acre_dtq(const T_CDTQ *pk_cdtq)
{
	DTQCB	*p_dtqcb;
	DTQINIB	*p_dtqinib;
	ATR		dtqatr;
	DTQMB	*p_dtqmb;
	ER		ercd;

	LOG_ACRE_DTQ_ENTER(pk_cdtq);
	CHECK_TSKCTX_UNL();
	CHECK_RSATR(pk_cdtq->dtqatr, TA_TPRI);
	if (pk_cdtq->dtqmb != NULL) {
		CHECK_ALIGN_MB(pk_cdtq->dtqmb);
	}
	dtqatr = pk_cdtq->dtqatr;
	p_dtqmb = pk_cdtq->dtqmb;

	t_lock_cpu();
	if (tnum_dtq == 0 || queue_empty(&free_dtqcb)) {
		ercd = E_NOID;
	}
	else {
		if (pk_cdtq->dtqcnt != 0 && p_dtqmb == NULL) {
			p_dtqmb = kernel_malloc(sizeof(DTQMB) * pk_cdtq->dtqcnt);
			dtqatr |= TA_MBALLOC;
		}
		if (pk_cdtq->dtqcnt != 0 && p_dtqmb == NULL) {
			ercd = E_NOMEM;
		}
		else {
			p_dtqcb = ((DTQCB *) queue_delete_next(&free_dtqcb));
			p_dtqinib = (DTQINIB *)(p_dtqcb->p_dtqinib);
			p_dtqinib->dtqatr = dtqatr;
			p_dtqinib->dtqcnt = pk_cdtq->dtqcnt;
			p_dtqinib->p_dtqmb = p_dtqmb;

			queue_initialize(&(p_dtqcb->swait_queue));
			queue_initialize(&(p_dtqcb->rwait_queue));
			p_dtqcb->count = 0U;
			p_dtqcb->head = 0U;
			p_dtqcb->tail = 0U;
			ercd = DTQID(p_dtqcb);
		}
	}
	t_unlock_cpu();

  error_exit:
	LOG_ACRE_DTQ_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_acre_dtq */

/*
 *  �ǡ������塼�κ��
 */
#ifdef TOPPERS_del_dtq

ER
del_dtq(ID dtqid)
{
	DTQCB	*p_dtqcb;
	DTQINIB	*p_dtqinib;
	bool_t	dspreq;
	ER		ercd;

	LOG_DEL_DTQ_ENTER(dtqid);
	CHECK_TSKCTX_UNL();
	CHECK_DTQID(dtqid);
	p_dtqcb = get_dtqcb(dtqid);

	t_lock_cpu();
	if (p_dtqcb->p_dtqinib->dtqatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (DTQID(p_dtqcb) > tmax_sdtqid) {
		dspreq = init_wait_queue(&(p_dtqcb->swait_queue));
		if (init_wait_queue(&(p_dtqcb->rwait_queue))) {
			dspreq = true;
		}
		p_dtqinib = (DTQINIB *)(p_dtqcb->p_dtqinib);
		if ((p_dtqinib->dtqatr & TA_MBALLOC) != 0U) {
			kernel_free(p_dtqinib->p_dtqmb);
		}
		p_dtqinib->dtqatr = TA_NOEXS;
		queue_insert_prev(&free_dtqcb, &(p_dtqcb->swait_queue));
		if (dspreq) {
			dispatch();
		}
		ercd = E_OK;
	}
	else {
		ercd = E_OBJ;
	}
	t_unlock_cpu();

  error_exit:
	LOG_DEL_DTQ_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_del_dtq */

/*
 *  �ǡ������塼�ؤ�����
 */
#ifdef TOPPERS_snd_dtq

ER
snd_dtq(ID dtqid, intptr_t data)
{
	DTQCB	*p_dtqcb;
	WINFO_DTQ winfo_dtq;
	bool_t	dspreq;
	ER		ercd;

	LOG_SND_DTQ_ENTER(dtqid, data);
	CHECK_DISPATCH();
	CHECK_DTQID(dtqid);
	p_dtqcb = get_dtqcb(dtqid);

	t_lock_cpu();
	if (p_dtqcb->p_dtqinib->dtqatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (send_data(p_dtqcb, data, &dspreq)) {
		if (dspreq) {
			dispatch();
		}
		ercd = E_OK;
	}
	else {
		winfo_dtq.data = data;
		p_runtsk->tstat = (TS_WAITING | TS_WAIT_SDTQ);
		wobj_make_wait((WOBJCB *) p_dtqcb, (WINFO_WOBJ *) &winfo_dtq);
		dispatch();
		ercd = winfo_dtq.winfo.wercd;
	}
	t_unlock_cpu();

  error_exit:
	LOG_SND_DTQ_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_snd_dtq */

/*
 *  �ǡ������塼�ؤ������ʥݡ���󥰡�
 */
#ifdef TOPPERS_psnd_dtq

ER
psnd_dtq(ID dtqid, intptr_t data)
{
	DTQCB	*p_dtqcb;
	bool_t	dspreq;
	ER		ercd;

	LOG_PSND_DTQ_ENTER(dtqid, data);
	CHECK_TSKCTX_UNL();
	CHECK_DTQID(dtqid);
	p_dtqcb = get_dtqcb(dtqid);

	t_lock_cpu();
	if (p_dtqcb->p_dtqinib->dtqatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (send_data(p_dtqcb, data, &dspreq)) {
		if (dspreq) {
			dispatch();
		}
		ercd = E_OK;
	}
	else {
		ercd = E_TMOUT;
	}
	t_unlock_cpu();

  error_exit:
	LOG_PSND_DTQ_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_psnd_dtq */

/*
 *  �ǡ������塼�ؤ������ʥݡ���󥰡��󥿥�������ƥ������ѡ�
 */
#ifdef TOPPERS_ipsnd_dtq

ER
ipsnd_dtq(ID dtqid, intptr_t data)
{
	DTQCB	*p_dtqcb;
	bool_t	dspreq;
	ER		ercd;

	LOG_IPSND_DTQ_ENTER(dtqid, data);
	CHECK_INTCTX_UNL();
	CHECK_DTQID(dtqid);
	p_dtqcb = get_dtqcb(dtqid);

	i_lock_cpu();
	if (p_dtqcb->p_dtqinib->dtqatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (send_data(p_dtqcb, data, &dspreq)) {
		if (dspreq) {
			reqflg = true;
		}
		ercd = E_OK;
	}
	else {
		ercd = E_TMOUT;
	}
	i_unlock_cpu();

  error_exit:
	LOG_IPSND_DTQ_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_ipsnd_dtq */

/*
 *  �ǡ������塼�ؤ������ʥ����ॢ���Ȥ����
 */
#ifdef TOPPERS_tsnd_dtq

ER
tsnd_dtq(ID dtqid, intptr_t data, TMO tmout)
{
	DTQCB	*p_dtqcb;
	WINFO_DTQ winfo_dtq;
	TMEVTB	tmevtb;
	bool_t	dspreq;
	ER		ercd;

	LOG_TSND_DTQ_ENTER(dtqid, data, tmout);
	CHECK_DISPATCH();
	CHECK_DTQID(dtqid);
	CHECK_TMOUT(tmout);
	p_dtqcb = get_dtqcb(dtqid);

	t_lock_cpu();
	if (p_dtqcb->p_dtqinib->dtqatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (send_data(p_dtqcb, data, &dspreq)) {
		if (dspreq) {
			dispatch();
		}
		ercd = E_OK;
	}
	else if (tmout == TMO_POL) {
		ercd = E_TMOUT;
	}
	else {
		winfo_dtq.data = data;
		p_runtsk->tstat = (TS_WAITING | TS_WAIT_SDTQ);
		wobj_make_wait_tmout((WOBJCB *) p_dtqcb, (WINFO_WOBJ *) &winfo_dtq,
														&tmevtb, tmout);
		dispatch();
		ercd = winfo_dtq.winfo.wercd;
	}
	t_unlock_cpu();

  error_exit:
	LOG_TSND_DTQ_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_tsnd_dtq */

/*
 *  �ǡ������塼�ؤζ�������
 */
#ifdef TOPPERS_fsnd_dtq

ER
fsnd_dtq(ID dtqid, intptr_t data)
{
	DTQCB	*p_dtqcb;	
	ER		ercd;

	LOG_FSND_DTQ_ENTER(dtqid, data);
	CHECK_TSKCTX_UNL();
	CHECK_DTQID(dtqid);
	p_dtqcb = get_dtqcb(dtqid);

	t_lock_cpu();
	if (p_dtqcb->p_dtqinib->dtqatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (!(p_dtqcb->p_dtqinib->dtqcnt > 0U)) {
		ercd = E_ILUSE;
	}
	else if (force_send_data(p_dtqcb, data)) {
		dispatch();
	}
	ercd = E_OK;
	t_unlock_cpu();

  error_exit:
	LOG_FSND_DTQ_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_fsnd_dtq */

/*
 *  �ǡ������塼�ؤζ����������󥿥�������ƥ������ѡ�
 */
#ifdef TOPPERS_ifsnd_dtq

ER
ifsnd_dtq(ID dtqid, intptr_t data)
{
	DTQCB	*p_dtqcb;
	ER		ercd;

	LOG_IFSND_DTQ_ENTER(dtqid, data);
	CHECK_INTCTX_UNL();
	CHECK_DTQID(dtqid);
	p_dtqcb = get_dtqcb(dtqid);

	i_lock_cpu();
	if (p_dtqcb->p_dtqinib->dtqatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (!(p_dtqcb->p_dtqinib->dtqcnt > 0U)) {
		ercd = E_ILUSE;
	}
	else if (force_send_data(p_dtqcb, data)) {
		reqflg = true;
	}
	ercd = E_OK;
	i_unlock_cpu();

  error_exit:
	LOG_IFSND_DTQ_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_ifsnd_dtq */

/*
 *  �ǡ������塼����μ���
 */
#ifdef TOPPERS_rcv_dtq

ER
rcv_dtq(ID dtqid, intptr_t *p_data)
{
	DTQCB	*p_dtqcb;
	WINFO_DTQ winfo_dtq;
	bool_t	dspreq;
	ER		ercd;

	LOG_RCV_DTQ_ENTER(dtqid, p_data);
	CHECK_DISPATCH();
	CHECK_DTQID(dtqid);
	p_dtqcb = get_dtqcb(dtqid);

	t_lock_cpu();
	if (p_dtqcb->p_dtqinib->dtqatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (receive_data(p_dtqcb, p_data, &dspreq)) {
		if (dspreq) {
			dispatch();
		}
		ercd = E_OK;
	}
	else {
		p_runtsk->tstat = (TS_WAITING | TS_WAIT_RDTQ);
		make_wait(&(winfo_dtq.winfo));
		queue_insert_prev(&(p_dtqcb->rwait_queue), &(p_runtsk->task_queue));
		winfo_dtq.p_dtqcb = p_dtqcb;
		LOG_TSKSTAT(p_runtsk);
		dispatch();
		ercd = winfo_dtq.winfo.wercd;
		if (ercd == E_OK) {
			*p_data = winfo_dtq.data;
		}
	}
	t_unlock_cpu();

  error_exit:
	LOG_RCV_DTQ_LEAVE(ercd, *p_data);
	return(ercd);
}

#endif /* TOPPERS_rcv_dtq */

/*
 *  �ǡ������塼����μ����ʥݡ���󥰡�
 */
#ifdef TOPPERS_prcv_dtq

ER
prcv_dtq(ID dtqid, intptr_t *p_data)
{
	DTQCB	*p_dtqcb;
	bool_t	dspreq;
	ER		ercd;

	LOG_PRCV_DTQ_ENTER(dtqid, p_data);
	CHECK_TSKCTX_UNL();
	CHECK_DTQID(dtqid);
	p_dtqcb = get_dtqcb(dtqid);

	t_lock_cpu();
	if (p_dtqcb->p_dtqinib->dtqatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (receive_data(p_dtqcb, p_data, &dspreq)) {
		if (dspreq) {
			dispatch();
		}
		ercd = E_OK;
	}
	else {
		ercd = E_TMOUT;
	}
	t_unlock_cpu();

  error_exit:
	LOG_PRCV_DTQ_LEAVE(ercd, *p_data);
	return(ercd);
}

#endif /* TOPPERS_prcv_dtq */

/*
 *  �ǡ������塼����μ����ʥ����ॢ���Ȥ����
 */
#ifdef TOPPERS_trcv_dtq

ER
trcv_dtq(ID dtqid, intptr_t *p_data, TMO tmout)
{
	DTQCB	*p_dtqcb;
	WINFO_DTQ winfo_dtq;
	TMEVTB	tmevtb;
	bool_t	dspreq;
	ER		ercd;

	LOG_TRCV_DTQ_ENTER(dtqid, p_data, tmout);
	CHECK_DISPATCH();
	CHECK_DTQID(dtqid);
	CHECK_TMOUT(tmout);
	p_dtqcb = get_dtqcb(dtqid);

	t_lock_cpu();
	if (p_dtqcb->p_dtqinib->dtqatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else if (receive_data(p_dtqcb, p_data, &dspreq)) {
		if (dspreq) {
			dispatch();
		}
		ercd = E_OK;
	}
	else if (tmout == TMO_POL) {
		ercd = E_TMOUT;
	}
	else {
		p_runtsk->tstat = (TS_WAITING | TS_WAIT_RDTQ);
		make_wait_tmout(&(winfo_dtq.winfo), &tmevtb, tmout);
		queue_insert_prev(&(p_dtqcb->rwait_queue), &(p_runtsk->task_queue));
		winfo_dtq.p_dtqcb = p_dtqcb;
		LOG_TSKSTAT(p_runtsk);
		dispatch();
		ercd = winfo_dtq.winfo.wercd;
		if (ercd == E_OK) {
			*p_data = winfo_dtq.data;
		}
	}
	t_unlock_cpu();

  error_exit:
	LOG_TRCV_DTQ_LEAVE(ercd, *p_data);
	return(ercd);
}

#endif /* TOPPERS_trcv_dtq */

/*
 *  �ǡ������塼�κƽ����
 */
#ifdef TOPPERS_ini_dtq

ER
ini_dtq(ID dtqid)
{
	DTQCB	*p_dtqcb;
	bool_t	dspreq;
	ER		ercd;
    
	LOG_INI_DTQ_ENTER(dtqid);
	CHECK_TSKCTX_UNL();
	CHECK_DTQID(dtqid);
	p_dtqcb = get_dtqcb(dtqid);

	t_lock_cpu();
	if (p_dtqcb->p_dtqinib->dtqatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else {
		dspreq = init_wait_queue(&(p_dtqcb->swait_queue));
		if (init_wait_queue(&(p_dtqcb->rwait_queue))) {
			dspreq = true;
		}
		p_dtqcb->count = 0U;
		p_dtqcb->head = 0U;
		p_dtqcb->tail = 0U;
		if (dspreq) {
			dispatch();
		}
		ercd = E_OK;
	}
	t_unlock_cpu();

  error_exit:
	LOG_INI_DTQ_LEAVE(ercd);
	return(ercd);
}

#endif /* TOPPERS_ini_dtq */

/*
 *  �ǡ������塼�ξ��ֻ���
 */
#ifdef TOPPERS_ref_dtq

ER
ref_dtq(ID dtqid, T_RDTQ *pk_rdtq)
{
	DTQCB	*p_dtqcb;
	ER		ercd;
    
	LOG_REF_DTQ_ENTER(dtqid, pk_rdtq);
	CHECK_TSKCTX_UNL();
	CHECK_DTQID(dtqid);
	p_dtqcb = get_dtqcb(dtqid);

	t_lock_cpu();
	if (p_dtqcb->p_dtqinib->dtqatr == TA_NOEXS) {
		ercd = E_NOEXS;
	}
	else {
		pk_rdtq->stskid = wait_tskid(&(p_dtqcb->swait_queue));
		pk_rdtq->rtskid = wait_tskid(&(p_dtqcb->rwait_queue));
		pk_rdtq->sdtqcnt = p_dtqcb->count;
		ercd = E_OK;
	}
	t_unlock_cpu();

  error_exit:
	LOG_REF_DTQ_LEAVE(ercd, pk_rdtq);
	return(ercd);
}

#endif /* TOPPERS_ref_dtq */