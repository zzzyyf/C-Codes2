/* Linear Table On Linked Structure */
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

/*---------page 10 on textbook ---------*/
#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR -3
#define OVERFLOW -2
#define MAX_LIST 100

typedef int status;
typedef int ElemType; //数据元素类型定义

/*-------page 22 on textbook -------*/
typedef struct LNode{  //顺序表（链式结构）节点的定义
    ElemType elem;
    struct LNode * next;
}Node;
typedef struct { //顺序表（链式结构）表头定义, head为NULL且length==0时为空表
    Node *head;
    int length;
}LinkList;

/*-----page 19 on textbook ---------*/
status IntiaList(LinkList *L);
status DestroyList(LinkList *L);
status ClearList(LinkList *L);
status ListEmpty(LinkList L);
int ListLength(LinkList L);
status GetElem(LinkList *L,int i,ElemType *e);
status LocateElem(LinkList L,ElemType e, status(*compare)(ElemType ,ElemType) );
status PriorElem(LinkList L,ElemType cur,ElemType * pre_e);
status NextElem(LinkList L,ElemType cur,ElemType * next_e);
status ListInsert(LinkList *L,int i,ElemType e);
status ListDelete(LinkList *L,int i,ElemType *e);
status ListTraverse(LinkList L, status(*visit)(ElemType));
status ReadFromFile(FILE *p, LinkList *L);
status SaveToFile(FILE *p, LinkList *L);
status Compare(ElemType e, ElemType e2){
    return e==e2;
}
status Visit(ElemType e){
    printf("%d ", e);
}
status HandleInput(char *input, char **str_end, ElemType *e);
/**
 * 获取用户的输入，判断是否越界，并返回指向一块存储用户输入结果的区域的指针且用0表示输入结束，或返回ERROR。
 * @param n
 * @return
 */
