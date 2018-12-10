#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR -3
#define OVERFLOW -2
#define DATA_LEN 20 //元素的others字段所用长度
#define MAX_TREES 5 //同时管理的树的最大数目
#define NAME_LEN 20 //树名的最大长度
typedef int status;
typedef struct Elem{
    int key; //key==0时判定所在的结点为空
    //TODO: key的正常值不能为0
    //TODO: 注意key是唯一主值，在插入子树时应确认
    char others[DATA_LEN+1];
}elem;
typedef struct Node{
    elem data;
    struct Node *lChild, *rChild;
}BiTree, node;
typedef struct{
    BiTree *root;//为NULL时树不存在
    char name[NAME_LEN+1];
}Tree;
status InitBiTree(Tree *T);
status DestroyBiTree(Tree *T);
status CreateBiTree(Tree *T, node *nodes, int n);
status ClearBiTree(BiTree *T);
status BiTreeEmpty(Tree T);
int BiTreeDepth(BiTree T);
BiTree *Root(Tree T);
BiTree *Value(BiTree T, int key);
status Assign(BiTree T, node *e, elem value);
BiTree *Parent(BiTree T, node e);
BiTree *LeftChild(BiTree T, node e);
BiTree *RightChild(BiTree T, node e);
BiTree *LeftSibling(BiTree T, node e);
BiTree *RightSibling(BiTree T, node e);
status InsertChild(BiTree *T, node *p, int LR, BiTree child);
status DeleteChild(BiTree *T, node *p, int LR);
status PreOrderTraverse(BiTree T, node(*Visit)(BiTree, int), node* value);
status InOrderTraverse(BiTree T, node(*Visit)(BiTree, int), node* value);
status PostOrderTraverse(BiTree *T, node(*Visit)(BiTree, int), node* value);
status LevelOrderTraverse(BiTree T, node(*Visit)(BiTree, int), node* value);
BiTree LinearlyPrint(BiTree T, int useless){
    printf("%d | %s\n", T.data.key, T.data.others);
}//直接输出元素
BiTree LevelPrint(BiTree T, int useless){
    int depth=BiTreeDepth(T);
    for (;depth>0;depth--)
        printf(" ");
    LinearlyPrint(T, useless);
} //树形输出元素，树形遍历时用
BiTree DeleteNode(BiTree T, int useless){
    free(&T);
}//删除结点，用于Clear操作
node *FindNode(BiTree T, int key);

int main() {
    int op=1;//选择操作用
    int i=0,result, n=1;//result用于存储函数返回的status，n用于指定操作的表数目
    int *selectedLists=NULL; //存储chooselist返回的动态表指针数组
    //进入系统
    while(op) {
        system("cls");
        printf("\n\n");
        printf("      Menu for Binary Tree On Linked Structure \n");
        printf("-------------------------------------------------\n");
        printf("    	  1. InitBiTree      \t11. LeftChild\n");
        printf("    	  2. DestroyBiTree   \t12. RightChild\n");
        printf("    	  3. CreateBiTree    \t13. LeftSibling\n");
        printf("    	  4. ClearBiTree     \t14. RightSibling\n");
        printf("    	  5. BiTreeEmpty     \t15. InsertChild\n");
        printf("    	  6. BiTreeDepth     \t16. DeleteChild\n");
        printf("    	  7. Root            \t17. PreOrderTraverse\n");
        printf("    	  8. Value           \t18. InOrderTraverse\n");
        printf("    	  9. Assign          \t19. PostOrderTraverse\n");
        printf("    	  10.Parent          \t20. LevelOrderTraverse\n");
        printf("          0. Exit\n");
        printf("-------------------------------------------------\n");
        printf("    请选择你的操作[0~20]:");
        rewind(stdin);
        scanf("%d", &op);
        switch (op) {
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
            case 10:
            case 11:
            case 12:
            default:
                break;
        }
    }
    return 0;
}

status InitBiTree(Tree *T){
    printf("请输入新建的树名：");
    scanf("%20s", T->name);//TODO: 此处的20为NAME_LEN，但似乎不能直接用宏代替
    T->root =(BiTree*)malloc(sizeof(BiTree));
    if (!T->root) return OVERFLOW;//分配失败则返回OVERFLOW
    T->root->data.key=-1;//改为非0表示存在，-1表示未初始化数据
    T->root->lChild=NULL;T->root->rChild=NULL;//初始化指针域
    return OK;
}
status DestroyBiTree(Tree *T){
    ClearBiTree(T->root);
    free(T->root);//TODO: root是否在上一步已经被free了？
    T->root=NULL;
}
/**
 * 采取先序遍历带空结点的结点序列的方式迭代创建树。
 * @param T 树指针
 * @param nodes 先序遍历的节点序列
 * @param n
 * @return
 */
status CreateBiTree(Tree *T, node *nodes, int n){
    if (!T->root)InitBiTree(T);
    if (!nodes[0].data.key) return OK;//若序列的首个结点为空则树为空
    node *stack[n];
    int i=0,j=0;//i遍历结点序列，j遍历栈
    for (i=0;i<n;i++){
        if(nodes[i].data.key) //若第i个结点不为空
            stack[j++] = &nodes[i];//第i个结点入栈，为空则不入栈
        if(j>0) {
            //若上个结点没有左孩子，则新入栈的结点为上个结点的左孩子
            if(!stack[j-1]->lChild)stack[j - 1]->lChild = &nodes[i];
            //若上个结点有左孩子但没有右孩子，则新入栈的结点为上个结点的右孩子
            else if(!stack[j-1]->rChild)stack[j-1]->rChild=&nodes[i];
            //若左右孩子已满则可退栈
            if(stack[j-1]->lChild && stack[j-1]->rChild)j--;
        }
    }
}
status ClearBiTree(BiTree *T){
    PostOrderTraverse(T, DeleteNode, NULL);
}
status BiTreeEmpty(Tree T){
    if(!T.root->data.key)return TRUE;
    else return FALSE;
}
int BiTreeDepth(BiTree T){

}
BiTree *Root(Tree T){
    return T.root;
}
BiTree *Value(BiTree T, int key){}
status Assign(BiTree T, node *e, elem value){}
BiTree *Parent(BiTree T, node e){}
BiTree *LeftChild(BiTree T, node e){}
BiTree *RightChild(BiTree T, node e){}
BiTree *LeftSibling(BiTree T, node e){}
BiTree *RightSibling(BiTree T, node e){}
status InsertChild(BiTree *T, node *p, int LR, BiTree child){}
status DeleteChild(BiTree *T, node *p, int LR){}
status PreOrderTraverse(BiTree T, node(*Visit)(BiTree, int), node* value){}
status InOrderTraverse(BiTree T, node(*Visit)(BiTree, int), node* value){}
status PostOrderTraverse(BiTree *T, node(*Visit)(BiTree, int), node* value){}
status LevelOrderTraverse(BiTree T, node(*Visit)(BiTree, int), node* value){}
node *FindNode(BiTree T, int key){}