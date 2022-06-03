using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using TTT;
using System.IO;
using System.Collections;

namespace TTTGUI
{

    public partial class GUIPlay : Form
    {

        private readonly string[] PLAYER_STR = new string[] { "", "先手", "後手" };
        //private readonly string[] PLAYER_MARK = new string[] { "　", "○", "×" };
        private readonly int[] PLAYER_MARK = new int[] { 0, 1, 2 };

        // STOPボタン
        bool STOP = false;

        private TicTacToe _ttt = new TicTacToe();
        private List<Plugin> _plugins = new List<Plugin>();
        private List<Plugin> _player = new List<Plugin>();
        static bool _showNumber = false;
        int battleCounter = 0; // 総対戦数
        int winCountPlayer1 = 0; // Player1の勝ち回数
        int winCountPlayer2 = 0; // Player2の勝ち回数

        // 3ターン目の盤の情報
        int [] boardData = new int[9];

        // Setting:親フォーム GUI:子フォーム → プロパティ変更後は親子関係を逆転
        public int mode = 2; // 表示速度切り替え 0:高速　1:ふつう　2:ゆっくり 
        public int limit = 0; // 対戦回数の上限 0:3回まで　1:10回まで　2:50回まで 3:無限 (AIの時だけ)
        public string modedisplay;

        // 待ち時間
        int waitTime1 = 500; // デフォルト
        int waitTime2 = 100; // デフォルト
        int waitTime3 = 50; // デフォルト

        /// <summary>
        /// プラグインを読み込む
        /// </summary>
        /// <param name="files"></param>
        private bool LoadPlugin(string[] files)
        {
            foreach (var file in files)
            {
                var plugin = new Plugin();
                if (!plugin.Load(file)) continue;
                _plugins.Add(plugin);
            }
            return _plugins.Count > 0;
        }

        // ゲームBGMを流す
        private System.Media.SoundPlayer player = null;
        bool bgmSwitch = false;
        string SoundFile = "D:/ss/SS08/TicTacToe/TTTGUI/BGM.wav";

        private void StopSound()
        {
            if (player != null)
            {
                player.Stop();
                player.Dispose();
                player = null;
            }
        }

        private void PlaySound()
        {
            player = new System.Media.SoundPlayer(SoundFile);
            player.Play();
        }

        /// <summary>
        /// 対戦
        /// </summary>
        private void Battle()
        {
            if (_ttt.Judge != JUDGE.None)
            {
                ShowResult();
                for (int i = 0; i < 9; i++)
                {
                    this.Controls[Button(i + 1)].Enabled = false;
                }
                button10.Enabled = true;
            }
            else
            {
                // 人間がいるとき---------------------------------------------------------------------------------------
                if (_player[0].IsHuman == true || _player[1].IsHuman == true)
                {
                    HumanBattle();
                }
                // 人間がいないとき-------------------------------------------------------------------------------------------
                else
                {
                    do
                    {
                        // 選択したマスを取得
                        textBox1.Refresh();
                        textBox1.Text = $"{PlayerStr()} の番です。";
                        textBox1.Update();
                        System.Threading.Thread.Sleep(waitTime1);
                        _ttt.Set(_player[(int)_ttt.Player - 1].MyTurn(_ttt.GetBoard()));
                        ShowBoard();
                        System.Threading.Thread.Sleep(waitTime2);
                    } while (_ttt.Judge == JUDGE.None);
                    ShowResult();
                }
                //--------------------------------------------------------------------------------------------------------------
            }
        }

        // 人間がいるときのバトル
        private void HumanBattle()
        {
            switch (_player[(int)_ttt.Player - 1].IsHuman)
            { 
                // 人間
                case true:
                    textBox1.Refresh();
                    textBox1.Text = $"{PlayerStr()} の番です。";
                    textBox1.Update();
                    System.Threading.Thread.Sleep(waitTime1);
                    textBox1.Text = "マスをクリックしてください。";
                    textBox1.Update();
                    break;
                // 人間以外
                case false:
                    textBox1.Refresh();
                    textBox1.Text = $"{PlayerStr()} の番です。";
                    textBox1.Update();
                    System.Threading.Thread.Sleep(waitTime1);
                    _ttt.Set(_player[(int)_ttt.Player - 1].MyTurn(_ttt.GetBoard()));
                    ShowBoard();
                    break;
            }
        }

