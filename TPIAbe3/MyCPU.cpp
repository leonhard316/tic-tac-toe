#include "TTTPlugin.h"
#include <random>
#include <iostream>
bool HEAD_OR_TAIL;


/// <summary>
/// 人間操作かどうかを返す
/// </summary>
/// <returns>人間ならtrue, 機械ならfalse</returns>
bool IsHuman() {
	return false;
}

/// <summary>
/// 名前を返す
/// </summary>
/// <param name="buf">名前を格納する文字列</param>
/// <param name="bufsize">文字列バッファのサイズ</param>
void GetName(char* buf, size_t bufsize) {
	sprintf_s(buf, bufsize, "Abe's CPU");
}

// 1手目, 2手目, 3手目, 4手目以降を見分ける関数(戻り値　(先手)1, 2, 3, 4  (後手)-1, -2, -3, -4)
int Hand(int *board) {
	// 自分の駒の数
	int myHand = 0;
	// 相手の駒の数
	int yourHand = 0;

	for (int i = 0; i < 9; i++) {
		if (board[i] > 0) {
			myHand += 1;
		}
		else if (board[i] < 0) {
			yourHand += 1;
		}
	}

	if (myHand == 0 && yourHand == 0) {
		// 先手の一手目
		HEAD_OR_TAIL = true;
		return 1;
	}
	else if (myHand == 1 && yourHand == 1) {
		// 先手の二手目
		return 2;
	}
	else if (myHand == 2 && yourHand == 2) {
		// 先手の三手目
		return 3;
	}

	else if (yourHand == 1 && myHand == 0) {
		// 後手の一手目
		HEAD_OR_TAIL = false;
		return -1;
	}
	else if (yourHand == 2 && myHand == 1) {
		// 後手の二手目
		return -2;
	}
	else if (yourHand == 3 && myHand == 2) {
		// 後手の三手目
		return -3;
	}
	else {
		if (HEAD_OR_TAIL == true) {
			return 4;
		}
		else if (HEAD_OR_TAIL == false) {
			return -4;
		}
		return 4;
	}
}