int *ChooseList(int n);
/*--------------------------------------------*/
void main(void){
    int op=1;//选择操作用
    int i=0,result, n=-1;//result用于存储函数返回的status，n用于指定操作的表数目
    int * selectedLists=NULL; //存储chooselist返回的动态表指针数组
    ElemType e;
    char c, input[11], *str_end=&input[0];//仅支持输入10位正数和9位负数
    char filename[10];
    FILE *in=NULL;
    status (*compare)(ElemType, ElemType)=Compare;
    status (*visit)(ElemType)=Visit;

    //指定需要管理的表数目
    while(n<0) {
        printf("请输入需要管理的表的数目（1-%d，输入0退出）：", MAX_LIST);
        if(scanf("%d", &n)!=1||n<0||n>MAX_LIST) {
            printf("输入错误，请检查！\n");
            n=-1;
        }
        else if(n==0) exit(0);
        rewind(stdin);
    }
    //初始化表
    LinkList Lists[n];
    for (i=0;i<n;i++){
        Lists[i].head=NULL;Lists[i].length=0;
    }

    //进入系统
    while(op){
        system("cls");
        printf("\n\n");
        printf("      Menu for Linear Table On Linked Structure \n");
        printf("-------------------------------------------------\n");
        printf("    	  1. IntiaList    \t7. LocateElem\n");
        printf("    	  2. DestroyList  \t8. PriorElem\n");
        printf("    	  3. ClearList    \t9. NextElem \n");
        printf("    	  4. ListEmpty    \t10. ListInsert\n");
        printf("    	  5. ListLength   \t11. ListDelete\n");
        printf("    	  6. GetElem      \t12. ListTraverse\n");
        printf("    	  13. ReadFromFile\t14. SaveToFile\n");
        printf("          0. Exit\n");
        printf("-------------------------------------------------\n");
        printf("    请选择你的操作[0~14]:");
        rewind(stdin);
        scanf("%d",&op);
        switch(op){
            case 1:
                //检测是否获取到了线性表的列表
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        //检测表是否存在
                        if (Lists[temp-1].head) printf("线性表%d已存在！", temp);
                        else {
                            //执行操作
                            if (IntiaList(&Lists[temp - 1]) == OK) printf("线性表%d创建成功！", temp);
                            else printf("线性表%d创建失败！", temp);
                        }
                    }
                    free(selectedLists);
                }else{
                    printf("线性表选择错误，请检查。");
                }
                printf("按任意键继续\n");
                rewind(stdin);getchar();
                break;
            case 2:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) printf("请先初始化表%d！", temp);
                        else {
                            if (DestroyList(&Lists[temp-1]) == OK) printf("已删除线性表%d。", temp);
                        }
                    }
                    free(selectedLists);
                }else{
                    printf("线性表选择错误，请检查。");
                }
                printf("按任意键继续\n");
                rewind(stdin);getchar();
                break;
            case 3:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) printf("请先初始化表%d！",temp);
                        else {
                            if (ClearList(&Lists[temp-1]) == OK) printf("已清空线性表%d。",temp);
                        }
                    }
                    free(selectedLists);
                }else{
                    printf("线性表选择错误，请检查。");
                }
                printf("按任意键继续\n");
                rewind(stdin);getchar();
                break;
            case 4:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) printf("请先初始化表%d！", temp);
                        else {
                            result = ListEmpty(Lists[temp-1]);
                            if (result == TRUE)printf("线性表%d为空。\n", temp);
                            else printf("线性表%d不为空。\n", temp);
                        }
                    }
                    free(selectedLists);
                } else{
                    printf("线性表选择错误，请检查。");
                }
                printf("按任意键继续\n");
                rewind(stdin);getchar();
                break;
            case 5:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        printf("线性表%d元素个数为：%d。", temp, ListLength(Lists[temp-1]));
                    }
                    free(selectedLists);
                }else
                    printf("线性表选择错误，请检查。");
                printf("按任意键继续\n");
                rewind(stdin);getchar();
                break;
            case 6:
                c = 'y';
                while (c != 'n') {
                    if ((selectedLists = ChooseList(n)) != ERROR) {
                        for (int j = 0; selectedLists[j]; j++) {
                            int temp = selectedLists[j];
                            if (!Lists[temp-1].head) {
                                printf("请先初始化表%d！按任意键继续\n", temp);
                                c = 'n';
                                rewind(stdin);getchar();
                            } else if (ListEmpty(Lists[temp-1])) {
                                printf("线性表%d为空表！按任意键继续\n", temp);
                                c = 'n';
                                rewind(stdin);getchar();
                            } else {
                                printf("请输入想要获取的元素序号（从1开始）：");
                                rewind(stdin);scanf("%d", &i);
                                if (i-1<0 || i>ListLength(Lists[temp-1]))
                                    printf("输入的序号超出线性表%d范围，请检查。\n", temp);
                                else {
                                    GetElem(&Lists[temp-1], i, &e);
                                    printf("获取的第%d个元素的值为：%d\n", i, e);
                                }
                                rewind(stdin);
                                if(selectedLists[j+1]==0)printf("需要继续获取元素吗？ y/n:");
                                scanf("%c", &c);
                            }
                        }
                        free(selectedLists);
                    }else {
                        printf("线性表选择错误，请检查。");
                        printf("按任意键继续\n");
                        rewind(stdin);
                        getchar();
                    }
                }
                break;
            case 7:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) {
                            printf("请先初始化表%d！", temp);
                        } else if (ListEmpty(Lists[temp-1])) {
                            printf("线性表%d为空表！", temp);
                        } else {
                            printf("\n请输入想要比较的元素值：");
                            result = HandleInput(input, &str_end, &i);
                            if (result == ERROR || result == OVERFLOW) printf("输入有误，请检查。");
                            else {
                                result = LocateElem(Lists[temp-1], i, compare);
                                if (result == ERROR) printf("未找到线性表%d中与给定元素值符合compare()关系的元素，请检查输入的元素值和compare()关系。", temp);
                                else printf("找到线性表%d中第一个与输入元素值符合compare()关系的元素，位序为：%d。", temp, result);
                            }
                        }
                    }
                    free(selectedLists);
                }else
                    printf("线性表选择错误，请检查。");
                printf("按任意键继续\n");
                rewind(stdin);getchar();
                break;
            case 8:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) {
                            printf("请先初始化表%d！", temp);
                        } else if (ListEmpty(Lists[temp-1])) {
                            printf("线性表%d为空表！", temp);
                        } else {
                            printf("请输入想要在表%d中查找前驱的元素值：", temp);
                            result = HandleInput(input, &str_end, &i);
                            if (result == ERROR || result == OVERFLOW) printf("输入有误，请检查。");
                            else {
                                result = PriorElem(Lists[temp-1], i, &e);
                                if (result == ERROR || result == OVERFLOW) printf("未找到线性表%d中给定元素值的前驱，请检查输入的元素值。", temp);
                                else printf("线性表%d中输入元素值的前驱值为：%d。", temp, e);
                            }
                        }
                    }
                    free(selectedLists);
                } else{
                    printf("线性表选择错误，请检查。");
                }
                printf("按任意键继续\n");
                rewind(stdin);getchar();
                break;
            case 9:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) {
                            printf("请先初始化表%d！", temp);
                        } else if (ListEmpty(Lists[temp-1])) {
                            printf("线性表%d为空表！", temp);
                        } else {
                            printf("请输入想要在表%d查找后继的元素值：", temp);
                            result = HandleInput(input, &str_end, &i);
                            if (result == ERROR || result == OVERFLOW) printf("输入有误，请检查。");
                            else {
                                result = NextElem(Lists[temp-1], i, &e);
                                if (result == ERROR || result == OVERFLOW) printf("未找到线性表%d中给定元素值的后继，请检查输入的元素值。", temp);
                                else printf("线性表%d中输入元素值的后继值为：%d。", temp, e);
                            }
                        }
                    }
                    free(selectedLists);
                }else{
                    printf("线性表选择错误，请检查。");
                }
                printf("按任意键继续\n");
                rewind(stdin);getchar();
                break;
            case 10:
                c = 'y';
                while (c != 'n') {
                    if ((selectedLists = ChooseList(n)) != ERROR) {
                        for (int j = 0; selectedLists[j]; j++) {
                            int temp = selectedLists[j];
                            if (!Lists[temp-1].head) {
                                printf("请先初始化表%d！按任意键继续\n", temp);
                                c = 'n';
                            } else {
                                printf("请输入想要插入表%d的元素值：", temp);
                                result = HandleInput(input, &str_end, &i);
                                if (result == ERROR || result == OVERFLOW) printf("输入有误，请检查。\n");
                                else {
                                    e = i;
                                    printf("请输入想要插入表%d的位置（从1开始）：", temp);
                                    scanf("%d", &i);
                                    result = ListInsert(&Lists[temp-1], i, e);
                                    if (result == OVERFLOW) {
                                        printf("无法插入元素，请检查输入的位置是否正确。");
                                        c = 'n';
                                    } else printf("插入完成。");
                                }
                                rewind(stdin);
                                if(selectedLists[j+1]==0)printf("需要继续插入新元素吗？ y/n:");
                                scanf("%c", &c);
                            }
                        }
                        free(selectedLists);
                        getchar();
                    }else {
                        printf("线性表选择错误，请检查。");
                        printf("按任意键继续\n");
                        c='n';
                        rewind(stdin);
                        getchar();
                    }
                }
                break;
            case 11:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head)
                            printf("请先初始化表%d！", temp);
                        else {
                            printf("请输入想要在表%d中删除的位置（从1开始）：", temp);
                            scanf("%d", &i);
                            result = ListDelete(&Lists[temp-1], i, &e);
                            if (result == OVERFLOW) printf("无法在表%d中删除元素，请检查输入的位置是否正确。", temp);
                            else printf("成功在表%d中删除值：%d。", temp, e);
                        }
                    }
                    free(selectedLists);
                }else{
                    printf("线性表选择错误，请检查。");
                }
                printf("按任意键继续\n");
                rewind(stdin);getchar();
                break;
            case 12:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) {
                            printf("请先初始化表%d！", temp);
                        } else {
                            printf("\n遍历开始：\n");
                            printf("-------------------------------------------------\n");
                            if (ListEmpty(Lists[temp-1])) printf("线性表%d是空表！\n",  temp);
                            else(ListTraverse(Lists[temp-1], visit));
                            printf("\n-------------------------------------------------\n");
                            printf("遍历结束。");
                        }
                    }
                    free(selectedLists);
                } else{
                    printf("线性表选择错误，请检查。");
                }
                printf("按任意键继续\n");
                rewind(stdin);getchar();
                break;
            case 13:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) IntiaList(&Lists[temp - 1]);
                        in = fopen(strcat(itoa(temp, filename, 10), ".txt"), "r");
                        if (ReadFromFile(in, &Lists[temp-1]) == ERROR)printf("请检查待读取的%d文件是否存在！",temp);
                        else printf("读取成功！");
                    }
                    free(selectedLists);
                } else{
                    printf("线性表选择有误，请检查。");
                }
                printf("按任意键继续\n");
                rewind(stdin);getchar();
                break;
            case 14:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) {
                            printf("请先初始化表%d！", temp);
                        } else if (Lists[temp-1].length == 0) {
                            printf("线性表%d是空表。确认要继续保存吗？ y/n", temp);
                            rewind(stdin);
                            scanf("%c", &c);
                            if (c != 'y') {
                                printf("\n取消保存表%d。按任意键继续\n", temp);
                                rewind(stdin);
                                getchar();
                                break;
                            }else {
                                in = fopen(strcat(itoa(temp, filename, 10), ".txt"), "w");
                                if (SaveToFile(in, &Lists[temp - 1]) == ERROR) printf("保存表%d出错！", temp);
                                else printf("保存表%d成功！", temp);
                            }
                        } else {
                            in = fopen(strcat(itoa(temp, filename, 10), ".txt"), "w");
                            if (SaveToFile(in, &Lists[temp-1]) == ERROR) printf("保存表%d出错！", temp);
                            else printf("保存表%d成功！", temp);
                        }
                    }
                    free(selectedLists);
                } else{
                    printf("线性表选择有误，请检查。");
                }
                printf("按任意键继续\n");
                rewind(stdin);getchar();
                break;
            case 0:
                fclose(in);
                break;
        }//end of switch
    }//end of while
    fclose(in);
    printf("欢迎下次再使用本系统！\n");
}//end of main()
/*--------page 23 on textbook --------------------*/
status IntiaList(LinkList *L){
    L->head = (Node *)malloc(sizeof (Node));
    if(!L->head) return OVERFLOW;
    L->head->next=NULL;
    L->length=0;
    return OK;
}
/**
 * 销毁表（即释放掉L的所有结点，包括头结点），并重置表的头结点为NULL，length为0
 * @param L
 * @return
 */