        string msg = "";

        /// <summary>
        /// 結果表示
        /// </summary>
        private void ShowResult()
        {
            switch (_ttt.Judge)
            {
                case JUDGE.WIN:
                    msg = $"{PlayerStr()} の勝利";

                    //int winCountPlayer1 = 0; // Player1の勝ち回数
                    //int winCountPlayer2 = 0; // Player2の勝ち回数
                    //int battleCounter = 0; // 総対戦数
                    switch ((int)_ttt.Player) 
                    {
                        case 1:
                            winCountPlayer1 += 1;
                            break;
                        case 2:
                            winCountPlayer2 += 1;
                            break;
                    }
                    break;
                case JUDGE.DRAW:
                    msg = $"引き分け";
                    break;
                case JUDGE.OUT_OF_RANGE:
                    msg = $"{PlayerStr()} の反則負け（範囲外）";
                    switch ((int)_ttt.Player)
                    {
                        case 1:
                            winCountPlayer2 += 1;
                            break;
                        case 2:
                            winCountPlayer1 += 1;
                            break;
                    }
                    break;
                case JUDGE.OVERLAP:
                    msg = $"{PlayerStr()} の反則負け（重ね置き）";
                    switch ((int)_ttt.Player)
                    {
                        case 1:
                            winCountPlayer2 += 1;
                            break;
                        case 2:
                            winCountPlayer1 += 1;
                            break;
                    }
                    break;
            }
            button10.Enabled = true;
            ShowBoard();
            textBox1.Text = msg;
        }

        /// <summary>
        /// 盤の更新
        /// </summary>
        private void ShowBoard()
        {
            // ターン表示(リフレッシュして強制的に再描画)
            label13.Refresh();
            Console.Write(_ttt.Turn);
            label13.Text = $" ターン{_ttt.Turn} ";
            label13.Update();
            //System.Threading.Thread.Sleep(800);

            // 3ターン目の盤の状態を保存
            if (_ttt.Turn == 3)
            {
                for (int i = 0; i < 9; i ++)
                {
                    // 空:0 先手駒:正 後手駒:負
                    boardData[i] = _ttt.Board[i];
                }
            }

            // PLAYER.First(先手) PLAYER.Second(後手)
            var flip = _ttt.Player == PLAYER.Second ? -1 : 1;
           
            for (var row = 0; row < _ttt.BOARD_ROWS; row++) // マスの行を移動
            {
                
                for (var col = 0; col < _ttt.BOARD_COLS; col++)
                {
                    var pos = row * _ttt.BOARD_COLS + col;
                    var pIdx = _ttt.Board[pos] * flip > 0 ? PLAYER.First :
                        _ttt.Board[pos] * flip < 0 ? PLAYER.Second : PLAYER.None;

                    
                    // 〇×と順番の表示
                    if (_showNumber) {
                        
                        this.Controls[Label(row * 3 + col + 1)].ForeColor = Color.White;
                       
                        //this.Controls[Button(row * 3 + col + 1)].ForeColor = Color.White;
                        this.Controls[Label(row * 3 + col + 1)].Refresh();
                        switch (PLAYER_MARK[(int)pIdx])
                        {
                            case 0:
                                this.Controls[Label(row * 3 + col + 1)].Text = _ttt.Board[pos] == 0 ? "  " :
                        string.Format("{0,2}", Math.Abs(_ttt.Board[pos]));
                                this.Controls[Button(row * 3 + col + 1)].BackgroundImage = null;
                                this.Controls[Button(row * 3 + col + 1)].Update();
                                this.Controls[Label(row * 3 + col + 1)].Update();
                                System.Threading.Thread.Sleep(waitTime3);
                                break;
                            case 1:
                                this.Controls[Label(row * 3 + col + 1)].Text = _ttt.Board[pos] == 0 ? "  " :
                        string.Format("{0,2}", Math.Abs(_ttt.Board[pos]));
                                this.Controls[Button(row * 3 + col + 1)].BackgroundImage = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\素材\マル.png");
                                this.Controls[Button(row * 3 + col + 1)].Update();
                                this.Controls[Label(row * 3 + col + 1)].Update();
                                System.Threading.Thread.Sleep(waitTime3);
                                break;
                            case 2:
                                this.Controls[Label(row * 3 + col + 1)].Text = _ttt.Board[pos] == 0 ? "  " :
                        string.Format("{0,2}", Math.Abs(_ttt.Board[pos]));
                                this.Controls[Button(row * 3 + col + 1)].BackgroundImage = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\素材\バツ.png");
                                this.Controls[Button(row * 3 + col + 1)].Update();
                                this.Controls[Label(row * 3 + col + 1)].Update();
                                System.Threading.Thread.Sleep(waitTime3);
                                break;
                        }

                        //this.Controls[Label(row * 3 + col + 1)].Update();
                       // System.Threading.Thread.Sleep(10);
                        //Button(row * 3 + col).Refresh();
                    }
                    else
                    {
                        this.Controls[Label(row * 3 + col + 1)].ForeColor = Color.White;
                        //textBox1.Refresh();
                        //textBox1.Text = $"{PlayerStr()} の番です。";
                        //textBox1.Update();
                        // this.Controls[Button(row * 3 + col + 1)].ForeColor = Color.White;
                        this.Controls[Label(row * 3 + col + 1)].Refresh();
                        switch (PLAYER_MARK[(int)pIdx])
                        {
                            case 0:
                                this.Controls[Label(row * 3 + col + 1)].Text = "";
                                this.Controls[Button(row * 3 + col + 1)].BackgroundImage = null;
                                this.Controls[Button(row * 3 + col + 1)].Update();
                                System.Threading.Thread.Sleep(waitTime3);
                                this.Controls[Label(row * 3 + col + 1)].Update();
                                break;
                            case 1:
                                this.Controls[Label(row * 3 + col + 1)].Text = "";
                                this.Controls[Button(row * 3 + col + 1)].BackgroundImage = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\素材\マル.png");
                                this.Controls[Button(row * 3 + col + 1)].Update();
                                System.Threading.Thread.Sleep(waitTime3);
                                this.Controls[Label(row * 3 + col + 1)].Update();
                                break;
                            case 2:
                                this.Controls[Label(row * 3 + col + 1)].Text = "";
                                this.Controls[Button(row * 3 + col + 1)].BackgroundImage = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\素材\バツ.png");
                                this.Controls[Button(row * 3 + col + 1)].Update();
                                System.Threading.Thread.Sleep(waitTime3);
                                this.Controls[Label(row * 3 + col + 1)].Update();
                                break;
                        }
                    }
                }
            }
        }

