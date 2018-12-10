#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR -3
#define OVERFLOW -2
#define DATA_LEN 20 //Ԫ�ص�others�ֶ����ó���
#define MAX_TREES 5 //ͬʱ��������������Ŀ
#define NAME_LEN 20 //��������󳤶�
typedef int status;
typedef struct Elem{
    int key; //key==0ʱ�ж����ڵĽ��Ϊ��
    //TODO: key������ֵ����Ϊ0
    //TODO: ע��key��Ψһ��ֵ���ڲ�������ʱӦȷ��
    char others[DATA_LEN+1];
}elem;
typedef struct Node{
    elem data;
    struct Node *lChild, *rChild;
}BiTree, node;
typedef struct{
    BiTree *root;//ΪNULLʱ��������
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
}//ֱ�����Ԫ��
BiTree LevelPrint(BiTree T, int useless){
    int depth=BiTreeDepth(T);
    for (;depth>0;depth--)
        printf(" ");
    LinearlyPrint(T, useless);
} //�������Ԫ�أ����α���ʱ��
BiTree DeleteNode(BiTree T, int useless){
    free(&T);
}//ɾ����㣬����Clear����
node *FindNode(BiTree T, int key);

int main() {
    int op=1;//ѡ�������
    int i=0,result, n=1;//result���ڴ洢�������ص�status��n����ָ�������ı���Ŀ
    int *selectedLists=NULL; //�洢chooselist���صĶ�̬��ָ������
    //����ϵͳ
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
        printf("    ��ѡ����Ĳ���[0~20]:");
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
    printf("�������½���������");
    scanf("%20s", T->name);//TODO: �˴���20ΪNAME_LEN�����ƺ�����ֱ���ú����
    T->root =(BiTree*)malloc(sizeof(BiTree));
    if (!T->root) return OVERFLOW;//����ʧ���򷵻�OVERFLOW
    T->root->data.key=-1;//��Ϊ��0��ʾ���ڣ�-1��ʾδ��ʼ������
    T->root->lChild=NULL;T->root->rChild=NULL;//��ʼ��ָ����
    return OK;
}
status DestroyBiTree(Tree *T){
    ClearBiTree(T->root);
    free(T->root);//TODO: root�Ƿ�����һ���Ѿ���free�ˣ�
    T->root=NULL;
}
/**
 * ��ȡ����������ս��Ľ�����еķ�ʽ������������
 * @param T ��ָ��
 * @param nodes ��������Ľڵ�����
 * @param n
 * @return
 */
status CreateBiTree(Tree *T, node *nodes, int n){
    if (!T->root)InitBiTree(T);
    if (!nodes[0].data.key) return OK;//�����е��׸����Ϊ������Ϊ��
    node *stack[n];
    int i=0,j=0;//i����������У�j����ջ
    for (i=0;i<n;i++){
        if(nodes[i].data.key) //����i����㲻Ϊ��
            stack[j++] = &nodes[i];//��i�������ջ��Ϊ������ջ
        if(j>0) {
            //���ϸ����û�����ӣ�������ջ�Ľ��Ϊ�ϸ���������
            if(!stack[j-1]->lChild)stack[j - 1]->lChild = &nodes[i];
            //���ϸ���������ӵ�û���Һ��ӣ�������ջ�Ľ��Ϊ�ϸ������Һ���
            else if(!stack[j-1]->rChild)stack[j-1]->rChild=&nodes[i];
            //�����Һ������������ջ
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