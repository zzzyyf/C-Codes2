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
typedef int ElemType; //����Ԫ�����Ͷ���

/*-------page 22 on textbook -------*/
typedef struct LNode{  //˳�����ʽ�ṹ���ڵ�Ķ���
    ElemType elem;
    struct LNode * next;
}Node;
typedef struct { //˳�����ʽ�ṹ����ͷ����, headΪNULL��length==0ʱΪ�ձ�
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
 * ��ȡ�û������룬�ж��Ƿ�Խ�磬������ָ��һ��洢�û��������������ָ������0��ʾ����������򷵻�ERROR��
 * @param n
 * @return
 */
int *ChooseList(int n);
/*--------------------------------------------*/
void main(void){
    int op=1;//ѡ�������
    int i=0,result, n=-1;//result���ڴ洢�������ص�status��n����ָ�������ı���Ŀ
    int * selectedLists=NULL; //�洢chooselist���صĶ�̬��ָ������
    ElemType e;
    char c, input[11], *str_end=&input[0];//��֧������10λ������9λ����
    char filename[10];
    FILE *in=NULL;
    status (*compare)(ElemType, ElemType)=Compare;
    status (*visit)(ElemType)=Visit;

    //ָ����Ҫ����ı���Ŀ
    while(n<0) {
        printf("��������Ҫ����ı����Ŀ��1-%d������0�˳�����", MAX_LIST);
        if(scanf("%d", &n)!=1||n<0||n>MAX_LIST) {
            printf("����������飡\n");
            n=-1;
        }
        else if(n==0) exit(0);
        rewind(stdin);
    }
    //��ʼ����
    LinkList Lists[n];
    for (i=0;i<n;i++){
        Lists[i].head=NULL;Lists[i].length=0;
    }

    //����ϵͳ
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
        printf("    ��ѡ����Ĳ���[0~14]:");
        rewind(stdin);
        scanf("%d",&op);
        switch(op){
            case 1:
                //����Ƿ��ȡ�������Ա���б�
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        //�����Ƿ����
                        if (Lists[temp-1].head) printf("���Ա�%d�Ѵ��ڣ�", temp);
                        else {
                            //ִ�в���
                            if (IntiaList(&Lists[temp - 1]) == OK) printf("���Ա�%d�����ɹ���", temp);
                            else printf("���Ա�%d����ʧ�ܣ�", temp);
                        }
                    }
                    free(selectedLists);
                }else{
                    printf("���Ա�ѡ��������顣");
                }
                printf("�����������\n");
                rewind(stdin);getchar();
                break;
            case 2:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) printf("���ȳ�ʼ����%d��", temp);
                        else {
                            if (DestroyList(&Lists[temp-1]) == OK) printf("��ɾ�����Ա�%d��", temp);
                        }
                    }
                    free(selectedLists);
                }else{
                    printf("���Ա�ѡ��������顣");
                }
                printf("�����������\n");
                rewind(stdin);getchar();
                break;
            case 3:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) printf("���ȳ�ʼ����%d��",temp);
                        else {
                            if (ClearList(&Lists[temp-1]) == OK) printf("��������Ա�%d��",temp);
                        }
                    }
                    free(selectedLists);
                }else{
                    printf("���Ա�ѡ��������顣");
                }
                printf("�����������\n");
                rewind(stdin);getchar();
                break;
            case 4:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) printf("���ȳ�ʼ����%d��", temp);
                        else {
                            result = ListEmpty(Lists[temp-1]);
                            if (result == TRUE)printf("���Ա�%dΪ�ա�\n", temp);
                            else printf("���Ա�%d��Ϊ�ա�\n", temp);
                        }
                    }
                    free(selectedLists);
                } else{
                    printf("���Ա�ѡ��������顣");
                }
                printf("�����������\n");
                rewind(stdin);getchar();
                break;
            case 5:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        printf("���Ա�%dԪ�ظ���Ϊ��%d��", temp, ListLength(Lists[temp-1]));
                    }
                    free(selectedLists);
                }else
                    printf("���Ա�ѡ��������顣");
                printf("�����������\n");
                rewind(stdin);getchar();
                break;
            case 6:
                c = 'y';
                while (c != 'n') {
                    if ((selectedLists = ChooseList(n)) != ERROR) {
                        for (int j = 0; selectedLists[j]; j++) {
                            int temp = selectedLists[j];
                            if (!Lists[temp-1].head) {
                                printf("���ȳ�ʼ����%d�������������\n", temp);
                                c = 'n';
                                rewind(stdin);getchar();
                            } else if (ListEmpty(Lists[temp-1])) {
                                printf("���Ա�%dΪ�ձ������������\n", temp);
                                c = 'n';
                                rewind(stdin);getchar();
                            } else {
                                printf("��������Ҫ��ȡ��Ԫ����ţ���1��ʼ����");
                                rewind(stdin);scanf("%d", &i);
                                if (i-1<0 || i>ListLength(Lists[temp-1]))
                                    printf("�������ų������Ա�%d��Χ�����顣\n", temp);
                                else {
                                    GetElem(&Lists[temp-1], i, &e);
                                    printf("��ȡ�ĵ�%d��Ԫ�ص�ֵΪ��%d\n", i, e);
                                }
                                rewind(stdin);
                                if(selectedLists[j+1]==0)printf("��Ҫ������ȡԪ���� y/n:");
                                scanf("%c", &c);
                            }
                        }
                        free(selectedLists);
                    }else {
                        printf("���Ա�ѡ��������顣");
                        printf("�����������\n");
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
                            printf("���ȳ�ʼ����%d��", temp);
                        } else if (ListEmpty(Lists[temp-1])) {
                            printf("���Ա�%dΪ�ձ�", temp);
                        } else {
                            printf("\n��������Ҫ�Ƚϵ�Ԫ��ֵ��");
                            result = HandleInput(input, &str_end, &i);
                            if (result == ERROR || result == OVERFLOW) printf("�����������顣");
                            else {
                                result = LocateElem(Lists[temp-1], i, compare);
                                if (result == ERROR) printf("δ�ҵ����Ա�%d�������Ԫ��ֵ����compare()��ϵ��Ԫ�أ����������Ԫ��ֵ��compare()��ϵ��", temp);
                                else printf("�ҵ����Ա�%d�е�һ��������Ԫ��ֵ����compare()��ϵ��Ԫ�أ�λ��Ϊ��%d��", temp, result);
                            }
                        }
                    }
                    free(selectedLists);
                }else
                    printf("���Ա�ѡ��������顣");
                printf("�����������\n");
                rewind(stdin);getchar();
                break;
            case 8:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) {
                            printf("���ȳ�ʼ����%d��", temp);
                        } else if (ListEmpty(Lists[temp-1])) {
                            printf("���Ա�%dΪ�ձ�", temp);
                        } else {
                            printf("��������Ҫ�ڱ�%d�в���ǰ����Ԫ��ֵ��", temp);
                            result = HandleInput(input, &str_end, &i);
                            if (result == ERROR || result == OVERFLOW) printf("�����������顣");
                            else {
                                result = PriorElem(Lists[temp-1], i, &e);
                                if (result == ERROR || result == OVERFLOW) printf("δ�ҵ����Ա�%d�и���Ԫ��ֵ��ǰ�������������Ԫ��ֵ��", temp);
                                else printf("���Ա�%d������Ԫ��ֵ��ǰ��ֵΪ��%d��", temp, e);
                            }
                        }
                    }
                    free(selectedLists);
                } else{
                    printf("���Ա�ѡ��������顣");
                }
                printf("�����������\n");
                rewind(stdin);getchar();
                break;
            case 9:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) {
                            printf("���ȳ�ʼ����%d��", temp);
                        } else if (ListEmpty(Lists[temp-1])) {
                            printf("���Ա�%dΪ�ձ�", temp);
                        } else {
                            printf("��������Ҫ�ڱ�%d���Һ�̵�Ԫ��ֵ��", temp);
                            result = HandleInput(input, &str_end, &i);
                            if (result == ERROR || result == OVERFLOW) printf("�����������顣");
                            else {
                                result = NextElem(Lists[temp-1], i, &e);
                                if (result == ERROR || result == OVERFLOW) printf("δ�ҵ����Ա�%d�и���Ԫ��ֵ�ĺ�̣����������Ԫ��ֵ��", temp);
                                else printf("���Ա�%d������Ԫ��ֵ�ĺ��ֵΪ��%d��", temp, e);
                            }
                        }
                    }
                    free(selectedLists);
                }else{
                    printf("���Ա�ѡ��������顣");
                }
                printf("�����������\n");
                rewind(stdin);getchar();
                break;
            case 10:
                c = 'y';
                while (c != 'n') {
                    if ((selectedLists = ChooseList(n)) != ERROR) {
                        for (int j = 0; selectedLists[j]; j++) {
                            int temp = selectedLists[j];
                            if (!Lists[temp-1].head) {
                                printf("���ȳ�ʼ����%d�������������\n", temp);
                                c = 'n';
                            } else {
                                printf("��������Ҫ�����%d��Ԫ��ֵ��", temp);
                                result = HandleInput(input, &str_end, &i);
                                if (result == ERROR || result == OVERFLOW) printf("�����������顣\n");
                                else {
                                    e = i;
                                    printf("��������Ҫ�����%d��λ�ã���1��ʼ����", temp);
                                    scanf("%d", &i);
                                    result = ListInsert(&Lists[temp-1], i, e);
                                    if (result == OVERFLOW) {
                                        printf("�޷�����Ԫ�أ����������λ���Ƿ���ȷ��");
                                        c = 'n';
                                    } else printf("������ɡ�");
                                }
                                rewind(stdin);
                                if(selectedLists[j+1]==0)printf("��Ҫ����������Ԫ���� y/n:");
                                scanf("%c", &c);
                            }
                        }
                        free(selectedLists);
                        getchar();
                    }else {
                        printf("���Ա�ѡ��������顣");
                        printf("�����������\n");
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
                            printf("���ȳ�ʼ����%d��", temp);
                        else {
                            printf("��������Ҫ�ڱ�%d��ɾ����λ�ã���1��ʼ����", temp);
                            scanf("%d", &i);
                            result = ListDelete(&Lists[temp-1], i, &e);
                            if (result == OVERFLOW) printf("�޷��ڱ�%d��ɾ��Ԫ�أ����������λ���Ƿ���ȷ��", temp);
                            else printf("�ɹ��ڱ�%d��ɾ��ֵ��%d��", temp, e);
                        }
                    }
                    free(selectedLists);
                }else{
                    printf("���Ա�ѡ��������顣");
                }
                printf("�����������\n");
                rewind(stdin);getchar();
                break;
            case 12:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) {
                            printf("���ȳ�ʼ����%d��", temp);
                        } else {
                            printf("\n������ʼ��\n");
                            printf("-------------------------------------------------\n");
                            if (ListEmpty(Lists[temp-1])) printf("���Ա�%d�ǿձ�\n",  temp);
                            else(ListTraverse(Lists[temp-1], visit));
                            printf("\n-------------------------------------------------\n");
                            printf("����������");
                        }
                    }
                    free(selectedLists);
                } else{
                    printf("���Ա�ѡ��������顣");
                }
                printf("�����������\n");
                rewind(stdin);getchar();
                break;
            case 13:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) IntiaList(&Lists[temp - 1]);
                        in = fopen(strcat(itoa(temp, filename, 10), ".txt"), "r");
                        if (ReadFromFile(in, &Lists[temp-1]) == ERROR)printf("�������ȡ��%d�ļ��Ƿ���ڣ�",temp);
                        else printf("��ȡ�ɹ���");
                    }
                    free(selectedLists);
                } else{
                    printf("���Ա�ѡ���������顣");
                }
                printf("�����������\n");
                rewind(stdin);getchar();
                break;
            case 14:
                if ((selectedLists = ChooseList(n)) != ERROR) {
                    for (int j = 0; selectedLists[j]; j++) {
                        int temp = selectedLists[j];
                        if (!Lists[temp-1].head) {
                            printf("���ȳ�ʼ����%d��", temp);
                        } else if (Lists[temp-1].length == 0) {
                            printf("���Ա�%d�ǿձ�ȷ��Ҫ���������� y/n", temp);
                            rewind(stdin);
                            scanf("%c", &c);
                            if (c != 'y') {
                                printf("\nȡ�������%d�������������\n", temp);
                                rewind(stdin);
                                getchar();
                                break;
                            }else {
                                in = fopen(strcat(itoa(temp, filename, 10), ".txt"), "w");
                                if (SaveToFile(in, &Lists[temp - 1]) == ERROR) printf("�����%d����", temp);
                                else printf("�����%d�ɹ���", temp);
                            }
                        } else {
                            in = fopen(strcat(itoa(temp, filename, 10), ".txt"), "w");
                            if (SaveToFile(in, &Lists[temp-1]) == ERROR) printf("�����%d����", temp);
                            else printf("�����%d�ɹ���", temp);
                        }
                    }
                    free(selectedLists);
                } else{
                    printf("���Ա�ѡ���������顣");
                }
                printf("�����������\n");
                rewind(stdin);getchar();
                break;
            case 0:
                fclose(in);
                break;
        }//end of switch
    }//end of while
    fclose(in);
    printf("��ӭ�´���ʹ�ñ�ϵͳ��\n");
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
 * ���ٱ����ͷŵ�L�����н�㣬����ͷ��㣩�������ñ��ͷ���ΪNULL��lengthΪ0
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
 * ��ձ��ͷű�ͷ��������н�㣬�����ñ��lengthΪ0
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
 * ���жϴ�������Ա�L���������Ƿ���Ԫ��
 * @param L ��������ݱ�
 * @return TRUE: L����������Ԫ�أ�FALSE: L����������Ԫ��
 */
