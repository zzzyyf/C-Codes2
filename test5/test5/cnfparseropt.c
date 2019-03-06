/**
 * cnfparseropt.c
 * �����ȡ������.cnf�ļ��еĺ�ȡ��ʽ��ʹ֮��Ϊ�ܱ�DPLL�㷨���������
 * �������������ṩ����ڲ��ṹ�ĺ���traverse���Ա���֤��ȡ�Ƿ���ȷ��
 */
#include "funcs.h"
#include <limits.h>
 /**
  * ��ĳ���Ӿ��е��������ָ�����̳����Ӿ䡣
  * @param head
  * @param liteHead
  */
void setShortestLen(clauseHead2 *head, liteNode2 *liteHead) {
	lvNode *p;
	for (p = head->head->next; p; p = p->next) {
		if (head->length < liteHead[p->index].shortestLen) {
			liteHead[p->index].shortestLen = head->length;
		}
	}
}
/**
 * ��.cnf����ڲ������ṹ��out�ļ�����
 * @param filename
 * @param root1
 * @return
 */
int traverse2(char *filename, root2 *root1) {
	FILE *out = fopen(filename, "w");
	if (!out) return ERROR;
	clauseHead2 *pc;
	lvNode *pn;
	fprintf(out, "p cnf %d %d\n", root1->literalNum, root1->clauseNum);
	for (pc = root1->chainHead->next; pc; pc = pc->next) { //���δ����֦���Ӿ�
		for (pn = pc->head->next; pn; pn = pn->next) {
			if (pn->value == FALSE)putc('-', out);
			fprintf(out, "%d ", pn->index);
		}
		fprintf(out, "0\n");
	}
	fclose(out);
	return OK;
}

/**
 * ��addAfter�����һ����ͷ���Ŀ��Ӿ䲢��������ӿ��Ӿ��ָ�롣
 * @param addAfter ���Ӿ���������
 * @return ������Ӿ��ָ��
 */
clauseHead2 *addClause2(clauseHead2 *addAfter) {
	clauseHead2 *p, *q = addAfter->next;
	lvNode *pl;
	p = malloc(sizeof(clauseHead2));
	if (!p) return NULL;
	pl = malloc(sizeof(lvNode));
	if (!pl) return NULL;
	p->head = pl;
	p->length = 0;
	p->isDeleted = FALSE;
	//�������Ӿ���
	p->next = addAfter->next;
	addAfter->next = p;
	//��ʼ��ͷ���
	pl->value = TRUE, pl->index = -1;
	pl->next = NULL;
	return p;
}
/**
 * ��addIn�Ӿ�ĩ�����һ����ֵΪvalue��index���֣������ظ����ֽ��ָ�롣
 * @param addIn
 * @param index
 * @param value
 * @return
 */
lvNode *addNode2(clauseHead2 *addIn, literal index, unsigned int value) {
	lvNode *p = addIn->head, *q;
	for (; p->next; p = p->next); //�������Ӿ��ڵ�ĩ�˿մ�
	q = malloc(sizeof(lvNode));
	if (!q) return NULL;
	//��ʼ���½��
	q->level = 0;
	q->index = index; q->value = value;
	q->next = NULL;
	p->next = q;
	addIn->length++;
	return q;
}
/**
 * ��ȡcnf�ļ���
 * @param filename �ļ������ַ���
 * @param root1 �Ӿ�������ͷ
 * @param isOpt �Ƿ�����Ż��㷨������Ϊ��0��������Ϊ0
 * @return ����ȡʧ�ܷ���NULL�����򷵻����ֱ�ı�ͷ
 */
liteNode2 *read2(char *filename, root2 *root1) {
	FILE *in = fopen(filename, "r");
	char t[200], *pc1 = t, *pc2;
	unsigned liteNum, clauseNum;
	if (!in) return NULL; //��ȡ�ļ�ʧ��
	//��ȡ�ļ�ͷ
	while (fscanf(in, "%[c]", t)) {
		while (fscanf(in, "%*[^\n]")); //����ע��
		fscanf(in, "%*[\n]");
	}
	if (fscanf(in, "p cnf %d%d", &liteNum, &clauseNum)) {//��ȡ�ļ�ͷ
		root1->literalNum = liteNum;
		root1->clauseNum = clauseNum;
		printf("��ȡ��cnf�ļ�\n");
	}
	else return NULL; //��ȡ�����ļ�ͷ

	//��ʼ�����ֱ�
	liteNode2 *liteHead = (liteNode2 *)malloc(sizeof(liteNode2)*(root1->literalNum + 1)); //�����ַ���ռ�
	if (!liteHead) return NULL;
	for (int i = 1; i < root1->literalNum + 1; i++) {
		liteHead[i].times = 0;
		liteHead[i].isAssigned = FALSE;
		liteHead[i].shortestLen = MAX_LITE_NUM;
		liteHead[i].isLenChecked = FALSE;
	}
	literal temp, ab; //���ÿ���Ӿ��е������ã�abΪtemp�ľ���ֵ

	//�����Ӿ�����ͷ���
	root1->chainHead = malloc(sizeof(clauseHead2));
	if (!root1->chainHead) return NULL;
	root1->chainHead->head = NULL;

	clauseHead2 *pChain = NULL, *q; //�Ӿ����ϵ�ָ�룬p����ǰ���Ӿ��������Ľ��
	//���ɵ�һ���Ӿ�ͷ
	pChain = malloc(sizeof(clauseHead2)); //����ռ�
	if (!pChain) return NULL;
	pChain->next = NULL; //��ʼ��
	pChain->isDeleted = FALSE;
	root1->chainHead->next = pChain; //�ѵ�һ���Ӿ�ͷ��������

	pChain->head = malloc(sizeof(lvNode)); //���ɵ�һ������ͷ
	if (!pChain->head) return NULL;
	lvNode *p = pChain->head; //�Ӿ��ڵ�ָ�룬p����ǰ���Ӿ��������Ľ��
	p->next = NULL; p->index = -1; //��ʼ��

	while (fgets(pc1, sizeof(t) - 1, in)) { //TODO: �ļ�β��ע���е��ܷ��������
		//���²�����strtol�������һ�ζ�ȡ�������֡�
		for (temp = strtol(pc1, &pc2, 10); pc2 != pc1; temp = strtol(pc1, &pc2, 10)) {
			if (temp) {//��i��Ϊ0
				ab = abs(temp);
				addNode2(pChain, ab, temp > 0 ? TRUE : FALSE);
				liteHead[ab].times++;
			}
			else {
				q = pChain;
				pChain = addClause2(pChain);
				setShortestLen(q, liteHead);
			}
			pc1 = pc2; //������ȡ��һ������
		}
		pc1 = t; //ÿ�ζ�ȡ��һ�о�Ҫ��t2��ͷ���¶�ȡ
	}
	//ɾȥ�����ͷ���
	free(q->next);
	q->next = NULL;
	printf("��ȡ��ɡ�");
	fclose(in);
	return liteHead;
}