// タテ，ヨコ，ナナメを走査して，条件にあうマスに評価値を与える(a, b, 0: 升目の値 評価値:value1, value2) 
void Scanning(int* board, int a, int b, int (&valueBoard)[9], int value1, int value2) {
	// タテ
	for (int col = 0; col < 3; col++) {

		// 自分
		if ((board[col] == a && board[col + 3] == b && board[col + 6] == 0) || (board[col] == b && board[col + 3] == a && board[col + 6] == 0)) {
			valueBoard[col + 6] += value1;
			//std::cout << col << "列目1" << std::endl;
		}
		if ((board[col] == a && board[col + 3] == 0 && board[col + 6] == b) || (board[col] == b && board[col + 3] == 0 && board[col + 6] == a)) {
			valueBoard[col + 3] += value1;
			//std::cout << col << "列目2" << std::endl;
		}
		if ((board[col] == 0 && board[col + 3] == a && board[col + 6] == b) || (board[col] == 0 && board[col + 3] == b && board[col + 6] == a)) {
			valueBoard[col] += value1;
			//std::cout << col << "列目3" << std::endl;
		}
		// 相手
		if ((board[col] == -a && board[col + 3] == -b && board[col + 6] == 0) || (board[col] == -b && board[col + 3] == -a && board[col + 6] == 0)) {
			valueBoard[col + 6] += value2;
			//std::cout << "相手:" << col << "列目1" << std::endl;
		}
		if ((board[col] == -a && board[col + 3] == 0 && board[col + 6] == -b) || (board[col] == -b && board[col + 3] == 0 && board[col + 6] == -a)) {
			valueBoard[col + 3] += value2;
			//std::cout << "相手:" << col << "列目2" << std::endl;

		}
		if ((board[col] == 0 && board[col + 3] == -a && board[col + 6] == -b) || (board[col] == 0 && board[col + 3] == -b && board[col + 6] == -a)) {
			valueBoard[col] += value2;
			//std::cout << "相手:" << col << "列目3" << std::endl;
		}
	}

	// ヨコ
	for (int row = 0; row < 3; row++) {
		// 自分
		if ((board[3 * row] == a && board[3 * row + 1] == b && board[3 * row + 1] == 0) || (board[3 * row] == b && board[3 * row + 1] == a && board[3 * row + 2] == 0)) {
			valueBoard[3 * row + 2] += value1;
			//std::cout << row << "行目1" << std::endl;

		}
		if ((board[3 * row] == a && board[3 * row + 1] == 0 && board[3 * row + 2] == b) || (board[3 * row] == b && board[3 * row + 1] == 0 && board[3 * row + 2] == a)) {
			valueBoard[3 * row + 1] += value1;
			//std::cout << row << "行目2" << std::endl;
		}
		if ((board[3 * row] == 0 && board[3 * row + 1] == a && board[3 * row + 2] == b) || (board[3 * row] == 0 && board[3 * row + 1] == b && board[3 * row + 2] == a)) {
			valueBoard[3 * row] += value1;
			//std::cout << row << "行目3" << std::endl;
		}
		// 相手
		if ((board[3 * row] == -a && board[3 * row + 1] == -b && board[3 * row + 2] == 0) || (board[3 * row] == -b && board[3 * row + 1] == -a && board[3 * row + 2] == 0)) {
			valueBoard[3 * row + 2] += value2;
			//std::cout << "相手:" << row << "行目1" << std::endl;
		}
		if ((board[3 * row] == -a && board[3 * row + 1] == 0 && board[3 * row + 2] == -b) || (board[3 * row] == -b && board[3 * row + 1] == 0 && board[3 * row + 2] == -a)) {
			valueBoard[3 * row + 1] += value2;
			//std::cout << "相手:" << row << "行目2" << std::endl;
		}
		if ((board[3 * row] == 0 && board[3 * row + 1] == -a && board[3 * row + 2] == -b) || (board[3 * row] == 0 && board[3 * row + 1] == -b && board[3 * row + 2] == -a)) {
			valueBoard[3 * row] += value2;
			//std::cout << "相手:" << row << "行目3" << std::endl;
		}
	}

	// ナナメ
	// 自分
	if ((board[0] == a && board[4] == b && board[8] == 0) || (board[0] == b && board[4] == a && board[8] == 0)) {
		valueBoard[8] += value1;
		//std::cout << "ナナメ1" << std::endl;
	}
	if ((board[0] == a && board[4] == 0 && board[8] == b) || (board[0] == b && board[4] == 0 && board[8] == a)) {
		valueBoard[4] += value1;
		//std::cout << "ナナメ2" << std::endl;
	}
	if ((board[0] == 0 && board[4] == a && board[8] == b) || (board[0] == 0 && board[4] == b && board[8] == a)) {
		valueBoard[0] += value1;
		//std::cout << "ナナメ3" << std::endl;
	}
	if ((board[2] == a && board[4] == b && board[6] == 0) || (board[2] == b && board[4] == a && board[6] == 0)) {
		valueBoard[6] += value1;
		//std::cout << "ナナメ4" << std::endl;
	}
	if ((board[2] == a && board[4] == 0 && board[6] == b) || (board[2] == b && board[4] == 0 && board[6] == a)) {
		valueBoard[4] += value1;
		//std::cout << "ナナメ5" << std::endl;
	}
	if ((board[2] == 0 && board[4] == a && board[6] == b) || (board[2] == 0 && board[4] == b && board[6] == a)) {
		valueBoard[2] += value1;
		//std::cout << "ナナメ6" << std::endl;
	}

	// 相手
	if ((board[0] == -a && board[4] == -b && board[8] == 0) || (board[0] == -b && board[4] == -a && board[8] == 0)) {
		valueBoard[8] += value2;
		//std::cout << "相手:" << "ナナメ-1" << std::endl;
	}
	if ((board[0] == -a && board[4] == 0 && board[8] == -b) || (board[0] == -b && board[4] == 0 && board[8] == -a)) {
		valueBoard[4] += value2;
		//std::cout << "相手:" << "ナナメ-2" << std::endl;
	}
	if ((board[0] == 0 && board[4] == -a && board[8] == -b) || (board[0] == 0 && board[4] == -b && board[8] == -a)) {
		valueBoard[0] += value2;
		//std::cout << "相手:" << "ナナメ-3" << std::endl;
	}
	if ((board[2] == -a && board[4] == -b && board[6] == 0) || (board[2] == -b && board[4] == -a && board[6] == 0)) {
		valueBoard[6] += value2;
		//std::cout << "相手:" << "ナナメ-4" << std::endl;
	}
	if ((board[2] == -a && board[4] == 0 && board[6] == -b) || (board[2] == -b && board[4] == 0 && board[6] == -a)) {
		valueBoard[4] += value2;
		//std::cout << "相手:" << "ナナメ-5" << std::endl;
	}
	if ((board[2] == 0 && board[4] == -a && board[6] == -b) || (board[2] == 0 && board[4] == -b && board[6] == -a)) {
		valueBoard[2] += value2;
		//std::cout << "相手:" << "ナナメ-6" << std::endl;
	}
}