status ListEmpty(LinkList L){
    if(ListLength(L)==0) return TRUE;
    else return FALSE;
}
int ListLength(LinkList L){
    return L.length;
}
/**
 * ���ظ������Ա�L�� <strong>��i��</strong> Ԫ�ص�ֵ
 * @param L
 * @param i
 * @param e
 * @return OK��*e��ȡ����Ԫ��ֵ
 */
status GetElem(LinkList *L,int i,ElemType *e){
    Node *p=L->head;
    for(;i>0;i--)p=p->next;
    *e=p->elem;
    return OK;
}
/**
 * �������Ա�L�е�1�������Ԫ��e�����ϵcompare()������Ԫ�ص�λ��
 * @param L
 * @param e
 * @param compare �ж����Ա�L��Ԫ�������Ԫ��e�Ĺ�ϵ�ĺ��������������ᵽ��λ���ERROR
 * @return order: ���Ա�L�е�1�������Ԫ��e�����ϵcompare()������Ԫ�ص�λ��
 * ERROR: ���Ա�L�в����������Ԫ��e�����ϵcompare()������Ԫ�أ���δָ��compare()����
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
 * @return ERROR: �����ڷ���������ǰ���� OK: ����
 */
status PriorElem(LinkList L,ElemType cur,ElemType *pre_e){
    if(ListLength(L)<=1) return ERROR;
    else {
        Node *p=L.head->next->next, *q=L.head->next;
        if(q->elem==cur) return ERROR;//��һ��ֱ�Ӵӵڶ�����ʼ�����Ƿ�����Ż���
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
    //������Բ��ڵ�һ�������һ���ĺ�һ��֮�䣬��1-length+1֮��
    if(i<1 || i>ListLength(*L)+1) return OVERFLOW;
    else {
        Node *p=(Node *)malloc(sizeof(Node));
        if(!p)return OVERFLOW;
        p->elem=e;
        Node *q=L->head;
        for(;i>1;i--)q=q->next;
        p->next=q->next;
        q->next=p;
        L->length++;//����length
        return OK;
    }
}
/**
 *
 * @param L
 * @param i
 * @param e
 * @return OVERFLOW: ������λ��Խ�磻 OK: �ɹ�
 */
status ListDelete(LinkList *L,int i,ElemType *e) {
    if (i < 1 || i > ListLength(*L)) return OVERFLOW;
    else {
        Node *p,*q=L->head;
        //�ҵ���λ�õ�ǰһ���ڵ�
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
                return ERROR;//��ָ����ַ���Ϊ��ֹ����˵��δת����ȫ
            }
            return OK;
        }
    }
    rewind(stdin);
    return ERROR;
}
/**
 * ��ȡ�û������룬�ж��Ƿ�Խ�磬������ָ��һ��洢�û��������������ָ������0��ʾ����������򷵻�ERROR��
 * @param n
 * @return
 */
int *ChooseList(int n) {
    int *p=NULL,*q, temp, result, i=1;
    //��n==1ʱ���Ż�����ȥ�û�����1��EOF���鷳
    if (n==1){
        if(!(q = realloc(p, 2*sizeof(int)))) return ERROR;
        else {
            p=q;
            p[0]=1;p[1]=0;
            getchar();//���ĵ�EOF����ascii==26
            return p;
        }
    }
    printf("������Ҫ���в����ı��(1-%d)���ÿո������EOF����:", n);
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
    getchar();//���ĵ�EOF����ascii==26
    if(p[0]==0) return ERROR;
    return p;
}