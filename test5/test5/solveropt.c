#include "funcs.h"
#include <math.h>
/** 以下是优化算法 **/

extern clock_t start, end;
/**
* 遍历deleteIn所指子句，并删除所有单子句的反文字，返回deleteIn所指子句的句长以便判断是否新增单子句。若isOpt为非零则为优化做准备。
* @param deleteIn
* @param liteHead
* @param isOpt
* @return
*/
int deleteReverseNode2(clauseHead2 *deleteIn, liteNode2 *liteHead) {
	lvNode *pn, *qn;
	unsigned len = 0; //统计句长用
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
	deleteIn->length = len;
	setLiteInfo(deleteIn, liteHead);
	return len;
}

/**
* 删除deleteAfter的后继子句。
* @param deleteAfter
*/
void deleteClause2(clauseHead2 *deleteAfter, liteNode2 *l) {
	lvNode *pn, *qn;
	clauseHead2 *pc; //待删除的子句，即deleteAfter的next
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
		l[pn->index].times--;
		free(pn);
		pn = qn;
		if (qn)qn = qn->next;
	}
	pc->next = NULL;
	free(pc);
}

/**
* 通过循环删除含有单子句相同文字或已被赋值文字的子句与删除所有单子句相反文字来简化SAT问题，DPLL的主要算法。
* 当简化至空子句链时返回TRUE，否则返回FALSE。
* @param liteHead
* @param root1
* @todo unfinished
*/
unsigned int simpleClause2(liteNode2 *l, root2 *r) {
	clauseHead2 *pc, *qc;
	int n = 1, deleted; //n标示删除文字后子句长，若长变为1说明需要再次删除单子句; deleted标示一次迭代是否删除了子句
	lvNode *qn;
	while (n == 1) {
		n = 0;
		for (pc = r->chainHead, qc = pc->next; qc;) { //检查非空子句qc是否可被简化
			deleted = 0;
			//先检查是否为单子句
			if (qc->head->next && qc->head->next->next == NULL) { //若qc需要被删除
				if (l[qc->head->next->index].isAssigned != TRUE ||
					l[qc->head->next->index].value == qc->head->next->value) { //若该变元已被赋值且目前的值与文字表值相反，说明待会儿要删成空子句，不能提前删除
					//标记qc内的变元
					l[qc->head->next->index].isAssigned = TRUE;
					l[qc->head->next->index].value = qc->head->next->value;
					deleteClause2(pc, l); //删除pc->next即qc
					deleted++;
				}
			}
			else {
				//再检查是否含单子句的文字
				for (qn = qc->head->next; qn;) {
					if (l[qn->index].isAssigned == TRUE &&
						l[qn->index].value == qn->value) { //qn的文字在单子句中出现
						setLiteInfo(qc, l);
						deleteClause2(pc, l); //删除pc->next即qc
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
				if (n != 1) {
					n = deleteReverseNode2(qc, l); //遍历删除所有单子句的反文字
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
 * 更新pc句中未被赋值的文字的最短子句长。
 * @param pc
 * @param l
 * @param level
 * @return
 */
void setLiteInfo(clauseHead2 *pc, liteNode2 *l) {
	lvNode *pn = pc->head->next;
	if (pc->isDeleted == FALSE) {
		//pc未被删除时，需要检查本句是否为未赋值文字的新最短子句
		for (; pn; pn = pn->next) {
			if (l[pn->index].isAssigned == FALSE &&
				l[pn->index].shortestLen > pc->length) {//该文字未被赋值且该句长更小
				if (l[pn->index].isLenChecked == FALSE)l[pn->index].isLenChecked = TRUE;
				l[pn->index].shortestLen = pc->length;
			}
		}
	}
	else {
		//pc已被删除，检查本句是否不再是未赋值文字的最短子句
		for (; pn; pn = pn->next) {
			if (l[pn->index].isAssigned == FALSE &&
				l[pn->index].shortestLen <= pc->length) { //该文字未被赋值且该句长更小
				if (l[pn->index].isLenChecked == FALSE) {
					l[pn->index].isLenChecked = TRUE;
					l[pn->index].shortestLen = MAX_LITE_NUM;
				}
			}
		}
	}
}
/**
 * 标记含有已被赋值文字的子句。注意这不是删除单子句的函数，而是标记的函数。
 * @param pc
 * @param l
 * @param p
 * @return ERROR：malloc失败； OK：正常
 */
int markAndSaveClause(clauseHead2 *pc, liteNode2 *l, lvElem *p) {
	lvNode *pn = pc->head->next;
	ptrNode *pp = p->cutChain;
	unsigned isDeleted = FALSE;
	//若pc是单子句
	if (pc->length == 1) {
		for (pn = pc->head->next; pn->level != 0; pn = pn->next); //遍历至单子句的结点
		//该句只有一个位于该层的结点，即为该层的单子句，而且要么未被赋值，要么已被赋值且不是反文字
		if (l[pn->index].isAssigned == TRUE && l[pn->index].value == pn->value || l[pn->index].isAssigned == FALSE) {
			pc->isDeleted = TRUE;
			if (l[pn->index].isAssigned == FALSE) { //要新增一个单子句
				//保存新增单子句信息
				p->liteList = realloc(p->liteList, sizeof(int)*(++p->liteList[0] + 1));
				p->liteList[p->liteList[0]] = pn->index; //在链尾增加新增文字
				if (pn->value == FALSE) p->liteList[p->liteList[0]] = -pn->index;
				//printf("selected: %d\n", p->liteList[p->liteList[0]]);
				l[pn->index].isAssigned = TRUE;
				l[pn->index].value = pn->value;
			}
			isDeleted = TRUE;
			//保存删除的子句位置
			for (; pp->next; pp = pp->next); //遍历至链尾
			pp->next = malloc(sizeof(ptrNode));
			if (!pp->next)
				return ERROR;
			pp = pp->next;
			pp->cutClause = pc;
			pp->next = NULL;
		}
	}
	else { //pc不是单子句时
		//遍历查找已被赋值的文字
		for (; pn; pn = pn->next) {
			if (pn->level==0 && l[pn->index].isAssigned == TRUE && l[pn->index].value == pn->value) {
				pc->isDeleted = TRUE;
				isDeleted = TRUE;
				//保存删除的子句位置
				for (; pp->next; pp = pp->next); //遍历至链尾
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
		setLiteInfo(pc, l); //若有未定的shortestLen则更改之
	}
	return OK;
}

/**
 *
 * @param pc
 * @param l
 * @param level
 * @return TRUE：发现空子句；2：发现新增单子句；FALSE：非以上二者
 */
int markLite(clauseHead2 *pc, liteNode2 *l, unsigned int level) {
	lvNode *pn; //pn是可能需要标记的文字，pn是qn前的文字
	int changed = 0;
	//只需检查未被标记的变元即可
	for (pn = pc->head; pn; pn=pn->next) {
		if (pn->level == 0 && l[pn->index].isAssigned == TRUE && l[pn->index].value!=pn->value) { //在pn->level之前的都已排过序，在之后的还未被标记
			pn->level = level;
			l[pn->index].times--;
			pc->length--;
			changed++;
		}
	}
	if (changed) {
		setLiteInfo(pc, l);
		if (pc->length == 0) //若pc的所有文字均被标记，成为空子句
			return TRUE;
		else if (pc->length == 1) 
			return 2; //若发现新的单子句则返回2
	}
	return FALSE;
}

/**
 * 根据本层的信息还原子句链和文字表，即取消本层的子句标记、取消大于等于本层的文字标记、
 * 取消本层的变元赋值，并更新最短子句
 * @param r
 * @param l
 * @param p
 */
void recover(root2 *r, liteNode2 *l, lvElem *p) {
	// 根据本层的信息还原子句链和文字表
	// 即根据记下的链表取消子句标记
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
	// 并遍历所有子句找到大于等于本层的文字，把层数改为0。
	for (pc = r->chainHead->next; pc; pc = pc->next) {
		for (pp = pc->head->next; pp; pp = pp->next) {
			if (pp->level == p->level) {
				pp->level = 0;
				l[pp->index].times++; // 更新出现次数
				pc->length++;
			}
		}
	}
	// 还原文字表
	for (int i = 1; i <= p->liteList[0]; i++) {
		l[abs(p->liteList[i])].isAssigned = FALSE;
		//printf("deselected: %d\n", p->liteList[i]);
	}
	p->liteList = realloc(p->liteList, 2 * sizeof(int));
	// TODO: realloc处理
	p->liteList[0] = 1;
	// 更新最短子句
	for (int i = 1; i < r->literalNum; i++) {
		l[i].isLenChecked = FALSE;
	}
	for (pc = r->chainHead->next; pc; pc = pc->next) {
		setLiteInfo(pc, l);
	}
}

int pickLiteOpt(liteNode2 *l, int num) {
	double score1, score2; // score1为当前最高分，score2为当前变元分数
	int i, lite;
	//给score和lite初值
	for (i = 1; l[i].isAssigned == TRUE && i <= num; i++);//找到第一个未被标记的文字
	score1 = l[i].times*pow(1. / 2, l[i].shortestLen); // TODO: score的计算公式
	lite = i;
	for (i = 1; i <= num; i++) {
		if (l[i].isAssigned == TRUE) continue;
		score2 = l[i].times*pow(1. / 2, l[i].shortestLen);
		if (score2 > score1) {
			score1 = score2; // 更新当前最高分
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
	unsigned level = 0; //当前层数，也是栈的长度
	int flag = 0, lite, isNewUnit = TRUE, t; // flag：是否为退栈；lite：当前所选文字
	// 预处理，删除单子句相关
	t = simpleClause2(l, r);
	if (t == TRUE) {
		*solution = l;
		return TRUE;
	}
	else if (t == FALSE) {
		return FALSE;
	}
	clauseHead2 *pc = r->chainHead->next;
	lite = pickLiteOpt(l, r->literalNum); //选择变元应该在处理了上个选择之后
	level=1;
	stack = malloc(level * sizeof(lvElem));
	if (!stack) return ERROR;
	//第一层
	p = &stack[0];
	p->liteList = malloc(sizeof(int) * 2);
	p->liteList[0] = 1;
	p->liteList[1] = lite; //本层处理本层选择的变元和值，这样当回溯到顶层仍为FALSE时就确定无解了
	p->cutChain = malloc(sizeof(ptrNode));
	p->cutChain->next = NULL;
	p->level = level;
	do {
		for (int i = 1; i < r->literalNum; i++) {
			l[i].isLenChecked = FALSE;
		}
		do { //第一次或markLite制造了新单子句时循环
			isNewUnit = FALSE;
			// 单子句策略-同文字
			for (pc = r->chainHead->next; pc; pc = pc->next) {
				if (pc->isDeleted == FALSE) {
					markAndSaveClause(pc, l, p); //标记单子句及含单子句的子句
				}
			}
			// 子句链是否全部标记
			for (pc = r->chainHead->next; pc; pc = pc->next) {
				if (pc->isDeleted == FALSE) break;
			}
			if (!pc) {
				*solution = l;
				l[0].shortestLen = r->literalNum;
				//TODO: free...
				return TRUE;
			}
			// 单子句策略-反文字
			for (pc = r->chainHead->next; pc; pc = pc->next) {
				t = FALSE;
				if (pc->isDeleted == FALSE)t = markLite(pc, l, level); //t为临时变量
				if (t == 2) {
					isNewUnit = TRUE;
				}
				else if (t == TRUE) {
					//发现空子句
					recover(r, l, p); //恢复本层的信息
					if (p->liteList[1] > 0) { //本层选择TRUE有误
						//改选FALSE
						l[abs(p->liteList[1])].isAssigned = TRUE;
						l[abs(p->liteList[1])].value = FALSE;
						p->liteList[1] = -p->liteList[1];
						//printf("selected: %d\n", p->liteList[1]);
						flag = 1;
					}
					else { //本层选择了FALSE，应更改上一层的选择，直到顶层
						if (p != stack) {
							do { // 回溯
								//TODO: free...
								temp = realloc(stack, (--level) * sizeof(lvElem)); //退栈
								if (!temp) return ERROR;
								stack = temp;
								p = &stack[level - 1];
								recover(r, l, p);
							} while (p != stack && p->liteList[1] < 0);
						}
						//此时要么到顶，要么到TRUE
						if (p == stack && p->liteList[1] < 0) { //顶层也为FALSE
							//TODO: free...
							return FALSE;
						}
						else {
							recover(r, l, p);
							p->liteList[1] = -p->liteList[1];
							l[abs(-p->liteList[1])].value = FALSE;
							l[abs(-p->liteList[1])].isAssigned = TRUE;
							flag = 1; //要回到上一层
						}
					}
				}
				if (flag == 1) break;
			}
			if (flag == 1) break;
		} while (isNewUnit == TRUE);//确认既是子句链不为空也没有空子句
		//是否为回溯，若不回溯则新建下层结点
		if (flag == 1) {
			flag = 0;
			continue;
		}
		else {
			lite = pickLiteOpt(l, r->literalNum);
			temp = realloc(stack, (++level) * sizeof(lvElem)); //TODO: 回溯有问题，会一直增加层数
			if (!temp) return ERROR;
			stack = temp;
			p = &stack[level - 1];
			p->liteList = malloc(sizeof(int) * 2);
			p->liteList[0] = 1;
			p->liteList[1] = lite; //先尝试TRUE
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
				i = 1; //代入为假就继续下一个，代入为真则验证成立
				break;
			}
		}
		if (!i) return n; //若整句均为假则不成立
	}
	return 0; //执行到最后说明全为真
}