// タテ，ヨコ，ナナメを走査して，条件にあうマスに評価値を与える(a, 0, 0: 升目の値 評価値:value1, value2) 
void SubScanning(int* board, int a, int (&valueBoard)[9], int value1, int value2) {
	// タテ
	for (int col = 0; col < 3; col++) {

		// 自分
		if (board[col] == a && board[col + 3] == 0 && board[col + 6] == 0) {
			valueBoard[col + 3] += value1;
			valueBoard[col + 6] += value1;
			//std::cout << col << "列目1" << std::endl;
		}
		if (board[col] == 0 && board[col + 3] == a && board[col + 6] == 0) {
			valueBoard[col] += value1;
			valueBoard[col + 6] += value1;
			//std::cout << col << "列目2" << std::endl;
		}
		if (board[col] == 0 && board[col + 3] == 0 && board[col + 6] == a) {
			valueBoard[col] += value1;
			valueBoard[col + 3] += value1;
			//std::cout << col << "列目3" << std::endl;
		}
		// 相手
		if (board[col] == -a && board[col + 3] == 0 && board[col + 6] == 0) {
			valueBoard[col + 3] += value2;
			valueBoard[col + 6] += value2;
			//std::cout << "相手:" << col << "列目1" << std::endl;
		}
		if (board[col] == 0 && board[col + 3] == -a && board[col + 6] == 0) {
			valueBoard[col] += value2;
			valueBoard[col + 6] += value2;
			//std::cout << "相手:" << col << "列目2" << std::endl;
		}
		if (board[col] == 0 && board[col + 3] == 0 && board[col + 6] == -a) {
			valueBoard[col] += value2;
			valueBoard[col + 3] += value2;
			//std::cout << "相手:" << col << "列目3" << std::endl;
		}
	}

	// ヨコ
	for (int row = 0; row < 3; row++) {
		// 自分
		if (board[3 * row] == a && board[3 * row + 1] == 0 && board[3 * row + 2] == 0) {
			valueBoard[3 * row + 1] += value1;
			valueBoard[3 * row + 2] += value1;
			//std::cout << row << "行目1" << std::endl;
		}
		if (board[3 * row] == 0 && board[3 * row + 1] == a && board[3 * row + 2] == 0) {
			valueBoard[3 * row] += value1;
			valueBoard[3 * row + 2] += value1;
			//std::cout << row << "行目2" << std::endl;
		}
		if (board[3 * row] == 0 && board[3 * row + 1] == 0 && board[3 * row + 2] == a) {
			valueBoard[3 * row] += value1;
			valueBoard[3 * row + 1] += value1;
			//std::cout << row << "行目3" << std::endl;
		}
		// 相手
		if (board[3 * row] == -a && board[3 * row + 1] == 0 && board[3 * row + 2] == 0) {
			valueBoard[3 * row + 1] += value2;
			valueBoard[3 * row + 2] += value2;
			//std::cout << "相手:" << row << "行目1" << std::endl;
		}
		if (board[3 * row] == 0 && board[3 * row + 1] == -a && board[3 * row + 2] == 0) {
			valueBoard[3 * row] += value2;
			valueBoard[3 * row + 2] += value2;
			//std::cout << "相手:" << row << "行目2" << std::endl;
		}
		if (board[3 * row] == 0 && board[3 * row + 1] == 0 && board[3 * row + 2] == -a) {
			valueBoard[3 * row] += value2;
			valueBoard[3 * row + 1] += value2;
			//std::cout << "相手:" << row << "行目3" << std::endl;
		}
		
	}

	// ナナメ
	// 自分
	if (board[0] == a && board[4] == 0 && board[8] == 0) {
		valueBoard[4] += value1;
		valueBoard[8] += value1;
		//std::cout << "ナナメ1" << std::endl;
	}
	if (board[0] == 0 && board[4] == 0 && board[8] == a) {
		valueBoard[0] += value1;
		valueBoard[4] += value1;
		//std::cout << "ナナメ2" << std::endl;
	}
	if (board[0] == 0 && board[4] == a && board[8] == 0) {
		valueBoard[0] += value1;
		valueBoard[8] += value1;
		//std::cout << "ナナメ3" << std::endl;
	}

	if (board[2] == a && board[4] == 0 && board[6] == 0) {
		valueBoard[4] += value1;
		valueBoard[6] += value1;
		//std::cout << "ナナメ4" << std::endl;
	}
	if (board[2] == 0 && board[4] == 0 && board[6] == a) {
		valueBoard[2] += value1;
		valueBoard[4] += value1;
		//std::cout << "ナナメ5" << std::endl;
	}
	if (board[2] == 0 && board[4] == a && board[6] == 0) {
		valueBoard[2] += value1;
		valueBoard[6] += value1;
		//std::cout << "ナナメ6" << std::endl;
	}

	// 相手
	if (board[0] == -a && board[4] == 0 && board[8] == 0) {
		valueBoard[4] += value2;
		valueBoard[8] += value2;
		//std::cout << "相手:" << "ナナメ-1" << std::endl;
	}
	if (board[0] == 0 && board[4] == 0 && board[8] == -a) {
		valueBoard[0] += value2;
		valueBoard[4] += value2;
		//std::cout << "相手:" << "ナナメ-2" << std::endl;
	}
	if (board[0] == 0 && board[4] == -a && board[8] == 0) {
		valueBoard[0] += value2;
		valueBoard[8] += value2;
		//std::cout << "相手:" << "ナナメ-3" << std::endl;
	}
	if (board[2] == -a && board[4] == 0 && board[6] == 0) {
		valueBoard[4] += value2;
		valueBoard[6] += value2;
		//std::cout << "相手:" << "ナナメ-4" << std::endl;
	}
	if (board[2] == 0 && board[4] == 0 && board[6] == -a) {
		valueBoard[2] += value2;
		valueBoard[4] += value2;
		//std::cout << "相手:" << "ナナメ-5" << std::endl;
	}
	if (board[2] == 0 && board[4] == -a && board[6] == 0) {
		valueBoard[2] += value2;
		valueBoard[6] += value2;
		//std::cout << "相手:" << "ナナメ-6" << std::endl;
	}
}

