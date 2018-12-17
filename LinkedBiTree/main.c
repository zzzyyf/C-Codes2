#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    //TODO: key的正常值不能为0，可以利用存储key的链表以0结尾来检查
    //TODO: 注意key是唯一主值，在新增结点时应确认
    char others[DATA_LEN+1];
}elem;
typedef struct Node{
    elem data;
    struct Node *lChild, *rChild;
}BiTree, Node;
typedef struct{
    BiTree *root;//为NULL时树不存在
    char name[NAME_LEN+1];
    int length;//树的结点数
}Tree;
status InitBiTree(Tree *T);
status DestroyBiTree(Tree *T);
status CreateBiTree(Tree *T, Node **nodes, int n);
status ClearBiTree(BiTree *T, int *keys);
status BiTreeEmpty(Tree T);
int BiTreeDepth(BiTree T, int *level, int n);
BiTree *Root(Tree T);
BiTree *Value(BiTree T, int key);
status Assign(BiTree T, Node *n, elem value, int **keys);
BiTree *Parent(BiTree *T, int key);
BiTree *LeftChild(BiTree *T, int key);
BiTree *RightChild(BiTree *T, int key);
BiTree *LeftSibling(BiTree *T, int key);
BiTree *RightSibling(BiTree *T, int key);
status InsertChild(BiTree *T, Node *p, int LR, BiTree *child);
status DeleteChild(BiTree *T, Node *p, int LR, int *keys);
status PreOrderTraverse(BiTree *T, int (*Visit)(BiTree *, int), int level);//level用于树形打印时计算层数
Node * InOrderTraverse(BiTree T, Node *(*Visit)(BiTree *, int), Node e, Node **value);
status PostOrderTraverse(BiTree *T, Node(*Visit)(BiTree *, int *), int *keys);//因为后序遍历用于清空、删除，故需传指针
status LevelOrderTraverse(BiTree *T, Node*(*Visit)(BiTree *, int), Node *value);

/*-- 以下为自定义辅助操作 --*/
BiTree *LinearlyPrint(BiTree *T, int useless){
    printf("%d | %s", T->data.key, T->data.others);
    return 0;
}//输出元素的主值和数据
BiTree *DirectlyPrint(BiTree *T, int useless){
    printf("%s ", T->data.others);
    return 0;
}//直接输出元素的数据
int *LevelPrint(BiTree *T, int level){
    for (;level>0;level--)
        printf(" ");
    LinearlyPrint(T, level);
    printf("\n");
    return 0;
} //树形输出元素，树形遍历时用
BiTree DeleteNode(BiTree *T, int *keys){
    Node n=*T;
    free(T);
    int i=0;
    for(;keys[i]!=n.data.key;i++);//TODO: 可能会死循环
    keys[i]=0;
    return n;
}//删除结点，用于Clear操作
Node *CprNodeByKey(BiTree *T, int key){
    if (T->data.key==key) return T;
    else return NULL;
}//通过比较key来比较两个结点
status FindNodeByData(BiTree *T, Node *n){
}
status CheckKey(int key, int **keys){
    int i;
    for(i=0;(*keys)[i];i++){
        if(key==(*keys)[i])return ERROR;
    }
    (*keys)[i]=key;//最后一个key替换为当前key
    *keys=realloc(*keys, sizeof(int)*(i+2));//给keys增加一个空间
    (*keys)[i+1]=0;//以0结尾
    return OK;
}//若给定key已存在则返回ERROR；不存在则可以新建结点
/**
 * 接收输入，把输入的结点的指针存入nodes指针数组中，返回结点的个数n
 * @param nodes
 * @param keys
 * @return
 */
