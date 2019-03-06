/**
 * cnfparser.c
 * 负责读取并解析.cnf文件中的合取范式，使之成为能被DPLL算法处理的问题
 * 还根据任务书提供输出内部结构的函数traverse，以便验证读取是否正确。
 */
#include "funcs.h"
#include <limits.h>
#include <wchar.h>

 /**
  * 以.cnf输出内部解析结构至out文件流。
  * @param filename
  * @param root1
  * @return
  */
int traverse(char *filename, root *root1) {
	FILE *out = fopen(filename, "w");
	if (!out) return ERROR;
	clauseHead *pc;
	node *pn;
	fprintf(out, "p cnf %d %d\n", root1->literalNum, root1->clauseNum);
	for (pc = root1->chainHead->next; pc; pc = pc->next) { //输出未被剪枝的子句
		for (pn = pc->head->next; pn; pn = pn->next) {
			if (pn->value == FALSE)putc('-', out);
			fprintf(out, "%d ", pn->index);
		}
		fprintf(out, "0\n");
	}
	fclose(out);
	return OK;
}

int analyze1(char *filename, root *root1) {
	FILE *out = fopen(filename, "wb"); //以二进制方式打开便于输出unicode
	if (!out) return ERROR;
	clauseHead *pc;
	node *pn;
	for (pc = root1->chainHead->next; pc; pc = pc->next) { //输出未被剪枝的子句
		for (pn = pc->head->next; pn; pn = pn->next) {
			if (pn->value == FALSE) {
				fwprintf(out, L"¬");
			}
			fwprintf(out, L"%d", pn->index);
			if(pn->next) fwprintf(out, L"∨");
		}
		if(pc->next) fwprintf(out, L"\n");
	}
	fclose(out);
	return OK;
}

/**
 * 在addAfter后添加一个带头结点的空子句并返回新添加空子句的指针。
 * @param addAfter 新子句添加在其后
 * @return 新添加子句的指针
 */
clauseHead *addClause(clauseHead *addAfter) {
	clauseHead *p, *q = addAfter->next;
	node *pl;
	p = malloc(sizeof(clauseHead));
	if (!p) return NULL;
	pl = malloc(sizeof(node));
	if (!pl) return NULL;
	p->head = pl;
	//连接至子句网
	p->next = addAfter->next;
	addAfter->next = p;
	//初始化头结点
	pl->value = TRUE, pl->index = -1;
	pl->next = NULL;
	return p;
}
/**
 * 在addIn子句末端添加一个真值为value的index文字，并返回该文字结点指针。
 * @param addIn
 * @param index
 * @param value
 * @return
 */
node *addNode(clauseHead *addIn, literal index, unsigned int value) {
	node *p = addIn->head, *q;
	for (; p->next; p = p->next); //遍历到子句内的末端空处
	q = malloc(sizeof(node));
	if (!q) return NULL;
	q->index = index; q->value = value;
	q->next = NULL;
	p->next = q;
	return q;
}
/**
 * 读取cnf文件。
 * @param filename 文件名的字符串
 * @param root1 子句链的链头
 * @param isOpt 是否采用优化算法，采用为非0，不采用为0
 * @return 若读取失败返回NULL，否则返回文字表的表头
 */
liteNode *read(char *filename, root *root1) {
	FILE *in = fopen(filename, "r");
	char t2[200], *pc1 = t2, *pc2;
	unsigned liteNum, clauseNum;
	if (!in) return NULL; //读取文件失败
	//读取文件头
	while (fscanf(in, "%[c]", t2)) {
		while (fscanf(in, "%*[^\n]")); //跳过注释
		fscanf(in, "%*[\n]");
	}
	if (fscanf(in, "p cnf %d%d", &liteNum, &clauseNum)) {//读取文件头
		root1->literalNum = liteNum;
		root1->clauseNum = clauseNum;
		printf("读取到cnf文件\n");
	}
	else return NULL; //读取不到文件头

	//初始化文字表
	liteNode *liteHead = (liteNode *)malloc(sizeof(liteNode)*(root1->literalNum + 1)); //给文字分配空间
	if (!liteHead) return NULL;
	for (int i = 1; i < root1->literalNum + 1; i++) {
		//liteHead[i].times=0;
		liteHead[i].isAssigned = FALSE;
		liteHead[i].shortestLen = MAX_LITE_NUM;
	}
	literal temp, ab; //检测每个子句中的文字用，ab为temp的绝对值

	//分配子句链的头结点
	root1->chainHead = malloc(sizeof(clauseHead));
	if (!root1->chainHead) return NULL;
	root1->chainHead->head = NULL;

	clauseHead *pChain = NULL, *q; //子句链上的指针，p代表当前与子句网相连的结点
	//生成第一个子句头
	pChain = malloc(sizeof(clauseHead)); //分配空间
	if (!pChain) return NULL;
	pChain->next = NULL; //初始化
	root1->chainHead->next = pChain; //把第一个子句头与网相连

	pChain->head = malloc(sizeof(node)); //生成第一个文字头
	if (!pChain->head) return NULL;
	node *p = pChain->head; //子句内的指针，p代表当前与子句网相连的结点
	p->next = NULL; p->index = -1; //初始化

	while (fgets(pc1, sizeof(t2) - 1, in)) { //TODO: 文件尾有注释行的能否处理需测试
		//以下部分用strtol逐个解析一次读取到的文字。
		for (temp = strtol(pc1, &pc2, 10); pc2 != pc1; temp = strtol(pc1, &pc2, 10)) {
			if (temp) {//若i不为0
				ab = abs(temp);
				addNode(pChain, ab, temp > 0 ? TRUE : FALSE);
			}
			else {
				q = pChain;
				pChain = addClause(pChain);
			}
			pc1 = pc2; //继续读取下一个数字
		}
		pc1 = t2; //每次读取完一行就要从t2开头重新读取
	}
	//删去多余的头结点
	free(q->next);
	q->next = NULL;
	printf("读取完成。");
	fclose(in);
	return liteHead;
}