        public GUIPlay(string[] pluginFiles)
        {
            this.Text = "TicTacToe";     // タイトルを設定
            // プラグインを読み込む
            if (!LoadPlugin(pluginFiles))
            {
                return;
            }

            InitializeComponent();
            Console.WriteLine("コンストラクタ終了.");
        }
      
        private void Form1_Load(object sender, EventArgs e)
        {
            // コンボボックスに要素を追加する
            for (var i = 0; i < _plugins.Count; i++)
            {
                comboBox1.Items.Add(_plugins[i].Name);
                comboBox2.Items.Add(_plugins[i].Name);
            }
            // デフォルトを設定(0個目)
            comboBox1.SelectedIndex = 0;
            comboBox2.SelectedIndex = 0;


            // STARTボタン押すまでマスはクリックできない
            for (int i = 0; i < 9; i ++)
            {
                this.Controls[Button(i + 1)].Enabled = false;
            }

            // アニメーション表示
            // フォームを表示するときの処理
            // 画像ファイルをロードする
            pictureBox1.Image = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\ぱそこんかたかた.gif");

            // BGMを流す
            PlaySound();
        }

        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
           
        }

        // とりあえずコントロールのインデックス操作についていったん忘れる----------------------------------
        // ボタンの設定--------------------------------------------------------------
        //引数indexに番号を受け取って、その番号が付いたButtonコントロールを返す
        string Button(int index)
        {
            return "button" + index.ToString();
        }
        //----------------------------------------------------------------------------

        // ラベルの設定-------------------------------------------------------------
        //引数indexに番号を受け取って、その番号が付いたLabelコントロールを返す
        string Label(int index)
        {
            return "label" + index.ToString();
            //return (Label)(this.Controls["Label" + index.ToString()]);
        }
        //--------------------------------------------------------------------------

