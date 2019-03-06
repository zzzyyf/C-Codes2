#include "funcs.h"
#include <math.h>
/** �������Ż��㷨 **/

extern clock_t start, end;
/**
* ����deleteIn��ָ�Ӿ䣬��ɾ�����е��Ӿ�ķ����֣�����deleteIn��ָ�Ӿ�ľ䳤�Ա��ж��Ƿ��������Ӿ䡣��isOptΪ������Ϊ�Ż���׼����
* @param deleteIn
* @param liteHead
* @param isOpt
* @return
*/
int deleteReverseNode2(clauseHead2 *deleteIn, liteNode2 *liteHead) {
	lvNode *pn, *qn;
	unsigned len = 0; //ͳ�ƾ䳤��
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
	deleteIn->length = len;
	setLiteInfo(deleteIn, liteHead);
	return len;
}

/**
* ɾ��deleteAfter�ĺ���Ӿ䡣
* @param deleteAfter
*/
void deleteClause2(clauseHead2 *deleteAfter, liteNode2 *l) {
	lvNode *pn, *qn;
	clauseHead2 *pc; //��ɾ�����Ӿ䣬��deleteAfter��next
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
		l[pn->index].times--;
		free(pn);
		pn = qn;
		if (qn)qn = qn->next;
	}
	pc->next = NULL;
	free(pc);
}

