#include "funcs.h"
#include <math.h>

extern clock_t start, end;
/** �����ǵ��Ӿ�����㷨 **/
/**
* ɾ��deleteAfter�ĺ���Ӿ䡣
* @param deleteAfter
*/
void deleteClause(clauseHead *deleteAfter) {
	node *pn, *qn;
	clauseHead *pc; //��ɾ�����Ӿ䣬��deleteAfter��next
	pc = deleteAfter->next;
	//��pc�����жϿ�
	deleteAfter->next = deleteAfter->next->next;
	//��ɾ����ͷ��������������Ϊ����������Ƿ�Ϊ��ͷ��if
	pn = pc->head, qn = pn->next;
	free(pn);
	pn = qn;
	if (qn)qn = qn->next;
	//��ɾ��������
	for (; pn;) {
		free(pn);
		pn = qn;
		if (qn)qn = qn->next;
	}
	pc->next = NULL;
	free(pc);
}

/**
* ����deleteIn��ָ�Ӿ䣬��ɾ�����е��Ӿ�ķ����֣�����deleteIn��ָ�Ӿ�ľ䳤�Ա��ж��Ƿ��������Ӿ䡣��isOptΪ������Ϊ�Ż���׼����
* @param deleteIn
* @param liteHead
* @param isOpt
* @return
*/
int deleteReverseNode(clauseHead *deleteIn, liteNode *liteHead) {
	node *pn, *qn;
	int len = 0; //ͳ�ƾ䳤��
	for (pn = deleteIn->head, qn = pn->next; qn;) {
		if (liteHead[qn->index].isAssigned == TRUE && liteHead[qn->index].value != qn->value) { //qn��������ָ�����ֻ�Ϊ������
			pn->next = qn->next;
			free(qn);
			qn = pn->next;
		}
		else { //qn���ֲ���ɾ
			pn = qn;
			qn = qn->next;
			len++;
		}
	}
	return len;
}

/**
* ͨ��ѭ��ɾ�����е��Ӿ���ͬ���ֻ��ѱ���ֵ���ֵ��Ӿ���ɾ�����е��Ӿ��෴��������SAT���⣬DPLL����Ҫ�㷨��
* ���������Ӿ���ʱ����TRUE�����ֿ��Ӿ�ʱ����FALSE�������������2��
* @param liteHead
* @param root1
*/
unsigned int simpleClause(liteNode *liteHead, root *root1) {
	clauseHead *pc, *qc;
	int n = 1, deleted; //n��ʾɾ�����ֺ��Ӿ䳤��������Ϊ1˵����Ҫ�ٴ�ɾ�����Ӿ�; deleted��ʾһ�ε����Ƿ�ɾ�����Ӿ�
	//clock_t start,end;
	node *qn;
	while (n == 1) {
		n = 0;
		//start=clock();
		for (pc = root1->chainHead, qc = pc->next; qc;) { //���ǿ��Ӿ�qc�Ƿ�ɱ���
			deleted = 0;
			//�ȼ���Ƿ�Ϊ���Ӿ�
			if (qc->head->next && qc->head->next->next == NULL) { //��qc��Ҫ��ɾ��
				if (liteHead[qc->head->next->index].isAssigned != TRUE ||
					liteHead[qc->head->next->index].value == qc->head->next->value) { //���ñ�Ԫ�ѱ���ֵ��Ŀǰ��ֵ�����ֱ�ֵ�෴��˵�������Ҫɾ�ɿ��Ӿ䣬������ǰɾ��
					//���qc�ڵı�Ԫ
					liteHead[qc->head->next->index].isAssigned = TRUE;
					liteHead[qc->head->next->index].value = qc->head->next->value;
					//printf("    ���Ӿ�");
					//if (liteHead[qc->head->next->index].value == FALSE) putchar('-');
					//printf("%d��\n", qc->head->next->index);
					deleteClause(pc); //ɾ��pc->next��qc
					deleted++;
				}
			}
			else {
				//�ټ���Ƿ񺬵��Ӿ������
				for (qn = qc->head->next; qn;) {
					if (liteHead[qn->index].isAssigned == TRUE &&
						liteHead[qn->index].value == qn->value) { //qn�������ڵ��Ӿ��г���
						deleteClause(pc); //ɾ��pc->next��qc
						deleted++;
						break; //ɾ�꼴������
					}
					else { //qnδ�ڵ��Ӿ��г��ֹ�
						qn = qn->next;
					}
				}
			}
			//ɾ�����Ӿ��ѭ������
			if (deleted) qc = pc->next; //qcԭ��ָ����Ѿ���ɾ����
			//����ѭ������
			if (!deleted) {
				//�ټ��qc�Ƿ񺬵��Ӿ�ķ�����
				if (n != 1) n = deleteReverseNode(qc, liteHead); //����ɾ�����е��Ӿ�ķ�����
				if (n == 0)
					return FALSE;
				pc = qc;
				qc = qc->next;
			}
		}
		//end=clock();
		if (!root1->chainHead->next)
			return TRUE;
	}
	return 2;
}

