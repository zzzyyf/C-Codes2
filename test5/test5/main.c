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
		if (op == 1) { //选择SAT
			printf("input .cnf file name:\n");
			scanf("%s", filename);
			if ((liteHead = read(filename, &root1)) && (liteHead2 = read2(filename, &root21))) {
				printf("读取未发现错误。\n");
				op = 0;
			}
			else {
				printf("读取失败。\n");
				op = 1;
			}
		}
		else { //选择数独
			isSudoku = 1;
			printf("Choose sudoku source: 1. read from file; 2. generate\n");
			scanf("%d", &op);
			if (op == 1) { //选择读取
				printf("input sudoku file name:\n");
				scanf("%s", filename);
				liteHead2 = malloc(sizeof(liteNode2) * 730); //多分配一个单元
				if (!liteHead2) return ERROR;
				for (int i = 1; i < 730; i++) {
					liteHead2[i].times = 0;
					liteHead2[i].isAssigned = FALSE;
					liteHead2[i].shortestLen = MAX_LITE_NUM;
					liteHead2[i].isLenChecked = FALSE;
				}
				if (convertSudoku(&root21, liteHead2, filename) != OK) {
					printf("读取失败。\n");
					op = 1;
				}
				else {
					printf("Choose function: 1. go to game; 2. watch solving process\n");
					scanf("%d", &op);
					if (op == 1) {
						//go to game
					}
					else {
						op = 0; //退出选择
					}
				}
			}
			else { //选择生成
				printf("to be finished...");
				op = 1;
			}

		}
		rewind(stdin); getchar();
	}
	op = 1;
	while (op) {
		system("cls");
		printf("  请选择要执行的操作：\n");
		printf("    1. 检查读取\n");
		printf("    2. 求解问题\n");
		printf("    3. 解析范式\n");
		printf("    0. 退出程序\n");
		scanf("%d", &op); //此处发现若op为char则会越界
		switch (op) {
		case 1:
			strcpy(temp, filename);
			if (isSudoku != 1) {
				op = traverse(strcat(temp, ".check1"), &root1);
				if (op != ERROR) printf("输出至文件%s完成。", temp);
				else printf("输出至文件失败。");
			}
			op = traverse2(strcat(temp, ".check2"), &root21);
			if (op != ERROR) printf("输出至文件%s完成。", temp);
			else printf("输出至文件失败。");
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
					printf("原算法解出一解，耗时%.2fms.\n", t1);
					if ((result = checkSolution(solution, &root1)) != 0)printf("求解错误，第%d行不满足。\n", result);
					else printf("解验证完毕，满足。\n");
				}
				else if (result == FALSE)printf("原算法解得此SAT问题无解，耗时%.2fms.\n", t1);
				else printf("原算法超时，耗时%.2fms。\n", t1);

				if (result != ERROR) {
					//得到同名.res文件名
					ps = strstr(filename, ".cnf");
					strcpy(ps, ".res");
					if (output(solution, t1, filename) == ERROR) {
						printf("输出至文件%s出错。\n", filename);
						rewind(stdin);
						getchar();
					}
					else {
						printf("输出至文件%s成功。\n", filename);
						rewind(stdin);
						getchar();
					}
					free(solution); //释放解所占内存
				}
			}
			start = clock();
			result = optDPLL(&root21, liteHead2, &solution2);
			end = clock();
			t2 = (int)((double)(end - start) / CLK_TCK * 1000);
			if (result == TRUE) {
				printf("优化算法解出一解，耗时%.2fms.\n", t2);
				if ((result = checkSolution2(solution2, &root21)) != 0)printf("求解错误，第%d行不满足。\n", result);
				else printf("解验证完毕，满足。\n");
			}
			else if (result == FALSE) printf("优化算法解得此SAT问题无解，耗时%.2fms.\n", t2);
			else printf("优化算法超时，耗时%.2fms.。\n", t2);
			if (result != ERROR) {
				//得到同名.res文件名
				ps = strstr(filename, ".cnf");
				if(ps)strcpy(ps, ".res");
				if (output2(solution2, t2, filename) == ERROR) {
					printf("输出至文件%s出错。\n", filename);
					rewind(stdin);
					getchar();
				}
				else {
					printf("输出至文件%s成功。\n", filename);
					rewind(stdin);
					getchar();
				}
				free(solution2); //释放解所占内存
				printf("优化率为%.1f%%。\n", (t1 - t2) / t1 * 100);
			}
			rewind(stdin);
			getchar();
			break;
		case 3:
			strcpy(temp, filename);
			if (isSudoku != 1) {
				op = analyze1(strcat(temp, ".anl1"), &root1);
				if (op != ERROR) printf("输出至文件%s完成。", temp);
				else printf("输出至文件失败。");
			}
			op = traverse2(strcat(temp, ".anl2"), &root21);
			if (op != ERROR) printf("输出至文件%s完成。", temp);
			else printf("输出至文件失败。");
			rewind(stdin);
			getchar();
			break;
		case 0:
			//原算法释放内存
			if (isSudoku != 1) {
				for (pc = root1.chainHead; pc->next; deleteClause(pc)); //删除chainHead后面的所有子句
				free(pc);
				free(liteHead);
			}
			//TODO: 优化算法释放内存待完成
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
	fprintf(out, "\nt %.2f\n", time); //输出时间
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
	fprintf(out, "\nt %.2f\n", time); //输出时间
	fclose(out);
	return OK;
}
