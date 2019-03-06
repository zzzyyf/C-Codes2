/**
 * funcs.h
 * ������SAT-DPLL�����еĹ�����
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
#define MAX_LITE_NUM 1023 //SAT������������������ǰΪ10bit
#define MAX_FILE_NAME 100 //�ļ���������׺������󳤶�
#define MAX_CLUS_NUM 524287 //SAT���������Ӿ�������ǰΪ19bit
#define MAX_TIME 30 //��ʱ��ֵ����λΪ��

 /** ���涨��һϵ�����ݽṹ **/
typedef int literal; //����

typedef struct Node {
	literal index:10; //�����������б���������������Ϊ�Ӿ�ͷ���ʱֵΪ-1
	unsigned int value : 1; //�����ڴ˾��е�������TRUEΪ����FALSEΪ��
	struct Node *next; //�Ӿ����¸����ֵ�ָ�룬���Ӿ������ΪNULL
} node; //�Ӿ���

typedef struct clausHead {
	//TODO: length��λ�ֶ�Ϊ��MAX_LITE_NUM��ռλ��
	node *head; //�Ӿ��׵�ָ��
	struct clausHead *next; //�¸��Ӿ�
} clauseHead, chainHead; //�Ӿ�ͷ���

typedef struct {
	unsigned int clauseNum : 19; //�Ӿ���
	unsigned int literalNum : 10; //������
	clauseHead *chainHead; //�׸��Ӿ��ͷָ��
} root; //�Ӿ䡰������ͷ���

typedef struct {
	unsigned int shortestLen : 10; //�����ֳ��ֵ�����Ӿ�ĳ���
	unsigned int isLenChecked : 1; //���Ż��㷨�У��������Ӿ�+���Ѻ��Ƿ�ȷ�����µ�shortestLen��TRUEΪȷ����FALSEΪδȷ��
	unsigned int isAssigned : 1; //�������Ƿ񱻼�֦��FALSEΪδ����֦��TRUEΪ�ѱ���֦
	unsigned int value : 1; //�����ֵ�ǰ����ֵ��TRUEΪ��FALSEΪ�٣�isAssignedΪFALSEʱ����
}liteNode; //���ֽ��

/** �������Ż��㷨���õ����ݽṹ **/
typedef struct levelNode {
	unsigned int level : 10; //����ǵĲ�������>=level����Ϊ�˽ڵ㲻���ڣ�0Ϊδ�����
	literal index:10; //�����������б���������������Ϊ�Ӿ�ͷ���ʱֵΪ-1
	unsigned int value : 1; //�����ڴ˾��е�������TRUEΪ����FALSEΪ��
	struct levelNode *next; //�Ӿ����¸����ֵ�ָ�룬���Ӿ������ΪNULL
} lvNode; //������Ϣ���Ӿ���

typedef struct clausHead2 {
	//TODO: length��λ�ֶ�Ϊ��MAX_LITE_NUM��ռλ��
	unsigned int length : 10; //�Ӿ䳤�ȣ��������Ϊ�Ӿ���ͷ���ʱֵΪ-1
	unsigned int isDeleted : 1; //�Ƿ񱻼�֦��FALSEΪ���ڣ�TRUEΪ����֦
	lvNode *head; //�Ӿ��׵�ָ��
	struct clausHead2 *next; //�¸��Ӿ�
} clauseHead2, chainHead2; //�Ӿ�ͷ���

typedef struct {
	int clauseNum; //�Ӿ���
	int literalNum; //������
	clauseHead2 *chainHead; //ָ���Ӿ�������ͷ
} root2;
typedef struct ptrnode {
	clauseHead2 *cutClause; //ָ�򱻼�֦�Ӿ��ָ�룬�׽��Ϊͷ���
	struct ptrnode *next; //�¸�����֦�Ӿ�
} ptrNode; //���ָ�򱻼�֦�Ӿ��ָ��Ľ��

typedef struct {
	unsigned int level : 10; //����
	unsigned int value : 1; //ѡ�����ֵ���ֵ��TRUE or FALSE
	ptrNode *cutChain; //���㱻��֦���Ӿ�
	int *liteList; //����ĵ��Ӿ����ֶ�̬���飬�׸�Ԫ��Ϊ���鳤���ڶ���Ϊ��ǰ��ѡ��ı�Ԫ������Ϊ��ֵ
}lvElem; //���ÿ�����߲����Ϣ

typedef struct {
	int times; //�������������Ӿ��г��ֵĴ���
	unsigned int shortestLen : 10; //�����ֳ��ֵ�����Ӿ�ĳ���
	//isLenChecked����ȷ���ڱ���Ӿ�����ֺ�shortestLen�������Ѿ����µ�����±�����
	unsigned int isLenChecked : 1; //���Ż��㷨�У��������Ӿ�+���Ѻ�shortestLen�Ƿ��Դ��ڣ�TRUEΪȷ����FALSEΪδȷ��
	unsigned int isAssigned : 1; //�������Ƿ񱻼�֦��FALSEΪδ����֦��TRUEΪ�ѱ���֦
	unsigned int value : 1; //�����ֵ�ǰ����ֵ��TRUEΪ��FALSEΪ�٣�isAssignedΪFALSEʱ����
}liteNode2; //���ֽ��

/** �����ǹ������� **/
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