status DestroyList(LinkList *L){
    ClearList(L);
    free(L->head);
    L->head=NULL;
    return OK;
}
/**
 * 清空表，释放表头结点后的所有结点，并重置表的length为0
 * @param L
 * @return
 */
status ClearList(LinkList *L){
    Node *p,*q;
    for (p=L->head->next;p;){
        q=p->next;
        free(p);
        p=q;
    }
    L->head->next=NULL;
    L->length=0;
    return OK;
}
/**
 * 仅判断传入的线性表L的数据域是否无元素
 * @param L 传入的数据表
 * @return TRUE: L的数据域无元素；FALSE: L的数据域有元素
 */
status ListEmpty(LinkList L){
    if(ListLength(L)==0) return TRUE;
    else return FALSE;
}
int ListLength(LinkList L){
    return L.length;
}
/**
 * 返回给定线性表L中 <strong>第i个</strong> 元素的值
 * @param L
 * @param i
 * @param e
 * @return OK；*e：取出的元素值
 */
status GetElem(LinkList *L,int i,ElemType *e){
    Node *p=L->head;
    for(;i>0;i--)p=p->next;
    *e=p->elem;
    return OK;
}
/**
 * 返回线性表L中第1个与给定元素e满足关系compare()的数据元素的位序
 * @param L
 * @param e
 * @param compare 判定线性表L中元素与给定元素e的关系的函数，返回上面提到的位序或ERROR
 * @return order: 线性表L中第1个与给定元素e满足关系compare()的数据元素的位序；
 * ERROR: 线性表L中不存在与给定元素e满足关系compare()的数据元素，或未指定compare()函数
 */
