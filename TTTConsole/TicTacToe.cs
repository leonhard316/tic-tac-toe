using System;
using System.Runtime.InteropServices;

namespace TTT {

    public enum PLAYER { None = 0, First = 1, Second = 2 };
    public enum JUDGE { None, WIN, DRAW, OUT_OF_RANGE, OVERLAP }

    /// <summary>
    /// TicTacToeクラス
    /// ゲーム進行のみ管理．UIは関与しない．
    /// </summary>
    class TicTacToe {

        public readonly int BOARD_ROWS = 3;         // マスの行数
        public readonly int BOARD_COLS = 3;         // マスの列数
        public readonly int BOARD_SIZE = 9;         // マスの総数
        public readonly int REMAIN_PIECES = 3;      // 盤に残る駒の数
        public readonly int PLAYERS = 2;            // プレイヤー数
        public readonly int TURN_LIMIT;             // ターン数上限（このターン数に至ると引き分け）
        public readonly int NONE = 0;               // 空のマス
        public readonly int[,] LINES;               // 並べる線の情報

        public int[] Board { get; private set; }    // 盤 0:空 正:先手駒 負:後手駒
        public int Turn { get; private set; }       // ターン数
        public PLAYER Player { get; private set; }  // 手を挿すプレイヤー 1:先手 2:後手
        public JUDGE Judge { get; private set; }    // ゲーム終了の判定
        public int LastSet { get; private set; }    // 直前に置いた場所
        private IntPtr _board;                      // アンマネージドメモリの盤データ

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="turnLimit">ターン数上限</param>
        public TicTacToe(int turnLimit = 100) {
            TURN_LIMIT = turnLimit;
            Board = new int[BOARD_SIZE];
            LINES = new int[8, 3]{ { 0, 1, 2 }, { 3, 4, 5 }, { 6, 7, 8 },
                { 0, 3, 6 }, { 1, 4, 7 }, { 2, 5, 8 }, { 0, 4, 8 }, { 2, 4, 6 } };
            _board = Marshal.AllocCoTaskMem(Marshal.SizeOf(typeof(int)) * BOARD_SIZE);
        }

        /// <summary>
        /// デストラクタ
        /// </summary>
        ~TicTacToe() {
            Marshal.FreeCoTaskMem(_board);
        }

        /// <summary>
        /// ゲームの初期化 
        /// </summary>
        public void Init() {
            for (var i = 0; i < BOARD_SIZE; i++) Board[i] = NONE;
            Turn = 1;
            Player = PLAYER.First;
            Judge = JUDGE.None;
        }

        /// <summary>
        /// 駒を置き，結果を判定
        /// </summary>
        /// <param name="pos">置く場所</param>
        /// <returns>判定結果</returns>        
        public void Set(int pos) {
            // 値チェック
            LastSet = pos;
            if (pos < 0 || pos >= BOARD_SIZE) {
                Judge = JUDGE.OUT_OF_RANGE;
                return;
            }

            // 反則（重ね置き）チェック
            if (Board[pos] != 0) {
                Judge = JUDGE.OVERLAP;
                return;
            }

            // 盤の更新
            if (Turn > REMAIN_PIECES) {
                for (var i = 0; i < BOARD_SIZE; i++) if (Board[i] > 0) --Board[i];
                Board[pos] = REMAIN_PIECES;
            } else {
                Board[pos] = Turn;
            }

            // 勝利判定
            for (var i = 0; i < LINES.GetLength(0); i++) {
                if (Board[LINES[i, 0]] > 0 && Board[LINES[i, 1]] > 0 && Board[LINES[i, 2]] > 0) {
                    Judge = JUDGE.WIN;
                    return;
                }
            }

            // 次へ
            if (Player == PLAYER.First) {
                Player = PLAYER.Second;
            } else {
                Player = PLAYER.First;
                if (++Turn >= TURN_LIMIT) {
                    Judge = JUDGE.DRAW;
                    return;
                }
            }
            FlipBoard();
            return;
        }

        /// <summary>
        /// 盤の値を反転
        /// </summary>
        private void FlipBoard() {
            for (var i = 0; i < BOARD_SIZE; i++) {
                Board[i] = -Board[i];
            }
        }

        /// <summary>
        /// アンマネージドメモリの盤データを取得
        /// </summary>
        /// <returns></returns>
        public IntPtr GetBoard() {
            Marshal.Copy(Board, 0, _board, BOARD_SIZE);
            return _board;
        }
    }
}
