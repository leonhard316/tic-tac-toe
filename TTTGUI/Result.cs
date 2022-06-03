using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using System.IO;
using TTT;


namespace TTTGUI
{
    public partial class Result : Form
    {
        //グローバル変数として用意する(親:GUI 子:Result)------------------
        //public int win_count_player1; // 勝ち数
        //public int win_count_player2;

        //public int result_count; // 総対戦数

        // プレイヤーの名前
        string player1Name, player2Name;

        string status;
        // player1 vs player2
        string text1;
        string text2;
        string text;

        public Result()
        {
            InitializeComponent();
        }
        private void Result_Load(object sender, EventArgs e)
        {
        }
        ////グローバル変数として用意する(親:GUI 子:Result)------------------
        //public int win_count_player1; // 勝ち数
        //public int win_count_player2;
        //public int result_count; // 総対戦数
        public void Renew(string player1, string player2, int win_count_player1, int win_count_player2, int result_count, int [] boardData)
        {
            player1Name = player1;
            player2Name = player2;
            // player1 vs player2
            text1 += player1Name + " vs " + player2Name + "\n";
            text2 += "{" + boardData[0] + ", " + boardData[1] + ", " + boardData[2] + ", " + boardData[3] + ", " + boardData[4] + ", " + boardData[5] + ", " + boardData[6] + ", " + boardData[7] + ", " + boardData[8] + "}";
            
            if (win_count_player1 > win_count_player2)
            {
                status = "かち";
            }
            else if (win_count_player1 == win_count_player2)
            {
                status = "あいこ";
            }
            else if (win_count_player1 < win_count_player2)
            {
                status = "まけ";
            }
            text += text1 + text2 + "\n" + "player1の" + status + "\n";
            switch (status)
            {
                case "かち":
                    //------------------Player1----------------------------------
                    pictureBox6.BackgroundImage = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\素材\かち.png");
                    pictureBox2.BackgroundImage = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\素材\にんじんけーき.png");
                    pictureBox6.Update();
                    pictureBox2.Update();
                    //------------------Player2----------------------------------
                    pictureBox7.BackgroundImage = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\素材\まけ.png");
                    pictureBox3.BackgroundImage = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\素材\にんじん.png");
                    pictureBox7.Update();
                    pictureBox3.Update();
                    label1.Text = win_count_player1.ToString();
                    progressBar1.Value = win_count_player1;
                    label1.Update();
                    label2.Text = win_count_player2.ToString();
                    progressBar2.Value = win_count_player2;
                    label2.Update();
                    break;
                case "あいこ":
                    //------------------Player1----------------------------------
                    pictureBox6.BackgroundImage = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\素材\あいこ.png");
                    pictureBox2.BackgroundImage = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\素材\あいこのにんじん.png");
                    pictureBox6.Update();
                    pictureBox2.Update();
                    //------------------Player2----------------------------------
                    pictureBox7.BackgroundImage = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\素材\あいこ.png");
                    pictureBox3.BackgroundImage = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\素材\あいこのにんじん.png");
                    pictureBox7.Update();
                    pictureBox3.Update();
                    label1.Text = win_count_player1.ToString();
                    progressBar1.Value = win_count_player1;
                    label1.Update();
                    label2.Text = win_count_player2.ToString();
                    progressBar2.Value = win_count_player2;
                    label2.Update();
                    break;
                case "まけ":
                    //------------------Player1----------------------------------
                    pictureBox6.BackgroundImage = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\素材\まけ.png");
                    pictureBox2.BackgroundImage = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\素材\にんじん.png");
                    pictureBox6.Update();
                    pictureBox2.Update();
                    //------------------Player2----------------------------------
                    pictureBox7.BackgroundImage = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\素材\かち.png");
                    pictureBox3.BackgroundImage = Image.FromFile(@"D:\ss\SS08\TicTacToe\TTTGUI\素材\にんじんけーき.png");
                    pictureBox7.Update();
                    pictureBox3.Update();
                    label1.Text = win_count_player1.ToString();
                    progressBar1.Value = win_count_player1;
                    label1.Update();
                    label2.Text = win_count_player2.ToString();
                    progressBar2.Value = win_count_player2;
                    label2.Update();
                    break;
            }

            //label1.Text = win_count_player1.ToString();
            //progressBar1.Value = win_count_player1;
            //label1.Update();
            //label2.Text = win_count_player2.ToString();
            //progressBar2.Value = win_count_player2;
            //label2.Update();
        }

        private void pictureBox8_Click(object sender, EventArgs e)
        {

        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            File.WriteAllText("result.txt", text);
        }
    }
}
