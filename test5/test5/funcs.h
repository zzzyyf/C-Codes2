/**
 * funcs.h
 * 定义在SAT-DPLL中所有的共享函数
 */

#ifndef SAT_DPLL_FUNCS_H
#define SAT_DPLL_FUNCS_H

#endif //SAT_DPLL_FUNCS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define OK 1
#define ERROR 2
#define TRUE 1
#define FALSE 0
#define MAX_LITE_NUM 1023 //SAT问题的最大文字数，当前为10bit
#define MAX_FILE_NAME 100 //文件名（含后缀）的最大长度
#define MAX_CLUS_NUM 524287 //SAT问题的最大子句数，当前为19bit
#define MAX_TIME 30 //超时阈值，单位为秒

 /** 下面定义一系列数据结构 **/
typedef int literal; //文字

typedef struct Node {
	literal index:10; //文字在文字列表的索引，当结点作为子句头结点时值为-1
	unsigned int value : 1; //文字在此句中的正反，TRUE为正，FALSE为反
	struct Node *next; //子句中下个文字的指针，若子句结束则为NULL
} node; //子句结点

typedef struct clausHead {
	//TODO: length的位字段为宏MAX_LITE_NUM所占位数
	node *head; //子句首的指针
	struct clausHead *next; //下个子句
} clauseHead, chainHead; //子句头结点

typedef struct {
	unsigned int clauseNum : 19; //子句数
	unsigned int literalNum : 10; //文字数
	clauseHead *chainHead; //首个子句的头指针
} root; //子句“网”的头结点

typedef struct {
	unsigned int shortestLen : 10; //该文字出现的最短子句的长度
	unsigned int isLenChecked : 1; //在优化算法中，经过单子句+分裂后是否确定过新的shortestLen，TRUE为确定，FALSE为未确定
	unsigned int isAssigned : 1; //该文字是否被剪枝，FALSE为未被剪枝，TRUE为已被剪枝
	unsigned int value : 1; //该文字当前的真值，TRUE为真FALSE为假，isAssigned为FALSE时无用
}liteNode; //文字结点

/** 下面是优化算法所用的数据结构 **/
typedef struct levelNode {
	unsigned int level : 10; //被标记的层数，即>=level可认为此节点不存在，0为未被标记
	literal index:10; //文字在文字列表的索引，当结点作为子句头结点时值为-1
	unsigned int value : 1; //文字在此句中的正反，TRUE为正，FALSE为反
	struct levelNode *next; //子句中下个文字的指针，若子句结束则为NULL
} lvNode; //带层信息的子句结点

typedef struct clausHead2 {
	//TODO: length的位字段为宏MAX_LITE_NUM所占位数
	unsigned int length : 10; //子句长度，当结点作为子句链头结点时值为-1
	unsigned int isDeleted : 1; //是否被剪枝，FALSE为存在，TRUE为被剪枝
	lvNode *head; //子句首的指针
	struct clausHead2 *next; //下个子句
} clauseHead2, chainHead2; //子句头结点

typedef struct {
	int clauseNum; //子句数
	int literalNum; //文字数
	clauseHead2 *chainHead; //指向子句链的链头
} root2;
typedef struct ptrnode {
	clauseHead2 *cutClause; //指向被剪枝子句的指针，首结点为头结点
	struct ptrnode *next; //下个被剪枝子句
} ptrNode; //存放指向被剪枝子句的指针的结点

typedef struct {
	unsigned int level : 10; //层数
	unsigned int value : 1; //选中文字的真值，TRUE or FALSE
	ptrNode *cutChain; //本层被剪枝的子句
	int *liteList; //本层的单子句文字动态数组，首个元素为数组长，第二个为当前层选择的变元，正负为其值
}lvElem; //存放每个决策层的信息

typedef struct {
	int times; //该文字在所有子句中出现的次数
	unsigned int shortestLen : 10; //该文字出现的最短子句的长度
	//isLenChecked用于确保在标记子句或文字后shortestLen不会在已经更新的情况下被增大
	unsigned int isLenChecked : 1; //在优化算法中，经过单子句+分裂后shortestLen是否仍存在，TRUE为确定，FALSE为未确定
	unsigned int isAssigned : 1; //该文字是否被剪枝，FALSE为未被剪枝，TRUE为已被剪枝
	unsigned int value : 1; //该文字当前的真值，TRUE为真FALSE为假，isAssigned为FALSE时无用
}liteNode2; //文字结点

/** 以下是公共函数 **/
liteNode *read(char *filename, root *root1);
clauseHead2 *addClause2(clauseHead2 *addAfter);
lvNode *addNode2(clauseHead2 *addIn, literal index, unsigned int value)
int traverse(char *filename, root *root1);
void deleteClause(clauseHead *deleteAfter);
unsigned int DPLL(root *root1, liteNode *liteHead, liteNode **solution, int lite, unsigned value);
int output(liteNode *solution, double time, char *filename);
unsigned int checkSolution(liteNode *solution, root *root1);
int analyze1(char *filename, root *root1);

liteNode2 *read2(char *filename, root2 *root1);
int traverse2(char *filename, root2 *root1);
unsigned optDPLL(root2 *root1, liteNode2 *liteHead, liteNode2 **solution);
int output2(liteNode2 *solution, double time, char *filename);
unsigned int checkSolution2(liteNode2 *solution, root2 *root1);
void setLiteInfo(clauseHead2 *pc, liteNode2 *l);
clock_t start, end;
