#include "TTTPlugin.h"
#include <iostream>

/// <summary>
/// 人間操作かどうかを返す
/// </summary>
/// <returns>人間ならtrue, 機械ならfalse</returns>
bool IsHuman() {
	return true;
}

/// <summary>
/// 名前を返す
/// </summary>
/// <param name="buf">名前を格納する文字列</param>
/// <param name="bufsize">文字列バッファのサイズ</param>
void GetName(char* buf, size_t bufsize) {
	sprintf_s(buf, bufsize, "人間");
}

/// <summary>
/// 手を打つ
/// </summary>
/// <param name="board">盤の状態（値 1～3は自分駒，-1～-3は相手駒，次置くと1が消える）</param>
/// <returns>置く場所</returns>
int MyTurn(int* board) {

	int pos = 0;
	do {
		std::cout << "どこに置きますか？ (1:左上 - 9:右下): ";
		std::cin >> pos;
	} while (pos < 1 || pos > 9 || board[pos - 1] != 0);

	return pos - 1;
}