        // ピクチャーボックスの設定--------------------------------------------------------------
        //引数indexに番号を受け取って、その番号が付いたButtonコントロールを返す
        string PictureBox(int index)
        {
            return "picturebox" + index.ToString();
        }

        //----------------------------------------------------------------------------------------------------

        // startボタン
        private void button10_Click(object sender, EventArgs e)
        {

            // 設定を取得する-------------------------------------
            // 対戦速度
            mode = trackBar1.Value;
            
            // 対戦回数の上限 0:3回まで　1:10回まで　2:50回まで 3:無限 (AIの時だけ)
            if (radioButton4.Checked == true)
            {
                limit = 0;
            }
            else if (radioButton5.Checked == true)
            {
                limit = 1;
            }
            else if (radioButton6.Checked == true)
            {
                limit = 2;
            }
            else if (radioButton7.Checked == true)
            {
                limit = 3;
            }
            switch (mode)
            {
                case 0:
                    waitTime1 = 0;
                    waitTime2 = 0;
                    waitTime3 = 0;
                    break;
                case 1:
                    waitTime1 = 300;
                    waitTime2 = 80;
                    waitTime3 = 30;
                    break;
                case 2:
                    waitTime1 = 500;
                    waitTime2 = 100;
                    waitTime3 = 50;
                    break;
            }

            //---------------------------------------

            label10.Update();
            // startボタン
            /// <summary>
            /// 実行
            /// </summary>
            /// <param name="files"></param>
            battleCounter += 1;
            textBox1.Refresh();
            textBox1.Text = battleCounter + "ゲーム目を開始します.";
            textBox1.Update();
            System.Threading.Thread.Sleep(waitTime1);
            //System.Threading.Thread.Sleep(1000);
            _ttt.Init();
            for (int i = 0; i < 9; i ++)
            {
                this.Controls[Label(i + 1)].Text = " ";
                this.Controls[Label(i + 1)].Update();

            }
            // コンボボックスで選択したアイテムのインデックス(添え字)を取得
            //　初回はaddで二回目以降は要素番号書き換え
            if (battleCounter == 1)
            {
                _player.Add(_plugins[comboBox2.SelectedIndex]);
                _player.Add(_plugins[comboBox1.SelectedIndex]);

            }
            else
            {
                _player[0] = _plugins[comboBox2.SelectedIndex];
                _player[1] = _plugins[comboBox1.SelectedIndex];
            }
            // 人間がいないときは更新---------------------------------------------------------------------------------------
            if (_player[0].IsHuman == false && _player[1].IsHuman == false)
            {
                winCountPlayer1 = 0;
                winCountPlayer2 = 0;
                battleCounter = 0;
            }
            ShowBoard();
            // スタートボタンを無効化
            button10.Enabled = false;
            // STARTボタン押してマスがクリック可能になる
            for (int i = 0; i < 9; i++)
            {
                this.Controls[Button(i + 1)].Enabled = true;
            }
            if (_player[0].IsHuman == true || _player[1].IsHuman == true)
            {
                Battle();
            }
            // AI対戦の時だけ対戦回数を変化する
            // 対戦回数の上限 0:3回まで　1:10回まで　2:50回まで 3:無限 (AIの時だけ)
            else
            {
                switch (limit) {
                    case 0:
                        for (int i = 0; i < 3; i ++)
                        {
                            Battle();
                            battleCounter += 1;
                            textBox1.Refresh();
                            textBox1.Text = battleCounter + "ゲーム目を開始します.";
                            textBox1.Update();
                            System.Threading.Thread.Sleep(waitTime1);
                            //System.Threading.Thread.Sleep(1000);
                            _ttt.Init();
                            ShowBoard();
                            // スタートボタンを無効化
                            button10.Enabled = false;
                        }
                        break;
                    case 1:
                        for (int i = 0; i < 10; i++)
                        {
                            // なんで二個おいてるんだろう
                            Battle();
                            //Battle();
                            battleCounter += 1;
                            textBox1.Refresh();
                            textBox1.Text = battleCounter + "ゲーム目を開始します.";
                            textBox1.Update();
                            System.Threading.Thread.Sleep(waitTime1);
                            //System.Threading.Thread.Sleep(1000);
                            _ttt.Init();
                            ShowBoard();
                            // スタートボタンを無効化
                            button10.Enabled = false;
                        }
                        break;
                    case 2:
                        for (int i = 0; i < 50; i++)
                        {
                            Battle();
                            //Battle();
                            battleCounter += 1;
                            textBox1.Refresh();
                            textBox1.Text = battleCounter + "ゲーム目を開始します.";
                            textBox1.Update();
                            System.Threading.Thread.Sleep(waitTime1);
                            //System.Threading.Thread.Sleep(1000);
                            _ttt.Init();
                            ShowBoard();
                            // スタートボタンを無効化
                            button10.Enabled = false;
                        }
                        break;
                    case 3:
                        while (true)
                        {
                            Battle();
                            //Battle();
                            battleCounter += 1;
                            textBox1.Refresh();
                            textBox1.Text = battleCounter + "ゲーム目を開始します.";
                            textBox1.Update();
                            System.Threading.Thread.Sleep(waitTime1);
                            //System.Threading.Thread.Sleep(1000);
                            _ttt.Init();
                            ShowBoard();
                            // スタートボタンを無効化
                            button10.Enabled = false;
                            if (STOP == true)
                            {
                                break;
                            }
                        }
                        break;
                }
                // スタートボタンを有効化
                button10.Enabled = true;
            }

        }