status LocateElem(LinkList L,ElemType e, status(*compare)(ElemType ,ElemType)) {
    if (!compare) return ERROR;
    Node *p=L.head->next;
    for (int i=1;p;p=p->next,i++) {
        if (compare(p->elem, e))return i;
    }
    return ERROR;
}
/**
 *
 * @param L
 * @param cur
 * @param pre_e
 * @return ERROR: 不存在符合条件的前驱； OK: 正常
 */
status PriorElem(LinkList L,ElemType cur,ElemType *pre_e){
    if(ListLength(L)<=1) return ERROR;
    else {
        Node *p=L.head->next->next, *q=L.head->next;
        if(q->elem==cur) return ERROR;//上一行直接从第二个开始遍历是否过早优化？
        for(;p;q=p,p=p->next){
            if(p->elem==cur){
                *pre_e=q->elem;
                return OK;
            }
        }
        return ERROR;
    }
}
status NextElem(LinkList L,ElemType cur,ElemType *next_e){
    if(ListLength(L)<=1) return ERROR;
    else {
        Node *p=L.head->next;
        for(;p->next;p=p->next){
            if(p->elem==cur){
                *next_e=p->next->elem;
                return OK;
            }
        }
        return ERROR;
    }
}
status ListInsert(LinkList *L,int i,ElemType e){
    //插入可以插在第一个至最后一个的后一个之间，即1-length+1之间
    if(i<1 || i>ListLength(*L)+1) return OVERFLOW;
    else {
        Node *p=(Node *)malloc(sizeof(Node));
        if(!p)return OVERFLOW;
        p->elem=e;
        Node *q=L->head;
        for(;i>1;i--)q=q->next;
        p->next=q->next;
        q->next=p;
        L->length++;//更新length
        return OK;
    }
}
/**
 *
 * @param L
 * @param i
 * @param e
 * @return OVERFLOW: 给定的位次越界； OK: 成功
 */
