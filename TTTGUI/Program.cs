using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace TTTGUI
{
    class Program
    {
        /// <summary>
        /// アプリケーションのメイン エントリ ポイントです。
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {

            // プラグインDLL取得
            var pluginFiles = Directory.GetFiles(".", "*.dll");

            // プレイ
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new GUIPlay(pluginFiles));
        }
    }
}
