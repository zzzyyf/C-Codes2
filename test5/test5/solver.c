#include "funcs.h"
#include <math.h>

extern clock_t start, end;
/** 以下是单子句规则算法 **/
/**
* 删除deleteAfter的后继子句。
* @param deleteAfter
*/
void deleteClause(clauseHead *deleteAfter) {
	node *pn, *qn;
	clauseHead *pc; //待删除的子句，即deleteAfter的next
	pc = deleteAfter->next;
	//把pc从链中断开
	deleteAfter->next = deleteAfter->next->next;
	//先删除链头，独立出来是因为避免做检查是否为链头的if
	pn = pc->head, qn = pn->next;
	free(pn);
	pn = qn;
	if (qn)qn = qn->next;
	//再删除其他句
	for (; pn;) {
		free(pn);
		pn = qn;
		if (qn)qn = qn->next;
	}
	pc->next = NULL;
	free(pc);
}

/**
* 遍历deleteIn所指子句，并删除所有单子句的反文字，返回deleteIn所指子句的句长以便判断是否新增单子句。若isOpt为非零则为优化做准备。
* @param deleteIn
* @param liteHead
* @param isOpt
* @return
*/
int deleteReverseNode(clauseHead *deleteIn, liteNode *liteHead) {
	node *pn, *qn;
	int len = 0; //统计句长用
	for (pn = deleteIn->head, qn = pn->next; qn;) {
		if (liteHead[qn->index].isAssigned == TRUE && liteHead[qn->index].value != qn->value) { //qn的文字与指定文字互为反文字
			pn->next = qn->next;
			free(qn);
			qn = pn->next;
		}
		else { //qn文字不被删
			pn = qn;
			qn = qn->next;
			len++;
		}
	}
	return len;
}

/**
* 通过循环删除含有单子句相同文字或已被赋值文字的子句与删除所有单子句相反文字来简化SAT问题，DPLL的主要算法。
* 当简化至空子句链时返回TRUE，出现空子句时返回FALSE。其他情况返回2。
* @param liteHead
* @param root1
*/
unsigned int simpleClause(liteNode *liteHead, root *root1) {
	clauseHead *pc, *qc;
	int n = 1, deleted; //n标示删除文字后子句长，若长变为1说明需要再次删除单子句; deleted标示一次迭代是否删除了子句
	//clock_t start,end;
	node *qn;
	while (n == 1) {
		n = 0;
		//start=clock();
		for (pc = root1->chainHead, qc = pc->next; qc;) { //检查非空子句qc是否可被简化
			deleted = 0;
			//先检查是否为单子句
			if (qc->head->next && qc->head->next->next == NULL) { //若qc需要被删除
				if (liteHead[qc->head->next->index].isAssigned != TRUE ||
					liteHead[qc->head->next->index].value == qc->head->next->value) { //若该变元已被赋值且目前的值与文字表值相反，说明待会儿要删成空子句，不能提前删除
					//标记qc内的变元
					liteHead[qc->head->next->index].isAssigned = TRUE;
					liteHead[qc->head->next->index].value = qc->head->next->value;
					//printf("    单子句");
					//if (liteHead[qc->head->next->index].value == FALSE) putchar('-');
					//printf("%d。\n", qc->head->next->index);
					deleteClause(pc); //删除pc->next即qc
					deleted++;
				}
			}
			else {
				//再检查是否含单子句的文字
				for (qn = qc->head->next; qn;) {
					if (liteHead[qn->index].isAssigned == TRUE &&
						liteHead[qn->index].value == qn->value) { //qn的文字在单子句中出现
						deleteClause(pc); //删除pc->next即qc
						deleted++;
						break; //删完即可跳出
					}
					else { //qn未在单子句中出现过
						qn = qn->next;
					}
				}
			}
			//删除了子句的循环出口
			if (deleted) qc = pc->next; //qc原来指向的已经被删除了
			//最后的循环出口
			if (!deleted) {
				//再检查qc是否含单子句的反文字
				if (n != 1) n = deleteReverseNode(qc, liteHead); //遍历删除所有单子句的反文字
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

/** 以下是DPLL算法 **/
/**
* 随机选取一个未被赋值的文字
* @param liteHead
* @param liteNum
* @return 选取的文字
*/
int pickLiteRandom(liteNode *liteHead, int liteNum) {
	int index;
	srand((unsigned)time(0));
	while (liteHead[index = liteNum * rand() / (RAND_MAX + 1u) + 1].isAssigned == TRUE); //若index已被赋值则重新选择
	return index;
}

int pickLiteSeq(liteNode *liteHead, int liteNum) {
	int index = 1;
	while (index < (liteNum + 1) && liteHead[index].isAssigned == TRUE)index++; //若index已被赋值则重新选择
	return index;
}

root *copyChain(root *root1) {
	//先复制网头
	root *r = malloc(sizeof(root));
	if (!r) return NULL;
	r->clauseNum = root1->clauseNum;
	r->literalNum = root1->literalNum;
	//再新建链头（无重要信息，无需复制，新建即可）
	r->chainHead = malloc(sizeof(chainHead)); //给链头分配空间
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
		p[i].isLenChecked = FALSE; //每次DPLL之前，最短子句都是未确定的
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
	//printf("%d：\n", lite);
	//保存当前层的数据结构
	root *r = copyChain(root1);
	liteNode *l = copyLite(liteHead, r->literalNum);
	unsigned result;
	l[lite].value = value; l[lite].isAssigned = TRUE;
	result = simpleClause(l, r);
	if (!r->chainHead->next) { //若得到子句链为空说明有解
		free(r->chainHead); //删除链头
		*solution = l; //保存解的指针
		l[0].shortestLen = r->literalNum; //保存变元个数
		free(r); //删除网头
		return TRUE;
	}
	else if (result == FALSE) { //若出现空子句说明此情况无解
		for (pc = r->chainHead; pc->next; deleteClause(pc)); //删除chainHead后面的所有子句
		free(pc);
		free(l);
		free(r);
		return FALSE;
	}
	else { //需要再取变量化简
		//lite = pickLiteRandom(l, root1->literalNum); //随机选取一个未被赋值的文字
		lite = pickLiteSeq(l, root1->literalNum);
		if (DPLL(r, l, solution, lite, TRUE) == TRUE) {
			for (pc = r->chainHead; pc->next; deleteClause(pc)); //删除chainHead后面的所有子句
			free(pc);
			free(r);
			free(l);
			return TRUE;
		}
		else {
			result = DPLL(r, l, solution, lite, FALSE);
			for (pc = r->chainHead; pc->next; deleteClause(pc)); //删除chainHead后面的所有子句
			free(pc);
			r->chainHead = NULL;
			free(r);
			free(l);
			return result;
		}
	}

}

/**
* 返回求出解不成立的行数，若成立返回0.
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
				i = 1; //代入为假就继续下一个，代入为真则验证成立
				break;
			}
		}
		if (!i) return n; //若整句均为假则不成立
	}
	return 0; //执行到最后说明全为真
}

