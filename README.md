# TicTacToe
拡張○×ゲーム　プラグイン版

## TTTConsole プロジェクト
- ゲーム進行フレームワーク
- C#実装
- プラットフォームを 'x64' とすること．'Any' はNG
### ConsolePlay.cs
- コンソールで入出力するUI実装

## TTTGUI プロジェクト
- ゲーム進行フレームワーク
- C#実装
- プラットフォームを 'x64' とすること．'Any' はNG
### GUIPlay.cs
- GUIで入出力するUI実装

### Program.cs
- Main関数（プログラム開始点）
### Plugin.cs
- 思考プラグインDLLを扱うクラス
- プラグイン名，人間操作有無を読み込む
- 思考関数ポインタを取得する
### TicTacToe.cs
- 拡張○×ゲームの進行管理

## プラグインプロジェクト TPIHUman, TPIRandom, TPIAbe
- 思考プラグイン
- C++実装
- プラットフォームを 'x64' とすること．
- ビルド後イベントに，コピーコマンドを記述
### 共通ヘッダーファイル TTTPlugin.h
- GetName, IsHuman, MyTurn 関数の定義
### 実装ファイル Human.cpp, Random.cpp, MyCPU.cpp
- 関数の実装
- void GetName(char* buf, size_t bufsize)
  - 引数の文字列バッファ buf にプラグイン名を代入する
- bool IsHuman()
  - プラグインが人間操作かどうかを返す
- int MyTurn(int* board)
  - 思考関数
  - 引数 board : 盤の状態（値 1～3は自分駒，-1～-3は相手駒，次置くと1が消える）
  - 戻り値 : 置く場所 (0-8)

- MyCPU.cpp
 - 関数の実装
 　- int Hand(int *board)
 　 - 先手後手と何手目かを見極める関数
 　 - 引数 board : 盤の状態（値 1～3は自分駒，-1～-3は相手駒，次置くと1が消える）
 　 - 戻り値 : 先手後手どちらなのかと何手目かの情報
   - void Scanning(int* board, int a, int b, int (&valueBoard)[9], int value1, int value2), void SubScanning(int* board, int a, int (&valueBoard)[9], int value1, int value2)
    - 縦,横,ナナメをそれぞれ走査する関数
    - 引数 board : 盤の状態（値 1～3は自分駒，-1～-3は相手駒，次置くと1が消える）
    - 引数 int a, int b 何マス目かの情報
    - 引数 valueBoard[9] それぞれのマスの評価値を格納する配列
    - 引数 int value1, int value2 aマス目とbマス目にそれぞれ与える評価値
   - int EvalBoard(int *board, int (&valueBoard)[9])
     - 引数 board : 盤の状態（値 1～3は自分駒，-1～-3は相手駒，次置くと1が消える）
     - 引数 valueBoard[9] それぞれのマスの評価値を格納する配列
   - void SuggestPos(int& pos, int (&valueBoard)[9], int* board)
     - 評価値が高いマスを次の手として提案する関数
     - 引数 int pos マスを置く場所
     - 引数 board : 盤の状態（値 1～3は自分駒，-1～-3は相手駒，次置くと1が消える）
     - 引数 valueBoard[9] それぞれのマスの評価値を格納する配列
     
### 開発環境
- Visual C#/C++ 2019