/**
* ͨ��ѭ��ɾ�����е��Ӿ���ͬ���ֻ��ѱ���ֵ���ֵ��Ӿ���ɾ�����е��Ӿ��෴��������SAT���⣬DPLL����Ҫ�㷨��
* ���������Ӿ���ʱ����TRUE�����򷵻�FALSE��
* @param liteHead
* @param root1
* @todo unfinished
*/
unsigned int simpleClause2(liteNode2 *l, root2 *r) {
	clauseHead2 *pc, *qc;
	int n = 1, deleted; //n��ʾɾ�����ֺ��Ӿ䳤��������Ϊ1˵����Ҫ�ٴ�ɾ�����Ӿ�; deleted��ʾһ�ε����Ƿ�ɾ�����Ӿ�
	lvNode *qn;
	while (n == 1) {
		n = 0;
		for (pc = r->chainHead, qc = pc->next; qc;) { //���ǿ��Ӿ�qc�Ƿ�ɱ���
			deleted = 0;
			//�ȼ���Ƿ�Ϊ���Ӿ�
			if (qc->head->next && qc->head->next->next == NULL) { //��qc��Ҫ��ɾ��
				if (l[qc->head->next->index].isAssigned != TRUE ||
					l[qc->head->next->index].value == qc->head->next->value) { //���ñ�Ԫ�ѱ���ֵ��Ŀǰ��ֵ�����ֱ�ֵ�෴��˵�������Ҫɾ�ɿ��Ӿ䣬������ǰɾ��
					//���qc�ڵı�Ԫ
					l[qc->head->next->index].isAssigned = TRUE;
					l[qc->head->next->index].value = qc->head->next->value;
					deleteClause2(pc, l); //ɾ��pc->next��qc
					deleted++;
				}
			}
			else {
				//�ټ���Ƿ񺬵��Ӿ������
				for (qn = qc->head->next; qn;) {
					if (l[qn->index].isAssigned == TRUE &&
						l[qn->index].value == qn->value) { //qn�������ڵ��Ӿ��г���
						setLiteInfo(qc, l);
						deleteClause2(pc, l); //ɾ��pc->next��qc
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
				if (n != 1) {
					n = deleteReverseNode2(qc, l); //����ɾ�����е��Ӿ�ķ�����
				}
				if (n == 0)
					return FALSE;
				pc = qc;
				qc = qc->next;
			}
		}
		if (!r->chainHead->next)
			return TRUE;
	}
	return 2;
}


/**
 * ����pc����δ����ֵ�����ֵ�����Ӿ䳤��
 * @param pc
 * @param l
 * @param level
 * @return
 */
void setLiteInfo(clauseHead2 *pc, liteNode2 *l) {
	lvNode *pn = pc->head->next;
	if (pc->isDeleted == FALSE) {
		//pcδ��ɾ��ʱ����Ҫ��鱾���Ƿ�Ϊδ��ֵ���ֵ�������Ӿ�
		for (; pn; pn = pn->next) {
			if (l[pn->index].isAssigned == FALSE &&
				l[pn->index].shortestLen > pc->length) {//������δ����ֵ�Ҹþ䳤��С
				if (l[pn->index].isLenChecked == FALSE)l[pn->index].isLenChecked = TRUE;
				l[pn->index].shortestLen = pc->length;
			}
		}
	}
	else {
		//pc�ѱ�ɾ������鱾���Ƿ�����δ��ֵ���ֵ�����Ӿ�
		for (; pn; pn = pn->next) {
			if (l[pn->index].isAssigned == FALSE &&
				l[pn->index].shortestLen <= pc->length) { //������δ����ֵ�Ҹþ䳤��С
				if (l[pn->index].isLenChecked == FALSE) {
					l[pn->index].isLenChecked = TRUE;
					l[pn->index].shortestLen = MAX_LITE_NUM;
				}
			}
		}
	}
}
/**
 * ��Ǻ����ѱ���ֵ���ֵ��Ӿ䡣ע���ⲻ��ɾ�����Ӿ�ĺ��������Ǳ�ǵĺ�����
 * @param pc
 * @param l
 * @param p
 * @return ERROR��mallocʧ�ܣ� OK������
 */
int markAndSaveClause(clauseHead2 *pc, liteNode2 *l, lvElem *p) {
	lvNode *pn = pc->head->next;
	ptrNode *pp = p->cutChain;
	unsigned isDeleted = FALSE;
	//��pc�ǵ��Ӿ�
	if (pc->length == 1) {
		for (pn = pc->head->next; pn->level != 0; pn = pn->next); //���������Ӿ�Ľ��
		//�þ�ֻ��һ��λ�ڸò�Ľ�㣬��Ϊ�ò�ĵ��Ӿ䣬����Ҫôδ����ֵ��Ҫô�ѱ���ֵ�Ҳ��Ƿ�����
		if (l[pn->index].isAssigned == TRUE && l[pn->index].value == pn->value || l[pn->index].isAssigned == FALSE) {
			pc->isDeleted = TRUE;
			if (l[pn->index].isAssigned == FALSE) { //Ҫ����һ�����Ӿ�
				//�����������Ӿ���Ϣ
				p->liteList = realloc(p->liteList, sizeof(int)*(++p->liteList[0] + 1));
				p->liteList[p->liteList[0]] = pn->index; //����β������������
				if (pn->value == FALSE) p->liteList[p->liteList[0]] = -pn->index;
				//printf("selected: %d\n", p->liteList[p->liteList[0]]);
				l[pn->index].isAssigned = TRUE;
				l[pn->index].value = pn->value;
			}
			isDeleted = TRUE;
			//����ɾ�����Ӿ�λ��
			for (; pp->next; pp = pp->next); //��������β
			pp->next = malloc(sizeof(ptrNode));
			if (!pp->next)
				return ERROR;
			pp = pp->next;
			pp->cutClause = pc;
			pp->next = NULL;
		}
	}
	else { //pc���ǵ��Ӿ�ʱ
		//���������ѱ���ֵ������
		for (; pn; pn = pn->next) {
			if (pn->level==0 && l[pn->index].isAssigned == TRUE && l[pn->index].value == pn->value) {
				pc->isDeleted = TRUE;
				isDeleted = TRUE;
				//����ɾ�����Ӿ�λ��
				for (; pp->next; pp = pp->next); //��������β
				pp->next = malloc(sizeof(ptrNode));
				if (!pp->next)
					return ERROR;
				pp = pp->next;
				pp->cutClause = pc;
				pp->next = NULL;
				break;
			}
		}
	}
	
	if (isDeleted == TRUE) {
		//printf("del:");
		for (pn = pc->head->next; pn; pn = pn->next) {
			//if (pn->value == FALSE) printf("-");
			//printf("%d ", pn->index);
			if (l[pn->index].isAssigned == FALSE && pn->level == 0) {
				l[pn->index].times--;
			}
		}
		//printf("\n");
		setLiteInfo(pc, l); //����δ����shortestLen�����֮
	}
	return OK;
}

/**
 *
 * @param pc
 * @param l
 * @param level
 * @return TRUE�����ֿ��Ӿ䣻2�������������Ӿ䣻FALSE�������϶���
 */
int markLite(clauseHead2 *pc, liteNode2 *l, unsigned int level) {
	lvNode *pn; //pn�ǿ�����Ҫ��ǵ����֣�pn��qnǰ������
	int changed = 0;
	//ֻ����δ����ǵı�Ԫ����
	for (pn = pc->head; pn; pn=pn->next) {
		if (pn->level == 0 && l[pn->index].isAssigned == TRUE && l[pn->index].value!=pn->value) { //��pn->level֮ǰ�Ķ����Ź�����֮��Ļ�δ�����
			pn->level = level;
			l[pn->index].times--;
			pc->length--;
			changed++;
		}
	}
	if (changed) {
		setLiteInfo(pc, l);
		if (pc->length == 0) //��pc���������־�����ǣ���Ϊ���Ӿ�
			return TRUE;
		else if (pc->length == 1) 
			return 2; //�������µĵ��Ӿ��򷵻�2
	}
	return FALSE;
}

/**
 * ���ݱ������Ϣ��ԭ�Ӿ��������ֱ���ȡ��������Ӿ��ǡ�ȡ�����ڵ��ڱ�������ֱ�ǡ�
 * ȡ������ı�Ԫ��ֵ������������Ӿ�
 * @param r
 * @param l
 * @param p
 */
void recover(root2 *r, liteNode2 *l, lvElem *p) {
	// ���ݱ������Ϣ��ԭ�Ӿ��������ֱ�
	// �����ݼ��µ�����ȡ���Ӿ���
	ptrNode *pn, *qn;
	clauseHead2 *pc;
	lvNode *pp;
	for (pn = p->cutChain->next; pn;) {
		if (pn->cutClause->head->next->index == 19) {
			rewind(stdin);
		}
		pn->cutClause->isDeleted = FALSE;
		qn = pn;
		pn = pn->next;
		free(qn);
	}
	p->cutChain->next = NULL;
	// �����������Ӿ��ҵ����ڵ��ڱ�������֣��Ѳ�����Ϊ0��
	for (pc = r->chainHead->next; pc; pc = pc->next) {
		for (pp = pc->head->next; pp; pp = pp->next) {
			if (pp->level == p->level) {
				pp->level = 0;
				l[pp->index].times++; // ���³��ִ���
				pc->length++;
			}
		}
	}
	// ��ԭ���ֱ�
	for (int i = 1; i <= p->liteList[0]; i++) {
		l[abs(p->liteList[i])].isAssigned = FALSE;
		//printf("deselected: %d\n", p->liteList[i]);
	}
	p->liteList = realloc(p->liteList, 2 * sizeof(int));
	// TODO: realloc����
	p->liteList[0] = 1;
	// ��������Ӿ�
	for (int i = 1; i < r->literalNum; i++) {
		l[i].isLenChecked = FALSE;
	}
	for (pc = r->chainHead->next; pc; pc = pc->next) {
		setLiteInfo(pc, l);
	}
}

int pickLiteOpt(liteNode2 *l, int num) {
	double score1, score2; // score1Ϊ��ǰ��߷֣�score2Ϊ��ǰ��Ԫ����
	int i, lite;
	//��score��lite��ֵ
	for (i = 1; l[i].isAssigned == TRUE && i <= num; i++);//�ҵ���һ��δ����ǵ�����
	score1 = l[i].times*pow(1. / 2, l[i].shortestLen); // TODO: score�ļ��㹫ʽ
	lite = i;
	for (i = 1; i <= num; i++) {
		if (l[i].isAssigned == TRUE) continue;
		score2 = l[i].times*pow(1. / 2, l[i].shortestLen);
		if (score2 > score1) {
			score1 = score2; // ���µ�ǰ��߷�
			lite = i;
		}
	}
	l[lite].isAssigned = TRUE;
	l[lite].value = TRUE;
	//printf("selected: %d\n", lite);
	return lite;
}

unsigned optDPLL(root2 *r, liteNode2 *l, liteNode2 **solution) {
	lvElem *stack, *p, *temp;
	unsigned level = 0; //��ǰ������Ҳ��ջ�ĳ���
	int flag = 0, lite, isNewUnit = TRUE, t; // flag���Ƿ�Ϊ��ջ��lite����ǰ��ѡ����
	// Ԥ����ɾ�����Ӿ����
	t = simpleClause2(l, r);
	if (t == TRUE) {
		*solution = l;
		return TRUE;
	}
	else if (t == FALSE) {
		return FALSE;
	}
	clauseHead2 *pc = r->chainHead->next;
	lite = pickLiteOpt(l, r->literalNum); //ѡ���ԪӦ���ڴ������ϸ�ѡ��֮��
	level=1;
	stack = malloc(level * sizeof(lvElem));
	if (!stack) return ERROR;
	//��һ��
	p = &stack[0];
	p->liteList = malloc(sizeof(int) * 2);
	p->liteList[0] = 1;
	p->liteList[1] = lite; //���㴦����ѡ��ı�Ԫ��ֵ�����������ݵ�������ΪFALSEʱ��ȷ���޽���
	p->cutChain = malloc(sizeof(ptrNode));
	p->cutChain->next = NULL;
	p->level = level;
	do {
		for (int i = 1; i < r->literalNum; i++) {
			l[i].isLenChecked = FALSE;
		}
		do { //��һ�λ�markLite�������µ��Ӿ�ʱѭ��
			isNewUnit = FALSE;
			// ���Ӿ����-ͬ����
			for (pc = r->chainHead->next; pc; pc = pc->next) {
				if (pc->isDeleted == FALSE) {
					markAndSaveClause(pc, l, p); //��ǵ��Ӿ估�����Ӿ���Ӿ�
				}
			}
			// �Ӿ����Ƿ�ȫ�����
			for (pc = r->chainHead->next; pc; pc = pc->next) {
				if (pc->isDeleted == FALSE) break;
			}
			if (!pc) {
				*solution = l;
				l[0].shortestLen = r->literalNum;
				//TODO: free...
				return TRUE;
			}
			// ���Ӿ����-������
			for (pc = r->chainHead->next; pc; pc = pc->next) {
				t = FALSE;
				if (pc->isDeleted == FALSE)t = markLite(pc, l, level); //tΪ��ʱ����
				if (t == 2) {
					isNewUnit = TRUE;
				}
				else if (t == TRUE) {
					//���ֿ��Ӿ�
					recover(r, l, p); //�ָ��������Ϣ
					if (p->liteList[1] > 0) { //����ѡ��TRUE����
						//��ѡFALSE
						l[abs(p->liteList[1])].isAssigned = TRUE;
						l[abs(p->liteList[1])].value = FALSE;
						p->liteList[1] = -p->liteList[1];
						//printf("selected: %d\n", p->liteList[1]);
						flag = 1;
					}
					else { //����ѡ����FALSE��Ӧ������һ���ѡ��ֱ������
						if (p != stack) {
							do { // ����
								//TODO: free...
								temp = realloc(stack, (--level) * sizeof(lvElem)); //��ջ
								if (!temp) return ERROR;
								stack = temp;
								p = &stack[level - 1];
								recover(r, l, p);
							} while (p != stack && p->liteList[1] < 0);
						}
						//��ʱҪô������Ҫô��TRUE
						if (p == stack && p->liteList[1] < 0) { //����ҲΪFALSE
							//TODO: free...
							return FALSE;
						}
						else {
							recover(r, l, p);
							p->liteList[1] = -p->liteList[1];
							l[abs(-p->liteList[1])].value = FALSE;
							l[abs(-p->liteList[1])].isAssigned = TRUE;
							flag = 1; //Ҫ�ص���һ��
						}
					}
				}
				if (flag == 1) break;
			}
			if (flag == 1) break;
		} while (isNewUnit == TRUE);//ȷ�ϼ����Ӿ�����Ϊ��Ҳû�п��Ӿ�
		//�Ƿ�Ϊ���ݣ������������½��²���
		if (flag == 1) {
			flag = 0;
			continue;
		}
		else {
			lite = pickLiteOpt(l, r->literalNum);
			temp = realloc(stack, (++level) * sizeof(lvElem)); //TODO: ���������⣬��һֱ���Ӳ���
			if (!temp) return ERROR;
			stack = temp;
			p = &stack[level - 1];
			p->liteList = malloc(sizeof(int) * 2);
			p->liteList[0] = 1;
			p->liteList[1] = lite; //�ȳ���TRUE
			p->cutChain = malloc(sizeof(ptrNode));
			p->cutChain->next = NULL;
			p->level = level;
		}
		end = clock();
		if ((end - start) / CLK_TCK > MAX_TIME) return ERROR;
	} while (1);
	return FALSE;
}

unsigned int checkSolution2(liteNode2 *solution, root2 *root1) {
	clauseHead2 *pc;
	lvNode *pn;
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