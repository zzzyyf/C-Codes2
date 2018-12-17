#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    //TODO: key������ֵ����Ϊ0���������ô洢key��������0��β�����
    //TODO: ע��key��Ψһ��ֵ�����������ʱӦȷ��
    char others[DATA_LEN+1];
}elem;
typedef struct Node{
    elem data;
    struct Node *lChild, *rChild;
}BiTree, Node;
typedef struct{
    BiTree *root;//ΪNULLʱ��������
    char name[NAME_LEN+1];
    int length;//���Ľ����
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
status PreOrderTraverse(BiTree *T, int (*Visit)(BiTree *, int), int level);//level�������δ�ӡʱ�������
Node * InOrderTraverse(BiTree T, Node *(*Visit)(BiTree *, int), Node e, Node **value);
status PostOrderTraverse(BiTree *T, Node(*Visit)(BiTree *, int *), int *keys);//��Ϊ�������������ա�ɾ�������贫ָ��
status LevelOrderTraverse(BiTree *T, Node*(*Visit)(BiTree *, int), Node *value);

/*-- ����Ϊ�Զ��帨������ --*/
BiTree *LinearlyPrint(BiTree *T, int useless){
    printf("%d | %s", T->data.key, T->data.others);
    return 0;
}//���Ԫ�ص���ֵ������
BiTree *DirectlyPrint(BiTree *T, int useless){
    printf("%s ", T->data.others);
    return 0;
}//ֱ�����Ԫ�ص�����
int *LevelPrint(BiTree *T, int level){
    for (;level>0;level--)
        printf(" ");
    LinearlyPrint(T, level);
    printf("\n");
    return 0;
} //�������Ԫ�أ����α���ʱ��
BiTree DeleteNode(BiTree *T, int *keys){
    Node n=*T;
    free(T);
    int i=0;
    for(;keys[i]!=n.data.key;i++);//TODO: ���ܻ���ѭ��
    keys[i]=0;
    return n;
}//ɾ����㣬����Clear����
Node *CprNodeByKey(BiTree *T, int key){
    if (T->data.key==key) return T;
    else return NULL;
}//ͨ���Ƚ�key���Ƚ��������
status FindNodeByData(BiTree *T, Node *n){
}
status CheckKey(int key, int **keys){
    int i;
    for(i=0;(*keys)[i];i++){
        if(key==(*keys)[i])return ERROR;
    }
    (*keys)[i]=key;//���һ��key�滻Ϊ��ǰkey
    *keys=realloc(*keys, sizeof(int)*(i+2));//��keys����һ���ռ�
    (*keys)[i+1]=0;//��0��β
    return OK;
}//������key�Ѵ����򷵻�ERROR��������������½����
/**
 * �������룬������Ľ���ָ�����nodesָ�������У����ؽ��ĸ���n
 * @param nodes
 * @param keys
 * @return
 */
int GetNodes(Node ***nodes, int **keys){
    char op='y';
    int n=0;
    while(op!='n'){
        Node *temp=(Node *)malloc(sizeof(Node));
        printf("�������½�����ֵkey��������0Ϊ��ӿս��:");
        scanf("%d", &temp->data.key);
        if(!temp->data.key){
            *nodes=(Node **)realloc(*nodes, sizeof(Node*)*(++n));//������������ӿռ�
            if(*nodes) {
                temp->lChild = NULL, temp->rChild = NULL;//��ʼ�����Һ���Ϊ�գ���Ȼ���洴��ʱ�޷���ȷ���������
                (*nodes)[n - 1] = temp;//���½�����������
                printf("��ӳɹ���");
            }else return OVERFLOW;
        }
        else if(CheckKey(temp->data.key, keys)==OK){
            printf("�������½������ݣ�");
            scanf("%20s", temp->data.others);
            *nodes=(Node **)realloc(*nodes, sizeof(Node*)*(++n));//������������ӿռ�
            if(*nodes) {
                temp->lChild = NULL, temp->rChild = NULL;//��ʼ�����Һ���Ϊ�գ���Ȼ���洴��ʱ�޷���ȷ���������
                (*nodes)[n - 1] = temp;//���½�����������
                printf("��ӳɹ���");
            }else return OVERFLOW;
        }else{
            printf("�������ֵ�����н����ֵ�ظ������顣\n");
        }
        printf("��Ҫ������ӽ���� y/n��");
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
            printf("�������ֵ%d�����н����ֵ�ظ���\n", key);
            printf("����1��������ǰ��㣻\n");
            printf("����2���ж϶�ȡ��\n");
            printf("��ѡ�������\n");
            rewind(stdin);scanf("%d", &op);
            if(op==1) continue;// ������ǰ���
            else if(op==2) return OK;
        }
        strcpy(temp->data.others, others);
        temp->lChild=NULL;temp->rChild=NULL;
        *nodes=(Node **)realloc(*nodes, sizeof(Node *)*(*n+1));
        (*nodes)[*n]=temp;// ����nodes����
        (*n)++;
    }
    return OK;
}