        // 先手の選択
        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            // ここにコンボボックス(左)のアイテムが変更された時の処理を記述する
            /// <summary>
            /// プレイヤーの選択
            /// </summary>
        }

        // 後手の選択
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            // ここにコンボボックス(右)のアイテムが変更された時の処理を記述する
            /// <summary>
            /// プレイヤーの選択
            /// </summary>
        }

        /// <summary>
        /// プレイヤーの表示文字列を生成
        /// </summary>
        /// <returns>表示文字列</returns>
        private string PlayerStr()
        {
            return $"{PLAYER_STR[(int)_ttt.Player]}（{_player[(int)_ttt.Player - 1].Name}）";
        }

        private void checkBox2_CheckedChanged(object sender, EventArgs e)
        {
            // 数字表示の選択
            if (checkBox2.Checked == true)
            {
                _showNumber = true;
            }
            else
            {
                _showNumber = false;
            }
        }

        private void label6_Click(object sender, EventArgs e)
        {
            // 何ターン目なのか表示
        }


        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }
        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        // 音声の切り替え
        private void button11_Click(object sender, EventArgs e)
        {

            if (bgmSwitch == false)
            {
                StopSound();
                bgmSwitch = true;
            }
            else
            {
                PlaySound();
                bgmSwitch = false;
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            _ttt.Set(0);
            ShowBoard();
            Battle();
        }

        private void button2_Click(object sender, EventArgs e)
        {
            _ttt.Set(1);
            ShowBoard();
            Battle();
        }

        private void button3_Click(object sender, EventArgs e)
        {
            _ttt.Set(2);
            ShowBoard();
            Battle();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            _ttt.Set(3);
            ShowBoard();
            Battle();
        }

        private void button5_Click(object sender, EventArgs e)
        {
            _ttt.Set(4);
            ShowBoard();
            Battle();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            _ttt.Set(5);
            ShowBoard();
            Battle();
        }

        private void button7_Click(object sender, EventArgs e)
        {
            _ttt.Set(6);
            ShowBoard();
            Battle();
        }

        private void button8_Click(object sender, EventArgs e)
        {
            _ttt.Set(7);
            ShowBoard();
            Battle();
        }

        private void button9_Click(object sender, EventArgs e)
        {
            _ttt.Set(8);
            ShowBoard();
            Battle();
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            // resultのインスタンスを生成
            Result resultForm = new Result();

            // resultを更新
            resultForm.Renew($"{PLAYER_STR[1]}（{_player[0].Name}）", $"{PLAYER_STR[2]}（{_player[1].Name}）", winCountPlayer1, winCountPlayer2, battleCounter, boardData);

            // resultを表示
            resultForm.Show();
            //pictureBox1.Enabled = false;
        }

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            //STOP = true;
        }
    }
}