// 評価関数
void EvalBoard(int *board, int (&valueBoard)[9]) {
	int judge = Hand(board);

	// ランダム要素
	/*std::random_device rnd;
	int random1, random2;*/

	// 先手------------------------------------
	if (HEAD_OR_TAIL == true) {
		// 一手目
		if (judge == 1) {
			valueBoard[0] += 10;
			//std::cout << "一手目" << std::endl;
		}
		// 二手目
		else if (judge == 2) {
			if (board[5] == -1) {
				valueBoard[2] += 100;
			}
			else if (board[7] == -1) {
				valueBoard[6] += 100;
			}
			// 自分のリーチを作る
			SubScanning(board, 1, valueBoard, 2, 1);
			//std::cout << "二手目" << std::endl;
		}
		// 三手目
		else if (judge == 3) {
			/*int position;
			do {
				std::cout << "三手目はどこに置きますか？ (1:左上 - 9:右下): ";
				std::cin >> position;
			} while (position < 1 || position > 9 || board[position - 1] != 0);*/

			// リーチ以外の有利な盤
			SubScanning(board, 2, valueBoard, 10, 5);
			// リーチ
			Scanning(board, 1, 2, valueBoard, 100, 80);
			//std::cout << "三手目" << std::endl;
		}
		// 四手目以降
		else if (judge == 4) {
			// リーチ以外の有利な盤
			Scanning(board,  -1, 3, valueBoard, 8, 7);
			Scanning(board, -3, 2, valueBoard, 3, 0);
			Scanning(board, 1, -1, valueBoard, 1, 0);
			SubScanning(board, 3, valueBoard, 1, 1);
			// リーチ
			Scanning(board, 3, 2, valueBoard, 100, 80);
			//std::cout << "四手目以降" << std::endl;
		}
	}
	 // 後手------------------------------------
	else if (HEAD_OR_TAIL == false) {
		// 一手目
		if (judge == -1) {
			if (board[0] == -1) {
				for (int i = 0; i < 9; i ++) {
					if (i == 5 || i == 7 || i == 8) {
						valueBoard[i] -= 80;
					}
					else {
						valueBoard[i] += 80;
					}
				}
			}
			else if (board[2] == -1) {
				for (int i = 0; i < 9; i++) {
					if (i == 3 || i == 7 || i == 6) {
						valueBoard[i] -= 80;
					}
					else {
						valueBoard[i] += 80;
					}
				}
			}
			else if (board[6] == -1) {
				for (int i = 0; i < 9; i++) {
					if (i == 1 || i == 5 || i == 2) {
						valueBoard[i] -= 80;
					}
					else {
						valueBoard[i] += 80;
					}
				}
			}
			else if (board[8] == -1) {
				for (int i = 0; i < 9; i++) {
					if (i == 1 || i == 3 || i == 0) {
						valueBoard[i] -= 80;
					}
					else {
						valueBoard[i] += 80;
					}
				}
			}
			/*else if (board[5] == -1 || board[7] == -1) {
				valueBoard[0] += 10;
			}
			else if (board[1] == -1 || board[3] == -1) {
				valueBoard[0] += 10;
			}
			else if(board[2] == -1) {
				board[6] += 10;
			}
			else if (board[0] == -1) {
				board[9] += 10;
			}
			else if (board[0] == -1) {
				board[9] += 10;
			}*/
			else {

			}
		}
		// 二手目
		else if (judge == -2) {
			// リーチを防ぐ
			Scanning(board, -1, -2, valueBoard, 100, 0);
			// 自分のリーチを作る
			SubScanning(board, 1, valueBoard, 10, 0);
		}
		// 三手目
		else if (judge == -3) {
			Scanning(board, 1, 2, valueBoard, 100, 0);
			Scanning(board, -3, -2, valueBoard, 80, 0);
			SubScanning(board, 3, valueBoard, 5, 0);
			Scanning(board, -1, 1, valueBoard, 2, 0);
			SubScanning(board, -2, valueBoard, 3, 0);
		}
		// 四手目以降
		else if (judge == -4) {
			// 自分のリーチ100 相手のリーチ80
			Scanning(board, 3, 2, valueBoard, 100, 80);
			// 有利な盤の判断
			Scanning(board, -3, 2, valueBoard, 10, 0);
			Scanning(board, -1, 3, valueBoard, 5, 0);
			SubScanning(board, 3, valueBoard, 1, 1);
			SubScanning(board, 1, valueBoard, 1, 2);
			Scanning(board, 1, -1, valueBoard, 1, 0);
		}
	}
}

