using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TTTGUI
{
    public partial class Setting : Form
    {
        public Setting()
        {
            InitializeComponent();
        }

        private void Form2_Load(object sender, EventArgs e)
        {
            this.Text = "Setting";
            // 最小値、最大値を設定
            trackBar1.Minimum = 0;
            trackBar1.Maximum = 2;

            // 初期値を設定
            trackBar1.Value = 0;

            // 描画される目盛りの刻みを設定
            trackBar1.TickFrequency = 2;

            // スライダーをキーボードやマウス、
            // PageUp,Downキーで動かした場合の移動量設定
            trackBar1.SmallChange = 1;
            trackBar1.LargeChange = 2;

            // 値が変更された際のイベントハンドらーを追加
            trackBar1.ValueChanged += new EventHandler(trackBar1_ValueChanged);
        }

        void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            // TrackBarの値が変更されたら対戦速度を変更
            
        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {
        }

        private void trackBar1_Scroll(object sender, EventArgs e)
        {
        }
    }
}
