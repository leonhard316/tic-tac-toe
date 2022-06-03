using System;
using System.Collections.Generic;
using TTT;

namespace TTTConsole {

    class ConsolePlay {

        private readonly string[] PLAYER_STR = new string[] { "", "先手", "後手" };
        private readonly string[] PLAYER_MARK = new string[] { "　", "○", "×" };

        private TicTacToe _ttt = new TicTacToe();
        private List<Plugin> _plugins = new List<Plugin>();
        private List<Plugin> _player = new List<Plugin>();
        static bool _showNumber = false;

        /// <summary>
        /// 実行
        /// </summary>
        /// <param name="files"></param>
        public void Run(string[] pluginFiles) {
            ShowTitle();
            if (!LoadPlugin(pluginFiles)) {
                Console.WriteLine("プラグインがありません．");
                return;
            }
            if (!SelectPlayer()) {
                Console.WriteLine("中断します．");
                return;
            }
            Battle();
            ShowResult();
        }
        private void ShowTitle()
        {
            Console.WriteLine(new string('*', 30));
            Console.WriteLine("拡張○×ゲーム");
            Console.WriteLine(new string('*', 30));
            Console.WriteLine("");
        }

        /// <summary>
        /// プラグインを読み込む
        /// </summary>
        /// <param name="files"></param>
        private bool LoadPlugin(string[] files) {
            foreach (var file in files) {
                var plugin = new Plugin();
                if (!plugin.Load(file)) continue;
                _plugins.Add(plugin);
            }
            return _plugins.Count > 0;
        }

        /// <summary>
        /// プレイヤーの選択
        /// </summary>
        private bool SelectPlayer() {
            Console.WriteLine("プレイヤーの選択");
            for (var pl = 0; pl < _ttt.PLAYERS; pl++) {
                for (var i = 0; i < _plugins.Count; i++) {
                    Console.WriteLine($"({i + 1}) {_plugins[i].Name}");
                }
                Console.Write($"{PLAYER_STR[pl + 1]} を選んでください: ");
                var input = Console.ReadLine();

                int selID;
                if (!int.TryParse(input, out selID)) return false;
                if (selID < 1 || selID > _plugins.Count) return false;
                _player.Add(_plugins[selID - 1]);
                //Console.WriteLine(_player[pl]);
            }

            // 数字表示の選択
            Console.Write("○×の順番を表示しますか？ 1:しない 2:する ");
            _showNumber = int.Parse(Console.ReadLine()) == 2 ? true : false;

            return true;
        }

        /// <summary>
        /// 対戦
        /// </summary>
        private void Battle() {
            _ttt.Init();
            do {
                Console.WriteLine("");
                ShowBoard();
                Console.Write($"{PlayerStr()} の番");

                if (_player[(int)_ttt.Player - 1].IsHuman) Console.WriteLine("");
                _ttt.Set(_player[(int)_ttt.Player - 1].MyTurn(_ttt.GetBoard()));
                if (!_player[(int)_ttt.Player - 1].IsHuman) Console.WriteLine($" --> {_ttt.LastSet + 1}");

            } while (_ttt.Judge == JUDGE.None);
        }

        /// <summary>
        /// 結果表示
        /// </summary>
        private void ShowResult() {
            string msg = "";
            switch (_ttt.Judge) {
            case JUDGE.WIN:
                msg = $"{PlayerStr()} の勝利";
                break;
            case JUDGE.DRAW:
                msg = $"引き分け";
                break;
            case JUDGE.OUT_OF_RANGE:
                msg = $"{PlayerStr()} の反則負け（範囲外）";
                break;
            case JUDGE.OVERLAP:
                msg = $"{PlayerStr()} の反則負け（重ね置き）";
                break;
            }

            Console.WriteLine("");
            Console.WriteLine(new string('*', 30));
            ShowBoard();
            Console.WriteLine("");
            Console.WriteLine(msg);
            Console.WriteLine(new string('*', 30));
        }

        /// <summary>
        /// 盤の表示
        /// </summary>
        private void ShowBoard() {
            Console.WriteLine(new string('~', 6) + $" ターン{_ttt.Turn} " + new string('~', 6));

            var flip = _ttt.Player == PLAYER.Second ? -1 : 1;
            for (var row = 0; row < _ttt.BOARD_ROWS; row++) {
                Console.Write("    ");
                for (var col = 0; col < _ttt.BOARD_COLS; col++) {
                    var pos = row * _ttt.BOARD_COLS + col;
                    var pIdx = _ttt.Board[pos] * flip > 0 ? PLAYER.First :
                        _ttt.Board[pos] * flip < 0 ? PLAYER.Second : PLAYER.None;
                    Console.Write(PLAYER_MARK[(int)pIdx]);
                    if (_showNumber) Console.Write(_ttt.Board[pos] == 0 ? "  " :
                        string.Format("{0,2}", Math.Abs(_ttt.Board[pos])));
                    if (col < _ttt.BOARD_COLS - 1) Console.Write(" | ");
                }
                Console.WriteLine("");
                if (row < _ttt.BOARD_ROWS - 1) {
                    if (_showNumber) Console.WriteLine("    -----+------+-----");
                    else Console.WriteLine("    ---+----+---");
                }
            }
        }

        /// <summary>
        /// プレイヤーの表示文字列を生成
        /// </summary>
        /// <returns>表示文字列</returns>
        private string PlayerStr() {
            return $"{PLAYER_STR[(int)_ttt.Player]}（{_player[(int)_ttt.Player - 1].Name}）";
        }
    }
}