/*-- main������ʼ --*/
int main() {
    int op=1;//ѡ�������
    int i=0,result, listNum=1, nodeNum;//result���ڴ洢�������ص�status��n����ָ�������ı���Ŀ
    int *selectedLists=NULL; //�洢chooselist���صĶ�̬��ָ������
    Tree tree;
    Node **nodes, temp, *p;
    nodes = (Node **)malloc(sizeof(Node *));//�洢�û�����Ľ������ָ��Ķ�̬����ͷ
    *nodes = &temp;
    int *keys=(int*)malloc(sizeof(int));//�洢���н�����ֵ�Ա����
    keys[0]=0;
    char filename[NAME_LEN+1];
    //����ϵͳ
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
        printf("    ��ѡ����Ĳ���[0~22]:");
        rewind(stdin);
        scanf("%d", &op);
        switch (op) {
            case 1:
                InitBiTree(&tree);
                break;
            case 2:
                if(!tree.root) {
                    printf("�������ڣ�");
                    printf("���������������");
                    rewind(stdin);getchar();
                    break;
                }
                DestroyBiTree(&tree);
                free(keys);
                break;
            case 3:
                listNum=GetNodes(&nodes, &keys);//TODO: listNumӦ�ڼ������������ȷ�Ժ�������
                CreateBiTree(&tree, nodes, listNum);
                break;
            case 4:
                if(!tree.root) {
                    printf("�������ڣ�");
                    printf("���������������");
                    rewind(stdin);getchar();
                    break;
                }
                if (!ClearBiTree(tree.root, keys)) {
                    free(keys);
                    printf("���������");
                }
                printf("���������������");
                rewind(stdin);
                getchar();
                break;
            case 5:
                if(!tree.root) {
                    printf("�������ڣ�");
                    printf("���������������");
                    rewind(stdin);getchar();
                    break;
                }
                if(BiTreeEmpty(tree)) printf("��Ϊ�ա�");
                else printf("����Ϊ�ա�");
                printf("���������������");
                rewind(stdin);
                getchar();
                break;
            case 6:
                if(!tree.root) {
                    printf("�������ڣ�");
                    printf("���������������");
                    rewind(stdin);getchar();
                    break;
                }
                int level=1;
                result= BiTreeDepth(*tree.root, &level, 1);
                printf("�������Ϊ��%d��", result);
                printf("���������������");
                rewind(stdin);getchar();
                break;
            case 7:
                if(!tree.root) {
                    printf("�������ڣ�");
                    printf("���������������");
                    rewind(stdin);getchar();
                    break;
                }
                printf("����Ϊ��");
                LinearlyPrint(tree.root, 0);
                printf("���������������");
                rewind(stdin);getchar();
                break;
            case 8:
                printf("������Ҫ��ȡ�Ľ����ֵ��");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    printf("��ȡ�Ľ��Ϊ��");
                    LinearlyPrint(Value(*tree.root, result), 0);
                }else{
                    printf("�������ֵ�����������У���ȷ�ϡ�");
                }
                printf("���������������");
                rewind(stdin);getchar();
                break;
            case 9:
                printf("������Ҫ��ֵ�Ľ����ֵ��");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    Node *n;
                    printf("Ҫ��ֵ�Ľ��Ϊ��");
                    LinearlyPrint(n=Value(*tree.root, result), 0);
                    while (CheckKey(result, &keys)!=OK) {
                        printf("���������µ���ֵ������0Ϊ����ԭ��ֵ���䣺");
                        scanf("%d", &result);
                        if (!result) {
                            printf("�������µ����ݣ�");
                            scanf("%20s", n->data.others);
                            printf("������ɡ�");
                        } else if (CheckKey(result, &keys) == OK) {
                            n->data.key = result;
                            printf("�������µ����ݣ�");
                            scanf("%20s", n->data.others);
                            printf("������ɡ�");
                        } else
                            printf("���������ֵ��������ֵ�ظ������顣");
                    }
                }else{
                    printf("�������ֵ�����������У���ȷ�ϡ�");
                }
                break;
            case 10:
                printf("������Ҫ����˫�׵Ľ����ֵ��");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    Node *n=Value(*tree.root, result);
                    InOrderTraverse(*tree.root, Parent, *n, &n);
                    if(!n)printf("δ�ҵ��ý���˫�ס�");
                    else {
                        printf("�ý���˫��Ϊ��");
                        LinearlyPrint(n, 0);
                    }
                }else{
                    printf("�������ֵ�����������У���ȷ�ϡ�");
                }
                printf("���������������");
                rewind(stdin);getchar();
                break;
            case 11:
                printf("������Ҫ�������ӵĽ����ֵ��");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    Node *n=LeftChild(tree.root, result);
                    if(!n)printf("δ�ҵ��ý������ӡ�");
                    else {
                        printf("�ý�������Ϊ��");
                        LinearlyPrint(n, 0);
                    }
                }else{
                    printf("�������ֵ�����������У���ȷ�ϡ�");
                }
                printf("���������������");
                rewind(stdin);getchar();
                break;
            case 12:
                printf("������Ҫ�������ӵĽ����ֵ��");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    Node *n=RightChild(tree.root, result);
                    if(!n)printf("δ�ҵ��ý������ӡ�");
                    else {
                        printf("�ý�������Ϊ��");
                        LinearlyPrint(n, 0);
                    }
                }else{
                    printf("�������ֵ�����������У���ȷ�ϡ�");
                }
                printf("���������������");
                rewind(stdin);getchar();
                break;
            case 13:
                printf("������Ҫ�������ֵܵĽ����ֵ��");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    Node *n=Value(*tree.root, result);
                    InOrderTraverse(*tree.root, LeftSibling, *n, &n);
                    if(!n)printf("δ�ҵ��ý������ֵܡ�");
                    else {
                        printf("�ý������ֵ�Ϊ��");
                        LinearlyPrint(n, 0);
                    }
                }else{
                    printf("�������ֵ�����������У���ȷ�ϡ�");
                }
                printf("���������������");
                rewind(stdin);getchar();
                break;
            case 14:
                printf("������Ҫ�������ֵܵĽ����ֵ��");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    Node *n=Value(*tree.root, result);
                    InOrderTraverse(*tree.root, RightSibling, *n, &n);
                    if(!n)printf("δ�ҵ��ý������ֵܡ�");
                    else {
                        printf("�ý������ֵ�Ϊ��");
                        LinearlyPrint(n, 0);
                    }
                }else{
                    printf("�������ֵ�����������У���ȷ�ϡ�");
                }
                printf("���������������");
                rewind(stdin);getchar();
                break;
            case 15:
                printf("��ѡ����뷽ʽ��1.�ֶ����룻2.���ļ��ж�ȡ��3.ȡ����");
                scanf("%d", &op);
                Tree t;
                if(op==2){
                    printf("���������ȡ���ļ�����");
                    scanf("%s",filename);
                    FILE *in = fopen(filename, "r");
                    if(!ReadFromFile(in, &nodes, &keys, &nodeNum)) {
                        if (!CreateBiTree(&t, nodes, nodeNum)) {
                            printf("��ȡ�ɹ���");
                        }
                    }else {
                        printf("��ȡ����");
                        printf("���������������");
                        rewind(stdin);getchar();
                        break;
                    }
                }
                else if(op==1){
                    if(!ReadFromFile(stdin, &nodes, &keys, &nodeNum)) {
                        if (!CreateBiTree(&t, nodes, nodeNum)) {
                            printf("��ȡ�ɹ���");
                        }
                    }else {
                        printf("��ȡ����");
                        printf("���������������");
                        rewind(stdin);getchar();
                        break;
                    }
                }
                else break;
                printf("������Ҫ���������Ľ����ֵ��");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    Node *n=Value(*tree.root, result);
                    printf("������Ҫ������������һ�ˣ�0.���ӣ�1.���ӣ�");
                    scanf("%d", &op);
                    InsertChild(tree.root, n, op, t.root);
                    printf("������ɡ�");
                }else{
                    printf("�������ֵ�����������У���ȷ�ϡ�");
                }
                op=1;
                printf("���������������");
                rewind(stdin);getchar();
                break;
            case 16:
                printf("������Ҫɾ�������Ľ����ֵ��");
                scanf("%d", &result);
                if(CheckKey(result, &keys)==ERROR) {
                    Node *n = Value(*tree.root, result);
                    printf("������Ҫɾ����������һ�ˣ�0.���ӣ�1.���ӣ�");
                    scanf("%d", &result);
                    DeleteChild(tree.root, n, result, keys);
                }else{
                    printf("�������ֵ�����������У���ȷ�ϡ�");
                }
                printf("���������������");
                rewind(stdin);getchar();
                break;
            case 17:
                PreOrderTraverse(tree.root, DirectlyPrint, 0);
                printf("���������������");
                rewind(stdin);getchar();
                break;
            case 18:
                InOrderTraverse(*tree.root, DirectlyPrint, temp, &p);
                printf("���������������");
                rewind(stdin);getchar();
                break;
            case 19:
                PostOrderTraverse(tree.root, DirectlyPrint, &result);
                printf("���������������");
                rewind(stdin);getchar();
                break;
            case 20:
                LevelOrderTraverse(tree.root, DirectlyPrint, &temp);
                printf("���������������");
                rewind(stdin);getchar();
                break;
            case 21:
                result=0;
                PreOrderTraverse(tree.root, LevelPrint, 0);
                printf("����������");
                printf("���������������");
                rewind(stdin);getchar();
                break;
            case 22:
                printf("���������ȡ���ļ�����");
                scanf("%s",filename);
                FILE *in = fopen(filename, "r");
                if(!ReadFromFile(in, &nodes, &keys, &nodeNum)) {
                    if (!CreateBiTree(&tree, nodes, nodeNum)) {
                        printf("��ȡ�ɹ���");
                        printf("���������������");
                        rewind(stdin);
                        getchar();
                    }
                }else {
                    printf("��ȡ����");
                    printf("���������������");
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
    printf("�������½���������");
    scanf("%20s", T->name);//TODO: �˴���20ΪNAME_LEN�����ƺ�����ֱ���ú����
    T->root =(BiTree*)malloc(sizeof(BiTree));
    if (!T->root) return OVERFLOW;//����ʧ���򷵻�OVERFLOW
    T->root->data.key=-1;//��Ϊ��0��ʾ���ڣ�-1��ʾδ��ʼ������
    T->root->lChild=NULL;T->root->rChild=NULL;//��ʼ��ָ����
    return OK;
}
status DestroyBiTree(Tree *T){
    ClearBiTree(T->root, NULL);//TODO: root�Ƿ�����һ���Ѿ���free�ˣ�
    T->root=NULL;
}
/**
 * ��ȡ����������ս��Ľ�����еķ�ʽ������������
 * @param T ��ָ��
 * @param nodes ��������Ľڵ�����ָ������
 * @param n
 * @return
 */