status ListDelete(LinkList *L,int i,ElemType *e) {
    if (i < 1 || i > ListLength(*L)) return OVERFLOW;
    else {
        Node *p,*q=L->head;
        //找到该位置的前一个节点
        for(;i>1;i--)q=q->next;
        p=q->next;
        *e=p->elem;
        q->next=q->next->next;
        free(p);
        L->length--;
        return OK;
    }
}

status ListTraverse(LinkList L, status(*visit)(ElemType)){
    for(Node *p=L.head->next;p;p=p->next)
        visit(p->elem);
    return OK;
}

status ReadFromFile(FILE *p, LinkList *L){
    if (p==NULL) return ERROR;
    else {
        int i=1;
        char temp[11];
        ClearList(L);
        while(fscanf(p, "%s", temp)!=EOF){
            ListInsert(L, i++, atoi(temp));
        }
        return OK;
    }
}
status SaveToFile(FILE *p, LinkList *L){
    if (p==NULL) return ERROR;
    else {
        Node *q=L->head->next;
        while(q){
            if(fprintf(p, "%d ", q->elem)<=0) return ERROR;
            q=q->next;
        }
        fflush(p);
        return OK;
    }
}
status HandleInput(char *input, char **str_end, ElemType *e){
    rewind(stdin);
    if(scanf("%s", input)!=1) return ERROR;
    if ((*e=strtol(input, str_end, 10))!=0){
        if (errno==ERANGE) {
            errno=0;
            return OVERFLOW;
        }else {
            if(**str_end!=0) {
                rewind(stdin);
                return ERROR;//若指向的字符不为终止符则说明未转换完全
            }
            return OK;
        }
    }
    rewind(stdin);
    return ERROR;
}
/**
 * 获取用户的输入，判断是否越界，并返回指向一块存储用户输入结果的区域的指针且用0表示输入结束，或返回ERROR。
 * @param n
 * @return
 */
int *ChooseList(int n) {
    int *p=NULL,*q, temp, result, i=1;
    //对n==1时的优化，免去用户输入1和EOF的麻烦
    if (n==1){
        if(!(q = realloc(p, 2*sizeof(int)))) return ERROR;
        else {
            p=q;
            p[0]=1;p[1]=0;
            getchar();//消耗掉EOF，即ascii==26
            return p;
        }
    }
    printf("请输入要进行操作的表号(1-%d)，用空格隔开，EOF结束:", n);
    while ((result=scanf("%d", &temp))!=0 && result!=EOF){
        if(!(q = realloc(p, i*sizeof(int)))) return ERROR;
        else {
            p=q;
            if(result>0){
                if(temp>0 && temp<=n)p[i-1]=temp;
                else return ERROR;
            }
            else p[i-1]=0;
            i++;
        }
    }
    if(!(q = realloc(p, i*sizeof(int)))) return ERROR;
    else {
        p=q;
        p[i-1]=0;
    }
    getchar();//消耗掉EOF，即ascii==26
    if(p[0]==0) return ERROR;
    return p;
}