#include <stdio.h>
#include <stdlib.h>
/*---------page 10 on textbook ---------*/
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASTABLE -1
#define OVERFLOW -2
#define DATA_LEN 20

typedef int status;
typedef struct{
    int key;
    char others[DATA_LEN+1];
}Elem;// 数据元素类型定义
typedef struct Arc{
    int adjVex;// 存储本弧弧头的顶点栈索引
    int weight;// 存储本弧的权值
    struct Arc *next;// 指向同弧尾的下一条弧的弧结点
}ArcNode;// 弧结点定义
typedef struct{
    Elem data;// 顶点的数据
    ArcNode *firstArc; // 指向以本顶点为弧尾的第一条弧的弧结点
}Vex;// 顶点定义

int main() {
    int op=1;
    while(op){
        system("cls");	printf("\n\n");
        printf("      Menu for Linear Table On Sequence Structure \n");
        printf("-------------------------------------------------\n");
        printf("    	  1. IntiaList       7. LocateElem\n");
        printf("    	  2. DestroyList     8. PriorElem\n");
        printf("    	  3. ClearList       9. NextElem \n");
        printf("    	  4. ListEmpty     10. ListInsert\n");
        printf("    	  5. ListLength     11. ListDelete\n");
        printf("    	  6. GetElem       12. ListTrabverse\n");
        printf("    	  0. Exit\n");
        printf("-------------------------------------------------\n");
        printf("    请选择你的操作[0~12]:");
        scanf("%d",&op);
        switch(op){
            case 1:
                //printf("\n----IntiaList功能待实现！\n");
                if(OK) printf("线性表创建成功！\n");
                else printf("线性表创建失败！\n");
                getchar();getchar();
                break;
            case 2:
                printf("\n----DestroyList功能待实现！\n");
                getchar();getchar();
                break;
            case 3:
                printf("\n----ClearList功能待实现！\n");
                getchar();getchar();
                break;
            case 4:
                printf("\n----ListEmpty功能待实现！\n");
                getchar();getchar();
                break;
            case 5:
                printf("\n----ListLength功能待实现！\n");
                getchar();getchar();
                break;
            case 6:
                printf("\n----GetElem功能待实现！\n");
                getchar();getchar();
                break;
            case 7:
                printf("\n----LocateElem功能待实现！\n");
                getchar();getchar();
                break;
            case 8:
                printf("\n----PriorElem功能待实现！\n");
                getchar();getchar();
                break;
            case 9:
                printf("\n----NextElem功能待实现！\n");
                getchar();getchar();
                break;
            case 10:
                printf("\n----ListInsert功能待实现！\n");
                getchar();getchar();
                break;
            case 11:
                printf("\n----ListDelete功能待实现！\n");
                getchar();getchar();
                break;
            case 12:
                //printf("\n----ListTrabverse功能待实现！\n");
                getchar();getchar();
                break;
            case 0:
                break;
        }//end of switch
    }//end of while    return 0;
}