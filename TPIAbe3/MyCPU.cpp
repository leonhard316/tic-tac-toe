#include "TTTPlugin.h"
#include <random>
#include <iostream>
bool HEAD_OR_TAIL;


/// <summary>
/// �l�ԑ��삩�ǂ�����Ԃ�
/// </summary>
/// <returns>�l�ԂȂ�true, �@�B�Ȃ�false</returns>
bool IsHuman() {
	return false;
}

/// <summary>
/// ���O��Ԃ�
/// </summary>
/// <param name="buf">���O���i�[���镶����</param>
/// <param name="bufsize">������o�b�t�@�̃T�C�Y</param>
void GetName(char* buf, size_t bufsize) {
	sprintf_s(buf, bufsize, "Abe's CPU");
}

// 1���, 2���, 3���, 4��ڈȍ~����������֐�(�߂�l�@(���)1, 2, 3, 4  (���)-1, -2, -3, -4)
int Hand(int *board) {
	// �����̋�̐�
	int myHand = 0;
	// ����̋�̐�
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
		// ���̈���
		HEAD_OR_TAIL = true;
		return 1;
	}
	else if (myHand == 1 && yourHand == 1) {
		// ���̓���
		return 2;
	}
	else if (myHand == 2 && yourHand == 2) {
		// ���̎O���
		return 3;
	}

	else if (yourHand == 1 && myHand == 0) {
		// ���̈���
		HEAD_OR_TAIL = false;
		return -1;
	}
	else if (yourHand == 2 && myHand == 1) {
		// ���̓���
		return -2;
	}
	else if (yourHand == 3 && myHand == 2) {
		// ���̎O���
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

// �^�e�C���R�C�i�i���𑖍����āC�����ɂ����}�X�ɕ]���l��^����(a, b, 0: ���ڂ̒l �]���l:value1, value2) 
void Scanning(int* board, int a, int b, int (&valueBoard)[9], int value1, int value2) {
	// �^�e
	for (int col = 0; col < 3; col++) {

		// ����
		if ((board[col] == a && board[col + 3] == b && board[col + 6] == 0) || (board[col] == b && board[col + 3] == a && board[col + 6] == 0)) {
			valueBoard[col + 6] += value1;
			//std::cout << col << "���1" << std::endl;
		}
		if ((board[col] == a && board[col + 3] == 0 && board[col + 6] == b) || (board[col] == b && board[col + 3] == 0 && board[col + 6] == a)) {
			valueBoard[col + 3] += value1;
			//std::cout << col << "���2" << std::endl;
		}
		if ((board[col] == 0 && board[col + 3] == a && board[col + 6] == b) || (board[col] == 0 && board[col + 3] == b && board[col + 6] == a)) {
			valueBoard[col] += value1;
			//std::cout << col << "���3" << std::endl;
		}
		// ����
		if ((board[col] == -a && board[col + 3] == -b && board[col + 6] == 0) || (board[col] == -b && board[col + 3] == -a && board[col + 6] == 0)) {
			valueBoard[col + 6] += value2;
			//std::cout << "����:" << col << "���1" << std::endl;
		}
		if ((board[col] == -a && board[col + 3] == 0 && board[col + 6] == -b) || (board[col] == -b && board[col + 3] == 0 && board[col + 6] == -a)) {
			valueBoard[col + 3] += value2;
			//std::cout << "����:" << col << "���2" << std::endl;

		}
		if ((board[col] == 0 && board[col + 3] == -a && board[col + 6] == -b) || (board[col] == 0 && board[col + 3] == -b && board[col + 6] == -a)) {
			valueBoard[col] += value2;
			//std::cout << "����:" << col << "���3" << std::endl;
		}
	}

	// ���R
	for (int row = 0; row < 3; row++) {
		// ����
		if ((board[3 * row] == a && board[3 * row + 1] == b && board[3 * row + 1] == 0) || (board[3 * row] == b && board[3 * row + 1] == a && board[3 * row + 2] == 0)) {
			valueBoard[3 * row + 2] += value1;
			//std::cout << row << "�s��1" << std::endl;

		}
		if ((board[3 * row] == a && board[3 * row + 1] == 0 && board[3 * row + 2] == b) || (board[3 * row] == b && board[3 * row + 1] == 0 && board[3 * row + 2] == a)) {
			valueBoard[3 * row + 1] += value1;
			//std::cout << row << "�s��2" << std::endl;
		}
		if ((board[3 * row] == 0 && board[3 * row + 1] == a && board[3 * row + 2] == b) || (board[3 * row] == 0 && board[3 * row + 1] == b && board[3 * row + 2] == a)) {
			valueBoard[3 * row] += value1;
			//std::cout << row << "�s��3" << std::endl;
		}
		// ����
		if ((board[3 * row] == -a && board[3 * row + 1] == -b && board[3 * row + 2] == 0) || (board[3 * row] == -b && board[3 * row + 1] == -a && board[3 * row + 2] == 0)) {
			valueBoard[3 * row + 2] += value2;
			//std::cout << "����:" << row << "�s��1" << std::endl;
		}
		if ((board[3 * row] == -a && board[3 * row + 1] == 0 && board[3 * row + 2] == -b) || (board[3 * row] == -b && board[3 * row + 1] == 0 && board[3 * row + 2] == -a)) {
			valueBoard[3 * row + 1] += value2;
			//std::cout << "����:" << row << "�s��2" << std::endl;
		}
		if ((board[3 * row] == 0 && board[3 * row + 1] == -a && board[3 * row + 2] == -b) || (board[3 * row] == 0 && board[3 * row + 1] == -b && board[3 * row + 2] == -a)) {
			valueBoard[3 * row] += value2;
			//std::cout << "����:" << row << "�s��3" << std::endl;
		}
	}

	// �i�i��
	// ����
	if ((board[0] == a && board[4] == b && board[8] == 0) || (board[0] == b && board[4] == a && board[8] == 0)) {
		valueBoard[8] += value1;
		//std::cout << "�i�i��1" << std::endl;
	}
	if ((board[0] == a && board[4] == 0 && board[8] == b) || (board[0] == b && board[4] == 0 && board[8] == a)) {
		valueBoard[4] += value1;
		//std::cout << "�i�i��2" << std::endl;
	}
	if ((board[0] == 0 && board[4] == a && board[8] == b) || (board[0] == 0 && board[4] == b && board[8] == a)) {
		valueBoard[0] += value1;
		//std::cout << "�i�i��3" << std::endl;
	}
	if ((board[2] == a && board[4] == b && board[6] == 0) || (board[2] == b && board[4] == a && board[6] == 0)) {
		valueBoard[6] += value1;
		//std::cout << "�i�i��4" << std::endl;
	}
	if ((board[2] == a && board[4] == 0 && board[6] == b) || (board[2] == b && board[4] == 0 && board[6] == a)) {
		valueBoard[4] += value1;
		//std::cout << "�i�i��5" << std::endl;
	}
	if ((board[2] == 0 && board[4] == a && board[6] == b) || (board[2] == 0 && board[4] == b && board[6] == a)) {
		valueBoard[2] += value1;
		//std::cout << "�i�i��6" << std::endl;
	}

	// ����
	if ((board[0] == -a && board[4] == -b && board[8] == 0) || (board[0] == -b && board[4] == -a && board[8] == 0)) {
		valueBoard[8] += value2;
		//std::cout << "����:" << "�i�i��-1" << std::endl;
	}
	if ((board[0] == -a && board[4] == 0 && board[8] == -b) || (board[0] == -b && board[4] == 0 && board[8] == -a)) {
		valueBoard[4] += value2;
		//std::cout << "����:" << "�i�i��-2" << std::endl;
	}
	if ((board[0] == 0 && board[4] == -a && board[8] == -b) || (board[0] == 0 && board[4] == -b && board[8] == -a)) {
		valueBoard[0] += value2;
		//std::cout << "����:" << "�i�i��-3" << std::endl;
	}
	if ((board[2] == -a && board[4] == -b && board[6] == 0) || (board[2] == -b && board[4] == -a && board[6] == 0)) {
		valueBoard[6] += value2;
		//std::cout << "����:" << "�i�i��-4" << std::endl;
	}
	if ((board[2] == -a && board[4] == 0 && board[6] == -b) || (board[2] == -b && board[4] == 0 && board[6] == -a)) {
		valueBoard[4] += value2;
		//std::cout << "����:" << "�i�i��-5" << std::endl;
	}
	if ((board[2] == 0 && board[4] == -a && board[6] == -b) || (board[2] == 0 && board[4] == -b && board[6] == -a)) {
		valueBoard[2] += value2;
		//std::cout << "����:" << "�i�i��-6" << std::endl;
	}
}

// �^�e�C���R�C�i�i���𑖍����āC�����ɂ����}�X�ɕ]���l��^����(a, 0, 0: ���ڂ̒l �]���l:value1, value2) 
void SubScanning(int* board, int a, int (&valueBoard)[9], int value1, int value2) {
	// �^�e
	for (int col = 0; col < 3; col++) {

		// ����
		if (board[col] == a && board[col + 3] == 0 && board[col + 6] == 0) {
			valueBoard[col + 3] += value1;
			valueBoard[col + 6] += value1;
			//std::cout << col << "���1" << std::endl;
		}
		if (board[col] == 0 && board[col + 3] == a && board[col + 6] == 0) {
			valueBoard[col] += value1;
			valueBoard[col + 6] += value1;
			//std::cout << col << "���2" << std::endl;
		}
		if (board[col] == 0 && board[col + 3] == 0 && board[col + 6] == a) {
			valueBoard[col] += value1;
			valueBoard[col + 3] += value1;
			//std::cout << col << "���3" << std::endl;
		}
		// ����
		if (board[col] == -a && board[col + 3] == 0 && board[col + 6] == 0) {
			valueBoard[col + 3] += value2;
			valueBoard[col + 6] += value2;
			//std::cout << "����:" << col << "���1" << std::endl;
		}
		if (board[col] == 0 && board[col + 3] == -a && board[col + 6] == 0) {
			valueBoard[col] += value2;
			valueBoard[col + 6] += value2;
			//std::cout << "����:" << col << "���2" << std::endl;
		}
		if (board[col] == 0 && board[col + 3] == 0 && board[col + 6] == -a) {
			valueBoard[col] += value2;
			valueBoard[col + 3] += value2;
			//std::cout << "����:" << col << "���3" << std::endl;
		}
	}

	// ���R
	for (int row = 0; row < 3; row++) {
		// ����
		if (board[3 * row] == a && board[3 * row + 1] == 0 && board[3 * row + 2] == 0) {
			valueBoard[3 * row + 1] += value1;
			valueBoard[3 * row + 2] += value1;
			//std::cout << row << "�s��1" << std::endl;
		}
		if (board[3 * row] == 0 && board[3 * row + 1] == a && board[3 * row + 2] == 0) {
			valueBoard[3 * row] += value1;
			valueBoard[3 * row + 2] += value1;
			//std::cout << row << "�s��2" << std::endl;
		}
		if (board[3 * row] == 0 && board[3 * row + 1] == 0 && board[3 * row + 2] == a) {
			valueBoard[3 * row] += value1;
			valueBoard[3 * row + 1] += value1;
			//std::cout << row << "�s��3" << std::endl;
		}
		// ����
		if (board[3 * row] == -a && board[3 * row + 1] == 0 && board[3 * row + 2] == 0) {
			valueBoard[3 * row + 1] += value2;
			valueBoard[3 * row + 2] += value2;
			//std::cout << "����:" << row << "�s��1" << std::endl;
		}
		if (board[3 * row] == 0 && board[3 * row + 1] == -a && board[3 * row + 2] == 0) {
			valueBoard[3 * row] += value2;
			valueBoard[3 * row + 2] += value2;
			//std::cout << "����:" << row << "�s��2" << std::endl;
		}
		if (board[3 * row] == 0 && board[3 * row + 1] == 0 && board[3 * row + 2] == -a) {
			valueBoard[3 * row] += value2;
			valueBoard[3 * row + 1] += value2;
			//std::cout << "����:" << row << "�s��3" << std::endl;
		}
		
	}

	// �i�i��
	// ����
	if (board[0] == a && board[4] == 0 && board[8] == 0) {
		valueBoard[4] += value1;
		valueBoard[8] += value1;
		//std::cout << "�i�i��1" << std::endl;
	}
	if (board[0] == 0 && board[4] == 0 && board[8] == a) {
		valueBoard[0] += value1;
		valueBoard[4] += value1;
		//std::cout << "�i�i��2" << std::endl;
	}
	if (board[0] == 0 && board[4] == a && board[8] == 0) {
		valueBoard[0] += value1;
		valueBoard[8] += value1;
		//std::cout << "�i�i��3" << std::endl;
	}

	if (board[2] == a && board[4] == 0 && board[6] == 0) {
		valueBoard[4] += value1;
		valueBoard[6] += value1;
		//std::cout << "�i�i��4" << std::endl;
	}
	if (board[2] == 0 && board[4] == 0 && board[6] == a) {
		valueBoard[2] += value1;
		valueBoard[4] += value1;
		//std::cout << "�i�i��5" << std::endl;
	}
	if (board[2] == 0 && board[4] == a && board[6] == 0) {
		valueBoard[2] += value1;
		valueBoard[6] += value1;
		//std::cout << "�i�i��6" << std::endl;
	}

	// ����
	if (board[0] == -a && board[4] == 0 && board[8] == 0) {
		valueBoard[4] += value2;
		valueBoard[8] += value2;
		//std::cout << "����:" << "�i�i��-1" << std::endl;
	}
	if (board[0] == 0 && board[4] == 0 && board[8] == -a) {
		valueBoard[0] += value2;
		valueBoard[4] += value2;
		//std::cout << "����:" << "�i�i��-2" << std::endl;
	}
	if (board[0] == 0 && board[4] == -a && board[8] == 0) {
		valueBoard[0] += value2;
		valueBoard[8] += value2;
		//std::cout << "����:" << "�i�i��-3" << std::endl;
	}
	if (board[2] == -a && board[4] == 0 && board[6] == 0) {
		valueBoard[4] += value2;
		valueBoard[6] += value2;
		//std::cout << "����:" << "�i�i��-4" << std::endl;
	}
	if (board[2] == 0 && board[4] == 0 && board[6] == -a) {
		valueBoard[2] += value2;
		valueBoard[4] += value2;
		//std::cout << "����:" << "�i�i��-5" << std::endl;
	}
	if (board[2] == 0 && board[4] == -a && board[6] == 0) {
		valueBoard[2] += value2;
		valueBoard[6] += value2;
		//std::cout << "����:" << "�i�i��-6" << std::endl;
	}
}

// �]���֐�
void EvalBoard(int *board, int (&valueBoard)[9]) {
	int judge = Hand(board);

	// �����_���v�f
	/*std::random_device rnd;
	int random1, random2;*/

	// ���------------------------------------
	if (HEAD_OR_TAIL == true) {
		// ����
		if (judge == 1) {
			valueBoard[0] += 10;
			//std::cout << "����" << std::endl;
		}
		// ����
		else if (judge == 2) {
			if (board[5] == -1) {
				valueBoard[2] += 100;
			}
			else if (board[7] == -1) {
				valueBoard[6] += 100;
			}
			// �����̃��[�`�����
			SubScanning(board, 1, valueBoard, 2, 1);
			//std::cout << "����" << std::endl;
		}
		// �O���
		else if (judge == 3) {
			/*int position;
			do {
				std::cout << "�O��ڂ͂ǂ��ɒu���܂����H (1:���� - 9:�E��): ";
				std::cin >> position;
			} while (position < 1 || position > 9 || board[position - 1] != 0);*/

			// ���[�`�ȊO�̗L���Ȕ�
			SubScanning(board, 2, valueBoard, 10, 5);
			// ���[�`
			Scanning(board, 1, 2, valueBoard, 100, 80);
			//std::cout << "�O���" << std::endl;
		}
		// �l��ڈȍ~
		else if (judge == 4) {
			// ���[�`�ȊO�̗L���Ȕ�
			Scanning(board,  -1, 3, valueBoard, 8, 7);
			Scanning(board, -3, 2, valueBoard, 3, 0);
			Scanning(board, 1, -1, valueBoard, 1, 0);
			SubScanning(board, 3, valueBoard, 1, 1);
			// ���[�`
			Scanning(board, 3, 2, valueBoard, 100, 80);
			//std::cout << "�l��ڈȍ~" << std::endl;
		}
	}
	 // ���------------------------------------
	else if (HEAD_OR_TAIL == false) {
		// ����
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
		// ����
		else if (judge == -2) {
			// ���[�`��h��
			Scanning(board, -1, -2, valueBoard, 100, 0);
			// �����̃��[�`�����
			SubScanning(board, 1, valueBoard, 10, 0);
		}
		// �O���
		else if (judge == -3) {
			Scanning(board, 1, 2, valueBoard, 100, 0);
			Scanning(board, -3, -2, valueBoard, 80, 0);
			SubScanning(board, 3, valueBoard, 5, 0);
			Scanning(board, -1, 1, valueBoard, 2, 0);
			SubScanning(board, -2, valueBoard, 3, 0);
		}
		// �l��ڈȍ~
		else if (judge == -4) {
			// �����̃��[�`100 ����̃��[�`80
			Scanning(board, 3, 2, valueBoard, 100, 80);
			// �L���ȔՂ̔��f
			Scanning(board, -3, 2, valueBoard, 10, 0);
			Scanning(board, -1, 3, valueBoard, 5, 0);
			SubScanning(board, 3, valueBoard, 1, 1);
			SubScanning(board, 1, valueBoard, 1, 2);
			Scanning(board, 1, -1, valueBoard, 1, 0);
		}
	}
}

// �]���{�[�h����œK�ȃ}�X����
// ��ԓ_���̍��������Ă���
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
			pos = i; // �]���l�������}�X�ڂ�pos�ɓ����
		}
	}

	// �L���Ȏ肪�Ȃ���΃����_��
	if (Max == 0) {
		// �����_��
		std::random_device rnd;
		int random;
		do {
			random = rnd() % BOARD_SIZE;
		} while (board[random] != 0);
		pos = random; 
		/*do {
			std::cout << "�L���Ȏ肪����܂���...�ǂ��ɒu���܂����H (1:���� - 9:�E��): ";
			std::cin >> pos;
		} while (pos < 1 || pos > 9 || board[pos - 1] != 0);
		pos -= 1;*/
	}
}

/// <summary>
/// ���ł�
/// </summary>
/// <param name="board">�Ղ̏�ԁi�l 1�`3�͎�����C-1�`-3�͑����C���u����1��������j</param>
/// <returns>�u���ꏊ</returns>
int MyTurn(int* board) {

	int pos = 0;

	// �{�[�h��]�����_��������
	int valueBoard[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	EvalBoard(board, valueBoard);

	// �]���{�[�h����œK�ȃ}�X���Ă���
	SuggestPos(pos, valueBoard, board);
	return pos;
}
