#include "funcs.h"

/**
* 用优化算法读取数独、生成数独、数独求解
* 数独从0行0列开始，到8行8列止。
*/
int convertSudoku(root2 *r, liteNode2 *l, char *filename) {
	FILE *in = fopen(filename, "r");
	if (!in) return ERROR;
	int n, x, row=0, column=0, result;
	int i, j, k;
	char c[2];
	//分配子句链的头结点
	r->chainHead = malloc(sizeof(clauseHead2));
	if (!r->chainHead) return ERROR;
	r->chainHead->head = NULL;
	r->chainHead->next = NULL;
	clauseHead2 *pc = r->chainHead;

	//变元从001-889，转换时换算一下即可。
	//先读取并转换所有非0格
	while (result = fscanf(in, "%1[0-9]", c), result == 1 && row<9) {
		if (c[0]-'0') {
			result = 81 * row + 9 * column + c[0]-'0'; //获取该数字的顺序，也是换算公式
			if (addClause2(pc)==NULL) return ERROR;
			pc = pc->next;
			if (addNode2(pc, result, TRUE) == NULL) return ERROR;
			l[result].times++;
			setLiteInfo(pc, l);
		}
		column = (column + 1) % 9;
		if (!column) row++;
	}
	
	//再添加数独的必要条件
	//一个空格至少有一个数字，即对所有x行y列，存在一个z[1,9]在xy里。 e.g. 111, 112, 113, ..., 119; 121, 122, ..., 129; 131, ..., 999
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
	//每个数字在每行至多出现一次，即对所有y列，所有z[1,9]，所有的x[1,8]和所有的n[x+1,9]，要么z不在xy，要么z不在iy。e.g. -111, -211; -112, -212;
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
	//每个数字在每列至多出现一次，即对所有x行，所有z[1,9]，所有的y[1,8]和所有的n[y+1,9]，要么z不在xy，要么z不在xi。 e.g. -111, -121; -112, -122;
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
	//每个数字在每九宫格至多出现一次，即对第n行第j列的九宫格的每一格（(3i+x)(3j+y)），要么z不在(3i+x)(3j+y)，要么z不在那一格的正下方（(3i+x)(3j+k)，k[y+1,3]）；
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

	//对第n行第j列的九宫格的每一格(3i+x)(3j+y)和(3i+k)(3j+l)，k[x+1,3]，l[1,3]，要么z不在(3i+x)(3j+y)，要么z不在那一格的下一行的三格内
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

