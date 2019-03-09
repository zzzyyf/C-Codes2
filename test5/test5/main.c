#include "funcs.h"
#include <string.h>

int main() {
	char filename[MAX_FILE_NAME], temp[MAX_FILE_NAME], *ps;
	int op = 1, isSudoku=0;
	clauseHead *pc;
	liteNode *liteHead, *solution = NULL;
	liteNode2 *liteHead2=NULL, *solution2 = NULL;
	extern clock_t start, end;
	root root1;
	root2 root21;
	double t1, t2;
	unsigned result;
	while (op) {
		system("cls");
		printf("Choose function: 1. SAT Problem; 2. Sudoku Game\n");
		scanf("%d", &op);
		if (op == 1) { //ѡ��SAT
			printf("input .cnf file name:\n");
			scanf("%s", filename);
			if ((liteHead = read(filename, &root1)) && (liteHead2 = read2(filename, &root21))) {
				printf("��ȡδ���ִ���\n");
				op = 0;
			}
			else {
				printf("��ȡʧ�ܡ�\n");
				op = 1;
			}
		}
		else { //ѡ������
			isSudoku = 1;
			printf("Choose sudoku source: 1. read from file; 2. generate\n");
			scanf("%d", &op);
			if (op == 1) { //ѡ���ȡ
				printf("input sudoku file name:\n");
				scanf("%s", filename);
				liteHead2 = malloc(sizeof(liteNode2) * 730); //�����һ����Ԫ
				if (!liteHead2) return ERROR;
				for (int i = 1; i < 730; i++) {
					liteHead2[i].times = 0;
					liteHead2[i].isAssigned = FALSE;
					liteHead2[i].shortestLen = MAX_LITE_NUM;
					liteHead2[i].isLenChecked = FALSE;
				}
				if (convertSudoku(&root21, liteHead2, filename) != OK) {
					printf("��ȡʧ�ܡ�\n");
					op = 1;
				}
				else {
					printf("Choose function: 1. go to game; 2. watch solving process\n");
					scanf("%d", &op);
					if (op == 1) {
						//go to game
					}
					else {
						op = 0; //�˳�ѡ��
					}
				}
			}
			else { //ѡ������
				printf("to be finished...");
				op = 1;
			}

		}
		rewind(stdin); getchar();
	}
	op = 1;
	while (op) {
		system("cls");
		printf("  ��ѡ��Ҫִ�еĲ�����\n");
		printf("    1. ����ȡ\n");
		printf("    2. �������\n");
		printf("    3. ������ʽ\n");
		printf("    0. �˳�����\n");
		scanf("%d", &op); //�˴�������opΪchar���Խ��
		switch (op) {
		case 1:
			strcpy(temp, filename);
			if (isSudoku != 1) {
				op = traverse(strcat(temp, ".check1"), &root1);
				if (op != ERROR) printf("������ļ�%s��ɡ�", temp);
				else printf("������ļ�ʧ�ܡ�");
			}
			op = traverse2(strcat(temp, ".check2"), &root21);
			if (op != ERROR) printf("������ļ�%s��ɡ�", temp);
			else printf("������ļ�ʧ�ܡ�");
			rewind(stdin);
			getchar();
			break;
		case 2:
			if (isSudoku != 1) {
				start = clock();
				result = DPLL(&root1, liteHead, &solution, 0, TRUE);
				end = clock();
				t1 = ((double)(end - start) / CLK_TCK * 1000);
				if (result == TRUE) {
					printf("ԭ�㷨���һ�⣬��ʱ%.2fms.\n", t1);
					if ((result = checkSolution(solution, &root1)) != 0)printf("�����󣬵�%d�в����㡣\n", result);
					else printf("����֤��ϣ����㡣\n");
				}
				else if (result == FALSE)printf("ԭ�㷨��ô�SAT�����޽⣬��ʱ%.2fms.\n", t1);
				else printf("ԭ�㷨��ʱ����ʱ%.2fms��\n", t1);

				if (result != ERROR) {
					//�õ�ͬ��.res�ļ���
					ps = strstr(filename, ".cnf");
					strcpy(ps, ".res");
					if (output(solution, t1, filename) == ERROR) {
						printf("������ļ�%s����\n", filename);
						rewind(stdin);
						getchar();
					}
					else {
						printf("������ļ�%s�ɹ���\n", filename);
						rewind(stdin);
						getchar();
					}
					free(solution); //�ͷŽ���ռ�ڴ�
				}
			}
			start = clock();
			result = optDPLL(&root21, liteHead2, &solution2);
			end = clock();
			t2 = (int)((double)(end - start) / CLK_TCK * 1000);
			if (result == TRUE) {
				printf("�Ż��㷨���һ�⣬��ʱ%.2fms.\n", t2);
				if ((result = checkSolution2(solution2, &root21)) != 0)printf("�����󣬵�%d�в����㡣\n", result);
				else printf("����֤��ϣ����㡣\n");
			}
			else if (result == FALSE) printf("�Ż��㷨��ô�SAT�����޽⣬��ʱ%.2fms.\n", t2);
			else printf("�Ż��㷨��ʱ����ʱ%.2fms.��\n", t2);
			if (result != ERROR) {
				//�õ�ͬ��.res�ļ���
				ps = strstr(filename, ".cnf");
				if(ps)strcpy(ps, ".res");
				if (output2(solution2, t2, filename) == ERROR) {
					printf("������ļ�%s����\n", filename);
					rewind(stdin);
					getchar();
				}
				else {
					printf("������ļ�%s�ɹ���\n", filename);
					rewind(stdin);
					getchar();
				}
				free(solution2); //�ͷŽ���ռ�ڴ�
				printf("�Ż���Ϊ%.1f%%��\n", (t1 - t2) / t1 * 100);
			}
			rewind(stdin);
			getchar();
			break;
		case 3:
			strcpy(temp, filename);
			if (isSudoku != 1) {
				op = analyze1(strcat(temp, ".anl1"), &root1);
				if (op != ERROR) printf("������ļ�%s��ɡ�", temp);
				else printf("������ļ�ʧ�ܡ�");
			}
			op = traverse2(strcat(temp, ".anl2"), &root21);
			if (op != ERROR) printf("������ļ�%s��ɡ�", temp);
			else printf("������ļ�ʧ�ܡ�");
			rewind(stdin);
			getchar();
			break;
		case 0:
			//ԭ�㷨�ͷ��ڴ�
			if (isSudoku != 1) {
				for (pc = root1.chainHead; pc->next; deleteClause(pc)); //ɾ��chainHead����������Ӿ�
				free(pc);
				free(liteHead);
			}
			//TODO: �Ż��㷨�ͷ��ڴ�����
			return 0;
		default:
			break;
		}
	}
	return 0;
}

int output(liteNode *solution, double time, char *filename) {
	FILE *out = fopen(filename, "w");
	if (!out) return ERROR;
	fprintf(out, "s %d\n", solution != NULL ? 1 : 0);
	if (solution) {
		fprintf(out, "v ");
		for (int i = 1; i < solution[0].shortestLen + 1; i++) {
			if (solution[i].value == FALSE)fprintf(out, "-");
			fprintf(out, "%d ", i);
		}
	}
	fprintf(out, "\nt %.2f\n", time); //���ʱ��
	fclose(out);
	return OK;
}

int output2(liteNode2 *solution, double time, char *filename) {
	FILE *out = fopen(filename, "w");
	if (!out) return ERROR;
	fprintf(out, "s %d\n", solution != NULL ? 1 : 0);
	if (solution) {
		fprintf(out, "v ");
		for (int i = 1; i < solution[0].shortestLen + 1; i++) {
			if (solution[i].value == FALSE)fprintf(out, "-");
			fprintf(out, "%d ", i);
		}
	}
	fprintf(out, "\nt %.2f\n", time); //���ʱ��
	fclose(out);
	return OK;
}