int GetNodes(Node ***nodes, int **keys){
    char op='y';
    int n=0;
    while(op!='n'){
        Node *temp=(Node *)malloc(sizeof(Node));
        printf("请输入新结点的主值key，若输入0为添加空结点:");
        scanf("%d", &temp->data.key);
        if(!temp->data.key){
            *nodes=(Node **)realloc(*nodes, sizeof(Node*)*(++n));//给结点序列增加空间
            if(*nodes) {
                temp->lChild = NULL, temp->rChild = NULL;//初始化左右孩子为空，不然后面创建时无法正确添加左右子
                (*nodes)[n - 1] = temp;//把新结点加入结点序列
                printf("添加成功！");
            }else return OVERFLOW;
        }
        else if(CheckKey(temp->data.key, keys)==OK){
            printf("请输入新结点的数据：");
            scanf("%20s", temp->data.others);
            *nodes=(Node **)realloc(*nodes, sizeof(Node*)*(++n));//给结点序列增加空间
            if(*nodes) {
                temp->lChild = NULL, temp->rChild = NULL;//初始化左右孩子为空，不然后面创建时无法正确添加左右子
                (*nodes)[n - 1] = temp;//把新结点加入结点序列
                printf("添加成功！");
            }else return OVERFLOW;
        }else{
            printf("输入的主值与已有结点主值重复，请检查。\n");
        }
        printf("需要继续添加结点吗？ y/n：");
        rewind(stdin);scanf("%c", &op);
    }
    return n;
}
status ReadFromFile(FILE* in, Node ***nodes, int **keys, int *n){
    if(!in)return ERROR;
    int key, op;
    char others[DATA_LEN+1];
    Node *temp;
    *n=0;
    while(fscanf(in, "%d %20s", &key, others)!=EOF){
        temp=(Node *)malloc(sizeof(Node));
        if(!temp) return OVERFLOW;
        temp->data.key=key;
        if(key&&CheckKey(key, keys)){
            printf("输入的主值%d与已有结点主值重复。\n", key);
            printf("输入1：跳过当前结点；\n");
            printf("输入2：中断读取。\n");
            printf("请选择操作：\n");
            rewind(stdin);scanf("%d", &op);
            if(op==1) continue;// 跳过当前结点
            else if(op==2) return OK;
        }
        strcpy(temp->data.others, others);
        temp->lChild=NULL;temp->rChild=NULL;
        *nodes=(Node **)realloc(*nodes, sizeof(Node *)*(*n+1));
        (*nodes)[*n]=temp;// 存入nodes序列
        (*n)++;
    }
    return OK;
}