/** ������DPLL�㷨 **/
/**
* ���ѡȡһ��δ����ֵ������
* @param liteHead
* @param liteNum
* @return ѡȡ������
*/
int pickLiteRandom(liteNode *liteHead, int liteNum) {
	int index;
	srand((unsigned)time(0));
	while (liteHead[index = liteNum * rand() / (RAND_MAX + 1u) + 1].isAssigned == TRUE); //��index�ѱ���ֵ������ѡ��
	return index;
}

int pickLiteSeq(liteNode *liteHead, int liteNum) {
	int index = 1;
	while (index < (liteNum + 1) && liteHead[index].isAssigned == TRUE)index++; //��index�ѱ���ֵ������ѡ��
	return index;
}

root *copyChain(root *root1) {
	//�ȸ�����ͷ
	root *r = malloc(sizeof(root));
	if (!r) return NULL;
	r->clauseNum = root1->clauseNum;
	r->literalNum = root1->literalNum;
	//���½���ͷ������Ҫ��Ϣ�����踴�ƣ��½����ɣ�
	r->chainHead = malloc(sizeof(chainHead)); //����ͷ����ռ�
	if (!r->chainHead) return NULL;
	r->chainHead->head = NULL;
	r->chainHead->next = NULL;
	clauseHead *pc1 = root1->chainHead->next, *pc2 = r->chainHead;
	node *pn1, *pn2;
	for (; pc1; pc1 = pc1->next) {
		pc2->next = malloc(sizeof(clauseHead));
		if (!pc2->next) return NULL;
		pc2 = pc2->next;
		pc2->next = NULL;
		pc2->head = malloc(sizeof(node));
		if (!pc2->head) return NULL;
		pc2->head->index = -1; pc2->head->next = NULL;
		for (pn1 = pc1->head->next, pn2 = pc2->head; pn1; pn1 = pn1->next) {
			pn2->next = malloc(sizeof(node));
			if (!pn2->next) return NULL;
			pn2 = pn2->next;
			pn2->value = pn1->value; pn2->index = pn1->index;
			pn2->next = NULL;
		}
	}
	return r;
}

liteNode *copyLite(liteNode *liteHead, int liteNum) {
	liteNode *p = malloc(sizeof(liteNode)*(liteNum + 1));
	if (!p) return NULL;
	for (int i = 1; i < liteNum + 1; i++) {
		p[i] = liteHead[i];
		p[i].isLenChecked = FALSE; //ÿ��DPLL֮ǰ������Ӿ䶼��δȷ����
	}
	return p;
}
/**
*
* @param root1
* @param liteHead
* @param solution
* @param lite
* @param value
* @return
* @todo sat-303, unsat-100
*/
unsigned DPLL(root *root1, liteNode *liteHead, liteNode **solution, int lite, unsigned value) {
	clauseHead *pc;
	end = clock();
	if ((end - start) / CLK_TCK > MAX_TIME) return ERROR;
	//printf("%d\n", nn);
	//if (value == FALSE)putchar('-');
	//printf("%d��\n", lite);
	//���浱ǰ������ݽṹ
	root *r = copyChain(root1);
	liteNode *l = copyLite(liteHead, r->literalNum);
	unsigned result;
	l[lite].value = value; l[lite].isAssigned = TRUE;
	result = simpleClause(l, r);
	if (!r->chainHead->next) { //���õ��Ӿ���Ϊ��˵���н�
		free(r->chainHead); //ɾ����ͷ
		*solution = l; //������ָ��
		l[0].shortestLen = r->literalNum; //�����Ԫ����
		free(r); //ɾ����ͷ
		return TRUE;
	}
	else if (result == FALSE) { //�����ֿ��Ӿ�˵��������޽�
		for (pc = r->chainHead; pc->next; deleteClause(pc)); //ɾ��chainHead����������Ӿ�
		free(pc);
		free(l);
		free(r);
		return FALSE;
	}
	else { //��Ҫ��ȡ��������
		//lite = pickLiteRandom(l, root1->literalNum); //���ѡȡһ��δ����ֵ������
		lite = pickLiteSeq(l, root1->literalNum);
		if (DPLL(r, l, solution, lite, TRUE) == TRUE) {
			for (pc = r->chainHead; pc->next; deleteClause(pc)); //ɾ��chainHead����������Ӿ�
			free(pc);
			free(r);
			free(l);
			return TRUE;
		}
		else {
			result = DPLL(r, l, solution, lite, FALSE);
			for (pc = r->chainHead; pc->next; deleteClause(pc)); //ɾ��chainHead����������Ӿ�
			free(pc);
			r->chainHead = NULL;
			free(r);
			free(l);
			return result;
		}
	}

}

/**
* ��������ⲻ����������������������0.
*/
unsigned int checkSolution(liteNode *solution, root *root1) {
	clauseHead *pc;
	node *pn;
	unsigned i, n = 0;
	for (pc = root1->chainHead->next; pc; pc = pc->next) {
		n++;
		i = 0;
		for (pn = pc->head->next; pn; pn = pn->next) {
			if (solution[pn->index].value == pn->value) {
				i = 1; //����Ϊ�پͼ�����һ��������Ϊ������֤����
				break;
			}
		}
		if (!i) return n; //�������Ϊ���򲻳���
	}
	return 0; //ִ�е����˵��ȫΪ��
}

