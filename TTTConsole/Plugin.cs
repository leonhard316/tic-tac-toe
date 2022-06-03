using System;
using System.Runtime.InteropServices;
using System.Text;

namespace TTT {

    class Plugin {

        // DLLの動的読み込みに必要なWin32API関数
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr LoadLibrary(string lib);
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern void FreeLibrary(IntPtr module);
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr GetProcAddress(IntPtr module, string proc);

        private delegate void GetNameDelegate(StringBuilder s, Int32 bufsize);
        private delegate bool IsHumanDelegate();
        public delegate int MyTurnDelegate(IntPtr board);
        public string Name { get; private set; }
        public bool IsHuman { get; private set; }
        public MyTurnDelegate MyTurn { get; private set; }

        private IntPtr _module;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public Plugin() {
            _module = IntPtr.Zero;
        }

        /// <summary>
        /// デストラクタ
        /// </summary>
        ~Plugin() {
            if (_module != IntPtr.Zero) FreeLibrary(_module);
        }

        /// <summary>
        /// プラグインの読み込み
        /// </summary>
        /// <param name="filename">プラグインDLLファイル名</param>
        /// <returns>読み込みの成否</returns>
        public bool Load(string filename) {
            var module = LoadLibrary(filename);
            if (module == IntPtr.Zero) return false;

            // 名前の取得
            IntPtr method = GetProcAddress(module, "GetName");
            if (method == IntPtr.Zero) return false;
            var GetName = (GetNameDelegate)Marshal.GetDelegateForFunctionPointer(method, typeof(GetNameDelegate));
            System.Text.StringBuilder sb = new System.Text.StringBuilder(256);
            GetName(sb, sb.Capacity);
            Name = sb.ToString();

            // 人間かどうかの取得
            method = GetProcAddress(module, "IsHuman");
            if (method == IntPtr.Zero) return false;
            var isHumanFunc = (IsHumanDelegate)Marshal.GetDelegateForFunctionPointer(method, typeof(IsHumanDelegate));
            IsHuman = isHumanFunc();

            // 思考関数ポインタの取得
            method = GetProcAddress(module, "MyTurn");
            if (method == IntPtr.Zero) return false;
            MyTurn = (MyTurnDelegate)Marshal.GetDelegateForFunctionPointer(method, typeof(MyTurnDelegate));

            return true;
        }
    }
}
