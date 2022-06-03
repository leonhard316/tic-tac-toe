#pragma once

const int BOARD_SIZE = 9;

extern "C"
{
	// 既定の関数
	__declspec(dllexport) bool IsHuman();
	__declspec(dllexport) void GetName(char* buf, size_t bufsize);
	__declspec(dllexport) int MyTurn(int* board);
}
