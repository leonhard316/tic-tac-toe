using System;
using System.IO;

namespace TTTConsole {

    class Program {

        /// <summary>
        /// プログラム開始点
        /// </summary>
        /// <param name="args"></param>
        static void Main(string[] args) {

            // プラグインDLL取得
            var pluginFiles = Directory.GetFiles(".", "*.dll");

            // プレイ
            var play = new ConsolePlay();
            play.Run(pluginFiles);

            // 終了
            Console.Write("Press [Enter] to exit.");
            Console.ReadLine();
        }
    }
}