/*-- main函数开始 --*/
int main() {
    int op=1;//选择操作用
    int i=0,result, listNum=1, nodeNum;//result用于存储函数返回的status，n用于指定操作的表数目
    int *selectedLists=NULL; //存储chooselist返回的动态表指针数组
    Tree tree;
    Node **nodes, temp, *p;
    nodes = (Node **)malloc(sizeof(Node *));//存储用户输入的结点序列指针的动态数组头
    *nodes = &temp;
    int *keys=(int*)malloc(sizeof(int));//存储已有结点的主值以便查重
    keys[0]=0;
    char filename[NAME_LEN+1];
    //进入系统
    while(op) {
        system("cls");
        printf("\n\n");
        printf("      Menu for Binary Tree On Linked Structure \n");
        printf("-------------------------------------------------\n");
        printf("    	  1. InitBiTree         \t2. DestroyBiTree\n");
        printf("    	  3. CreateBiTree       \t4. ClearBiTree\n");
        printf("    	  5. BiTreeEmpty        \t6. BiTreeDepth\n");
        printf("    	  7. Root               \t8. Value\n");
        printf("    	  9. Assign             \t10.Parent\n");
        printf("    	  11.LeftChild          \t12. RightChild\n");
        printf("    	  13. LeftSibling       \t14. RightSibling\n");
        printf("    	  15. InsertChild       \t16. DeleteChild\n");
        printf("    	  17. PreOrderTraverse  \t18. InOrderTraverse\n");
        printf("    	  19. PostOrderTraverse \t20. LevelOrderTraverse\n");
        printf("    	  21. Dir-LikePrint     \t22. ReadFromFile\n");
        printf("          0. Exit\n");
        printf("-------------------------------------------------\n");
        printf("    请选择你的操作[0~22]:");
        rewind(stdin);
        scanf("%d", &op);
        switch (op) {
            case 1:
                InitBiTree(&tree);
                break;
            case 2:
                if(!tree.root) {
                    printf("树不存在！");
                    printf("按任意键继续……");
                    rewind(stdin);getchar();
                    break;
                }
                DestroyBiTree(&tree);
                free(keys);
                break;
            case 3:
                listNum=GetNodes(&nodes, &keys);//TODO: listNum应在检查过结点序列正确性后再设置
                CreateBiTree(&tree, nodes, listNum);
                break;
            case 4:
                if(!tree.root) {
                    printf("树不存在！");
                    printf("按任意键继续……");
                    rewind(stdin);getchar();
                    break;
                }
                if (!ClearBiTree(tree.root, keys)) {
                    free(keys);
                    printf("已清空树。");
                }
                printf("按任意键继续……");
                rewind(stdin);
                getchar();
                break;
            case 5:
                if(!tree.root) {
                    printf("树不存在！");
                    printf("按任意键继续……");
                    rewind(stdin);getchar();
                    break;
                }
                if(BiTreeEmpty(tree)) printf("树为空。");
                else printf("树不为空。");
                printf("按任意键继续……");
                rewind(stdin);
                getchar();
                break;
            case 6:
                if(!tree.root) {
                    printf("树不存在！");
                    printf("按任意键继续……");
                    rewind(stdin);getchar();
                    break;
                }
                int level=1;
                result= BiTreeDepth(*tree.root, &level, 1);
                printf("树的深度为：%d。", result);
                printf("按任意键继续……");
                rewind(stdin);getchar();
                break;
            case 7:
                if(!tree.root) {
                    printf("树不存在！");
                    printf("按任意键继续……");
                    rewind(stdin);getchar();
                    break;
                }
                printf("树根为：");
                LinearlyPrint(tree.root, 0);
                printf("按任意键继续……");
                rewind(stdin);getchar();
                break;
            case 8:
                printf("请输入要获取的结点主值：");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    printf("获取的结点为：");
                    LinearlyPrint(Value(*tree.root, result), 0);
                }else{
                    printf("输入的主值不存在于树中，请确认。");
                }
                printf("按任意键继续……");
                rewind(stdin);getchar();
                break;
            case 9:
                printf("请输入要赋值的结点主值：");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    Node *n;
                    printf("要赋值的结点为：");
                    LinearlyPrint(n=Value(*tree.root, result), 0);
                    while (CheckKey(result, &keys)!=OK) {
                        printf("，请输入新的主值，输入0为保持原主值不变：");
                        scanf("%d", &result);
                        if (!result) {
                            printf("请输入新的数据：");
                            scanf("%20s", n->data.others);
                            printf("更改完成。");
                        } else if (CheckKey(result, &keys) == OK) {
                            n->data.key = result;
                            printf("请输入新的数据：");
                            scanf("%20s", n->data.others);
                            printf("更改完成。");
                        } else
                            printf("输入的新主值与已有主值重复，请检查。");
                    }
                }else{
                    printf("输入的主值不存在于树中，请确认。");
                }
                break;
            case 10:
                printf("请输入要查找双亲的结点主值：");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    Node *n=Value(*tree.root, result);
                    InOrderTraverse(*tree.root, Parent, *n, &n);
                    if(!n)printf("未找到该结点的双亲。");
                    else {
                        printf("该结点的双亲为：");
                        LinearlyPrint(n, 0);
                    }
                }else{
                    printf("输入的主值不存在于树中，请确认。");
                }
                printf("按任意键继续……");
                rewind(stdin);getchar();
                break;
            case 11:
                printf("请输入要查找左子的结点主值：");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    Node *n=LeftChild(tree.root, result);
                    if(!n)printf("未找到该结点的左子。");
                    else {
                        printf("该结点的左子为：");
                        LinearlyPrint(n, 0);
                    }
                }else{
                    printf("输入的主值不存在于树中，请确认。");
                }
                printf("按任意键继续……");
                rewind(stdin);getchar();
                break;
            case 12:
                printf("请输入要查找右子的结点主值：");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    Node *n=RightChild(tree.root, result);
                    if(!n)printf("未找到该结点的右子。");
                    else {
                        printf("该结点的右子为：");
                        LinearlyPrint(n, 0);
                    }
                }else{
                    printf("输入的主值不存在于树中，请确认。");
                }
                printf("按任意键继续……");
                rewind(stdin);getchar();
                break;
            case 13:
                printf("请输入要查找左兄弟的结点主值：");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    Node *n=Value(*tree.root, result);
                    InOrderTraverse(*tree.root, LeftSibling, *n, &n);
                    if(!n)printf("未找到该结点的左兄弟。");
                    else {
                        printf("该结点的左兄弟为：");
                        LinearlyPrint(n, 0);
                    }
                }else{
                    printf("输入的主值不存在于树中，请确认。");
                }
                printf("按任意键继续……");
                rewind(stdin);getchar();
                break;
            case 14:
                printf("请输入要查找右兄弟的结点主值：");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    Node *n=Value(*tree.root, result);
                    InOrderTraverse(*tree.root, RightSibling, *n, &n);
                    if(!n)printf("未找到该结点的右兄弟。");
                    else {
                        printf("该结点的右兄弟为：");
                        LinearlyPrint(n, 0);
                    }
                }else{
                    printf("输入的主值不存在于树中，请确认。");
                }
                printf("按任意键继续……");
                rewind(stdin);getchar();
                break;
            case 15:
                printf("请选择插入方式：1.手动输入；2.从文件中读取；3.取消：");
                scanf("%d", &op);
                Tree t;
                if(op==2){
                    printf("请输入待读取的文件名：");
                    scanf("%s",filename);
                    FILE *in = fopen(filename, "r");
                    if(!ReadFromFile(in, &nodes, &keys, &nodeNum)) {
                        if (!CreateBiTree(&t, nodes, nodeNum)) {
                            printf("读取成功！");
                        }
                    }else {
                        printf("读取出错！");
                        printf("按任意键继续……");
                        rewind(stdin);getchar();
                        break;
                    }
                }
                else if(op==1){
                    if(!ReadFromFile(stdin, &nodes, &keys, &nodeNum)) {
                        if (!CreateBiTree(&t, nodes, nodeNum)) {
                            printf("读取成功！");
                        }
                    }else {
                        printf("读取出错！");
                        printf("按任意键继续……");
                        rewind(stdin);getchar();
                        break;
                    }
                }
                else break;
                printf("请输入要插入子树的结点主值：");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    Node *n=Value(*tree.root, result);
                    printf("请输入要插入子树的哪一端，0.左子，1.右子：");
                    scanf("%d", &op);
                    InsertChild(tree.root, n, op, t.root);
                    printf("插入完成。");
                }else{
                    printf("输入的主值不存在于树中，请确认。");
                }
                op=1;
                printf("按任意键继续……");
                rewind(stdin);getchar();
                break;
            case 16:
                printf("请输入要删除子树的结点主值：");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    Node *n = Value(*tree.root, result);
                    printf("请输入要删除子树的哪一端，0.左子，1.右子：");
                    scanf("%d", &result);
                    DeleteChild(tree.root, n, result, keys);
                }else{
                    printf("输入的主值不存在于树中，请确认。");
                }
                printf("按任意键继续……");
                rewind(stdin);getchar();
                break;
            case 17:
                PreOrderTraverse(tree.root, DirectlyPrint, 0);
                printf("按任意键继续……");
                rewind(stdin);getchar();
                break;
            case 18:
                InOrderTraverse(*tree.root, DirectlyPrint, temp, &p);
                printf("按任意键继续……");
                rewind(stdin);getchar();
                break;
            case 19:
                PostOrderTraverse(tree.root, DirectlyPrint, &result);
                printf("按任意键继续……");
                rewind(stdin);getchar();
                break;
            case 20:
                LevelOrderTraverse(tree.root, DirectlyPrint, &temp);
                printf("按任意键继续……");
                rewind(stdin);getchar();
                break;
            case 21:
                result=0;
                PreOrderTraverse(tree.root, LevelPrint, 0);
                printf("遍历结束。");
                printf("按任意键继续……");
                rewind(stdin);getchar();
                break;
            case 22:
                printf("请输入待读取的文件名：");
                scanf("%s",filename);
                FILE *in = fopen(filename, "r");
                if(!ReadFromFile(in, &nodes, &keys, &nodeNum)) {
                    if (!CreateBiTree(&tree, nodes, nodeNum)) {
                        printf("读取成功！");
                        printf("按任意键继续……");
                        rewind(stdin);
                        getchar();
                    }
                }else {
                    printf("读取出错！");
                    printf("按任意键继续……");
                    rewind(stdin);
                    getchar();
                }
                break;
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
    ClearBiTree(T->root, NULL);//TODO: root是否在上一步已经被free了？
    T->root=NULL;
}
/**
 * 采取先序遍历带空结点的结点序列的方式迭代创建树。
 * @param T 树指针
 * @param nodes 先序遍历的节点序列指针数组
 * @param n
 * @return
 */
