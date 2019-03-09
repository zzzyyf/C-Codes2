#include "funcs.h"

/**
* ���Ż��㷨��ȡ�����������������������
* ������0��0�п�ʼ����8��8��ֹ��
*/
int convertSudoku(root2 *r, liteNode2 *l, char *filename) {
	FILE *in = fopen(filename, "r");
	if (!in) return ERROR;
	int n, x, row=0, column=0, result;
	int i, j, k;
	char c[2];
	//�����Ӿ�����ͷ���
	r->chainHead = malloc(sizeof(clauseHead2));
	if (!r->chainHead) return ERROR;
	r->chainHead->head = NULL;
	r->chainHead->next = NULL;
	clauseHead2 *pc = r->chainHead;

	//��Ԫ��001-889��ת��ʱ����һ�¼��ɡ�
	//�ȶ�ȡ��ת�����з�0��
	while (result = fscanf(in, "%1[0-9]", c), result == 1 && row<9) {
		if (c[0]-'0') {
			result = 81 * row + 9 * column + c[0]-'0'; //��ȡ�����ֵ�˳��Ҳ�ǻ��㹫ʽ
			if (addClause2(pc)==NULL) return ERROR;
			pc = pc->next;
			if (addNode2(pc, result, TRUE) == NULL) return ERROR;
			l[result].times++;
			setLiteInfo(pc, l);
		}
		column = (column + 1) % 9;
		if (!column) row++;
	}
	
	//����������ı�Ҫ����
	//һ���ո�������һ�����֣���������x��y�У�����һ��z[1,9]��xy� e.g. 111, 112, 113, ..., 119; 121, 122, ..., 129; 131, ..., 999
	for (row = 0; row < 9; row++) {
		if (addClause2(pc) == NULL) return ERROR;
		pc = pc->next;
		for (column = 0; column < 9; column++) {
			for (n = 1; n < 10; n++) {
				addNode2(pc, 81 * row + 9 * column + n, TRUE);
				l[81 * row + 9 * column + n].times++;
			}
		}
		setLiteInfo(pc, l);
	}
	//ÿ��������ÿ���������һ�Σ���������y�У�����z[1,9]�����е�x[1,8]�����е�n[x+1,9]��Ҫôz����xy��Ҫôz����iy��e.g. -111, -211; -112, -212;
	for (column = 0; column < 9; column++) {
		for (n = 1; n < 10; n++) {
			for (row = 0; row < 8; row++) {
				for (x = row + 1; x < 9; x++) {
					if (addClause2(pc) == NULL) return ERROR;
					pc = pc->next;
					if (addNode2(pc, 81 * row + 9 * column + n, FALSE) == NULL) return ERROR;
					l[81 * row + 9 * column + n].times++;
					if (addNode2(pc, 81 * x + 9 * column + n, FALSE) == NULL) return ERROR; 
					l[81 * x + 9 * column + n].times++;
					setLiteInfo(pc, l);
				}
			}
		}
	}
	//ÿ��������ÿ���������һ�Σ���������x�У�����z[1,9]�����е�y[1,8]�����е�n[y+1,9]��Ҫôz����xy��Ҫôz����xi�� e.g. -111, -121; -112, -122;
	for (row = 0; row < 9; row++) {
		for (n = 1; n < 10; n++) {
			for (column = 0; column < 8; column++) {
				for (x = column + 1; x < 9; x++) {
					if (addClause2(pc) == NULL) return ERROR;
					pc = pc->next;
					if (addNode2(pc, 81 * row + 9 * column + n, FALSE)== NULL) return ERROR; 
					l[81 * row + 9 * column + n].times++;
					if (addNode2(pc, 81 * row + 9 * x + n, FALSE)== NULL) return ERROR; 
					l[81 * row + 9 * x + n].times++;
					setLiteInfo(pc, l);
				}
			}
		}
	}
	//ÿ��������ÿ�Ź����������һ�Σ����Ե�n�е�j�еľŹ����ÿһ��(3i+x)(3j+y)����Ҫôz����(3i+x)(3j+y)��Ҫôz������һ������·���(3i+x)(3j+k)��k[y+1,3]����
	for (n = 1; n < 10; n++) {
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				for (row = 0; row < 3; row++) {
					for (column = 0; column < 3; column++) {
						for (x = column + 1; x < 3; x++) {
							if (addClause2(pc) == NULL) return ERROR;
							pc = pc->next;
							if (addNode2(pc, 81 * (3*i+row) + 9 * (3*j+column) + n, FALSE)== NULL) return ERROR; 
							l[81 * (3 * i + row) + 9 * (3 * j + column) + n].times++;
							if (addNode2(pc, 81 * (3*i+row) + 9 * (3*j+x) + n, FALSE)== NULL) return ERROR; 
							l[81 * (3 * i + row) + 9 * (3 * j + x) + n].times++;
							setLiteInfo(pc, l);
						}
					}
				}
			}
		}
	}

	//�Ե�n�е�j�еľŹ����ÿһ��(3i+x)(3j+y)��(3i+k)(3j+l)��k[x+1,3]��l[1,3]��Ҫôz����(3i+x)(3j+y)��Ҫôz������һ�����һ�е�������
	for (n = 1; n < 10; n++) {
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 3; j++) {
				for (row = 0; row < 3; row++) {
					for (column = 0; column < 3; column++) {
						for (x = column + 1; x < 3; x++) {
							for (k = 0; k < 3; k++) {
								if (addClause2(pc) == NULL) return ERROR;
								pc = pc->next;
								if (addNode2(pc, 81 * (3 * i + row) + 9 * (3 * j + column) + n, FALSE)== NULL) return ERROR; 
								if (addNode2(pc, 81 * (3 * i + x) + 9 * (3 * j + k) + n, FALSE)== NULL) return ERROR; 
								setLiteInfo(pc, l);
							}
						}
					}
				}
			}
		}
	}
	fclose(in);
	return OK;
}