status CreateBiTree(Tree *T, Node **nodes, int n){
    if (!T->root)InitBiTree(T);
    if (!nodes[0]->data.key) return OK;// �����е��׸����Ϊ������Ϊ��
    Node **stack=(Node **)malloc(sizeof(Node *)*n);
    int i=0,j=0;// i����������У�j������Ч���ջ
    for (i=0;i<n;i++){
        stack[j++] = nodes[i];// ��i�������ջ��Ϊ�����Ժ���ջ��Ϊ�˺ϲ�����ǿս����߼���
        if(j>1) {
            // ���ϸ����û�����ӣ���ǰ���Ϊ�ϸ���������
            if(!stack[j-2]->lChild)stack[j - 2]->lChild = nodes[i];
            // ���ϸ���������ӵ�û���Һ��ӣ�������ջ�Ľ��Ϊ�ϸ������Һ���
            else if(!stack[j-2]->rChild)stack[j-2]->rChild = nodes[i];
            // �����Һ������������Һ���Ϊ��ʱ�ɰ���������ջ�������֮ǰ�Ľ���Ƿ�������������Ҳ��ջ
            if(stack[j-2]->lChild && stack[j-2]->rChild) {
                if(!stack[j-2]->rChild->data.key) {
                    stack[j-2]->rChild=NULL;
                    if(!stack[j-2]->lChild->data.key) {
                        stack[j-2]->lChild=NULL;
                        j++;//������Ϊ���򲹳�һλ
                    }
                    j-=3;//ע��˴����������Ӻ�˫��
                    for(;j>0 && stack[j-1]->lChild && stack[j-1]->rChild;j--){
                        if(!stack[j-1]->lChild->data.key) stack[j-1]->lChild=NULL;
                        if(!stack[j-1]->rChild->data.key) stack[j-1]->rChild=NULL;
                    }
                    continue;
                }
            }
            if(!stack[j-1]->data.key) j--; // ����i�����Ϊ����Ѹý����ջ
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
 * ���������keyѰ�ҽ�㣬�����ҵ����ĵ�ַ��NULL
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
 * ���������keyѰ�ҽ�㲢���Ľ���ֵ������OK��ERROR
 * @param T
 * @param n
 * @param value
 * @return
 * @todo �������ֵ�ĺϷ������������е���������������
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
            //����Ϊ����
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
    //�˴��и����⣬*value�д����p�д�ŵĵ�ַ����*valueָ��p������p���ˣ�����pӦ������һ��p������Ӱ�쵽*value
    //�����ڲ��ҽ�㣬���Ǹý��Ӧ����NULL,���򷵻ظý��ָ�룻���Ѿ��ҵ�������ܸ���
    if(T.rChild) InOrderTraverse(*T.rChild, Visit, e, value);
    return *value;
}
status PostOrderTraverse(BiTree *T, Node(*Visit)(BiTree *, int *), int *keys){
    if(T->lChild) PostOrderTraverse(T->lChild, Visit, keys);
    if(T->rChild) PostOrderTraverse(T->rChild, Visit, keys);
    Visit(T, keys);//TODO: �������ʱ������������Ĳ��������������ֵ.
    return OK;
}
status LevelOrderTraverse(BiTree *T, Node*(*Visit)(BiTree *, int), Node *value){
    Node **parents=NULL, **children=NULL;//ÿ���˫��ָ��ջ��˫��ջ��Ӧ�ĺ���ָ��ջ
    parents=(Node **)malloc(sizeof(Node *));
    children=(Node **)malloc(sizeof(Node *));//Ϊ��ʡ�ڴ棬ջ��ֻ��洢ָ�����ָ��
    children[0]=T;//����ջһ��ʼֻ��һ�����ڵ�
    int p,c,pNum=0,cNum=1;//p����˫��ջ��c��������ջ��pNumΪ˫��ջ���ȣ�cNumΪ����ջ����
    Visit(T, 0);//��visit���ڵ�
    do {
        free(parents);parents=children;
        children=(Node **)malloc(sizeof(Node *));//ÿ�κ���ջ��Ҫ���·���
        pNum=cNum;cNum=0;//��һ���˫��ջ���ϣ��˲��˫��ջ=�ϲ�ĺ���ջ
        for (p = 0; p<pNum; p++) {//��ʼ����˫��ջ�ı����Ա�������
            if (parents[p]->lChild) {
                Visit(parents[p]->lChild, 0);
                children = (Node **) realloc(children, sizeof(Node *) * (cNum + 1));//������ջ����
                children[cNum++] = parents[p]->lChild;//�����Ӵ��뺢��ջ������ջ����+1
            }
            if (parents[p]->rChild) {
                Visit(parents[p]->rChild, 0);
                children = (Node **) realloc(children, sizeof(Node *) * (cNum + 1));//������ջ����
                children[cNum++] = parents[p]->rChild;//���Һ��Ӵ��뺢��ջ������ջ����+1
            }
        }//˫��ջ��������
    }while(cNum!=0);
    free(parents);free(children);
    return OK;
}