status CreateBiTree(Tree *T, Node **nodes, int n){
    if (!T->root)InitBiTree(T);
    if (!nodes[0]->data.key) return OK;// 若序列的首个结点为空则树为空
    Node **stack=(Node **)malloc(sizeof(Node *)*n);
    int i=0,j=0;// i遍历结点序列，j遍历有效结点栈
    for (i=0;i<n;i++){
        stack[j++] = nodes[i];// 第i个结点入栈，为空则稍后退栈（为了合并空与非空结点的逻辑）
        if(j>1) {
            // 若上个结点没有左孩子，则当前结点为上个结点的左孩子
            if(!stack[j-2]->lChild)stack[j - 2]->lChild = nodes[i];
            // 若上个结点有左孩子但没有右孩子，则新入栈的结点为上个结点的右孩子
            else if(!stack[j-2]->rChild)stack[j-2]->rChild = nodes[i];
            // 若左右孩子已满，且右孩子为空时可把三个都退栈，并检查之前的结点是否有满，若满了也退栈
            if(stack[j-2]->lChild && stack[j-2]->rChild) {
                if(!stack[j-2]->rChild->data.key) {
                    stack[j-2]->rChild=NULL;
                    if(!stack[j-2]->lChild->data.key) {
                        stack[j-2]->lChild=NULL;
                        j++;//若左子为空则补偿一位
                    }
                    j-=3;//注意此处退了左、右子和双亲
                    for(;j>0 && stack[j-1]->lChild && stack[j-1]->rChild;j--){
                        if(!stack[j-1]->lChild->data.key) stack[j-1]->lChild=NULL;
                        if(!stack[j-1]->rChild->data.key) stack[j-1]->rChild=NULL;
                    }
                    continue;
                }
            }
            if(!stack[j-1]->data.key) j--; // 若第i个结点为空则把该结点退栈
        }
    }
    T->root=*nodes;
    free(stack);
    return OK;
}
status ClearBiTree(BiTree *T, int *keys) {
    if(T->lChild && T->rChild){
        PostOrderTraverse(T->lChild, DeleteNode, keys);
        PostOrderTraverse(T->rChild, DeleteNode, keys);
        T->data.key=0;
        T->lChild=NULL, T->rChild=NULL;
    }
    return OK;
}
status BiTreeEmpty(Tree T){
    if(!T.root->data.key)return TRUE;
    else return FALSE;
}
int BiTreeDepth(BiTree T, int *level, int n) {
    if(n>*level) *level=n;
    if(T.lChild)BiTreeDepth(*T.lChild, level, n+1);
    if(T.rChild)BiTreeDepth(*T.rChild, level, n+1);
    return *level;
}
BiTree *Root(Tree T){
    return T.root;
}
/**
 * 根据输入的key寻找结点，返回找到结点的地址或NULL
 * @param T
 * @param key
 * @return
 */