// 評価ボードから最適なマスを提案
// 一番点数の高い一手を提案する
void SuggestPos(int& pos, int (&valueBoard)[9], int* board) {

	for (int j = 0; j < 9; j++) {
		if (board[j] != 0) {
			valueBoard[j] = -10;
		}
	}

	int Max = 0;
	for (int i = 0; i < 9; i++) {
		if (Max < valueBoard[i]) {
			Max = valueBoard[i];
			pos = i; // 評価値が高いマス目をposに入れる
		}
	}

	// 有利な手がなければランダム
	if (Max == 0) {
		// ランダム
		std::random_device rnd;
		int random;
		do {
			random = rnd() % BOARD_SIZE;
		} while (board[random] != 0);
		pos = random; 
		/*do {
			std::cout << "有利な手がありません...どこに置きますか？ (1:左上 - 9:右下): ";
			std::cin >> pos;
		} while (pos < 1 || pos > 9 || board[pos - 1] != 0);
		pos -= 1;*/
	}
}

/// <summary>
/// 手を打つ
/// </summary>
/// <param name="board">盤の状態（値 1～3は自分駒，-1～-3は相手駒，次置くと1が消える）</param>
/// <returns>置く場所</returns>
int MyTurn(int* board) {

	int pos = 0;

	// ボードを評価し点数をつける
	int valueBoard[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	EvalBoard(board, valueBoard);

	// 評価ボードから最適なマスを提案する
	SuggestPos(pos, valueBoard, board);
	return pos;
}