BiTree *Value(BiTree T, int key){
    Node n, *p=NULL;
    n.data.key=key;
    InOrderTraverse(T, CprNodeByKey, n, &p);
    return p;
}
/**
 * 根据输入的key寻找结点并更改结点的值。返回OK或ERROR
 * @param T
 * @param n
 * @param value
 * @return
 * @todo 检查新主值的合法性在主函数中调用其他函数处理
 */
status Assign(BiTree T, Node *n, elem value, int **keys) {
    if(!n) return ERROR;
    n->data=value;
    return OK;
}
BiTree *Parent(BiTree *T, int key){
    if(T->lChild->data.key==key || T->rChild->data.key==key) return T;
    else return NULL;
}
BiTree *LeftChild(BiTree *T, int key){
    Node *value = Value(*T, key);
    if(!value) return NULL;
    return value->lChild;
}
BiTree *RightChild(BiTree *T, int key){
    Node *value = Value(*T, key);
    if(!value) return NULL;
    return value->rChild;
}
BiTree *LeftSibling(BiTree *T, int key){
    if(!T || T->rChild->data.key!=key) return NULL;
    else return T->lChild;
}
BiTree *RightSibling(BiTree *T, int key){
    if(!T || T->lChild->data.key!=key) return NULL;
    else return T->rChild;
}
status InsertChild(BiTree *T, Node *p, int LR, BiTree *child){
    Node *value=NULL, *temp;
    InOrderTraverse(*T, CprNodeByKey, *p, &value);
    if(!value)return ERROR;
    else{
        if(!LR){
            //插入为左子
            temp=value->lChild;
            value->lChild=child;
        }else{
            temp=value->rChild;
            value->rChild=child;
        }
        child->rChild=temp;
    }

}
status DeleteChild(BiTree *T, Node *p, int LR, int *keys) {
    if(!LR)PostOrderTraverse(p->lChild, DeleteNode, keys);
    else PostOrderTraverse(p->rChild, DeleteNode, keys);
    return OK;
}
status PreOrderTraverse(BiTree *T, int (*Visit)(BiTree *, int), int level){
    int n;
    Visit(T, level);
    if(T->lChild) PreOrderTraverse(T->lChild, Visit, level+1);
    if(T->rChild) PreOrderTraverse(T->rChild, Visit, level+1);
    return OK;
}
Node *InOrderTraverse(BiTree T, Node *(*Visit)(BiTree *, int), Node e, Node **value){
    if(T.lChild) InOrderTraverse(*T.lChild, Visit, e, value);
    if(Visit(&T, e.data.key),!*value)*value=Visit(&T, e.data.key);
    //此处有个问题，*value中存的是p中存放的地址，即*value指向p，现在p变了，但是p应该是另一个p，不会影响到*value
    //若用于查找结点，则不是该结点应返回NULL,是则返回该结点指针；若已经找到结点则不能覆盖
    if(T.rChild) InOrderTraverse(*T.rChild, Visit, e, value);
    return *value;
}
status PostOrderTraverse(BiTree *T, Node(*Visit)(BiTree *, int *), int *keys){
    if(T->lChild) PostOrderTraverse(T->lChild, Visit, keys);
    if(T->rChild) PostOrderTraverse(T->rChild, Visit, keys);
    Visit(T, keys);//TODO: 后序输出时传给输出函数的参数会变成无意义的值.
    return OK;
}
status LevelOrderTraverse(BiTree *T, Node*(*Visit)(BiTree *, int), Node *value){
    Node **parents=NULL, **children=NULL;//每层的双亲指针栈和双亲栈对应的孩子指针栈
    parents=(Node **)malloc(sizeof(Node *));
    children=(Node **)malloc(sizeof(Node *));//为节省内存，栈中只需存储指向结点的指针
    children[0]=T;//孩子栈一开始只有一个根节点
    int p,c,pNum=0,cNum=1;//p遍历双亲栈，c遍历孩子栈，pNum为双亲栈长度，cNum为孩子栈长度
    Visit(T, 0);//先visit根节点
    do {
        free(parents);parents=children;
        children=(Node **)malloc(sizeof(Node *));//每次孩子栈都要重新分配
        pNum=cNum;cNum=0;//上一层的双亲栈作废，此层的双亲栈=上层的孩子栈
        for (p = 0; p<pNum; p++) {//开始本层双亲栈的遍历以遍历孩子
            if (parents[p]->lChild) {
                Visit(parents[p]->lChild, 0);
                children = (Node **) realloc(children, sizeof(Node *) * (cNum + 1));//给孩子栈扩容
                children[cNum++] = parents[p]->lChild;//把左孩子存入孩子栈，孩子栈索引+1
            }
            if (parents[p]->rChild) {
                Visit(parents[p]->rChild, 0);
                children = (Node **) realloc(children, sizeof(Node *) * (cNum + 1));//给孩子栈扩容
                children[cNum++] = parents[p]->rChild;//把右孩子存入孩子栈，孩子栈索引+1
            }
        }//双亲栈遍历结束
    }while(cNum!=0);
    free(parents);free(children);
    return OK;
}