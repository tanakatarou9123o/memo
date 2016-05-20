using PS3Lib;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ccapi
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private CCAPI PS3 = new CCAPI();
        uint adores = 0;
        string ip;


        private void button1_Click(object sender, EventArgs e)
        {
            Regex regex = new Regex(@"\d*\.\d*\.\d*\.\d*$");
            

            FileStream file = new FileStream("ip.txt", FileMode.OpenOrCreate, FileAccess.ReadWrite);
            StreamReader reader = new StreamReader(file);//FileStreamのインスタンス
            StreamWriter writer= new StreamWriter(file);//FileStreamのインスタンス;

            if (reader.EndOfStream ==true)
            {
                ip = textBox1.Text;
                writer.WriteLine(ip);
            }
            while (reader.EndOfStream == false)//最後になるまで
            {
                string a = reader.ReadLine();//一行読み込み
                MatchCollection matchCol = regex.Matches(a);
                foreach (Match match in matchCol)
                {
                    Match xxx = match;
                    textBox1.Text = xxx.Value;
                    ip = xxx.Value;
                }
            }
            writer.Close();


            int fl =PS3.ConnectTarget(textBox1.Text);
            if (!PS3.SUCCESS(fl))
            {
                MessageBox.Show("PS3.ConnectTarget失敗");
                return;
            }

            PS3.AttachProcess();

            adores= 0x01A33230;
            string bytest = "600000004BFFFFB42C0700074081FF88";
            byte[] s = stringTobytes(bytest);
            
            PS3.Extension.WriteBytes(adores, s);

            adores = 0x01A33D9C;
            s = stringTobytes("7CE3232E4E8000208063007C7C6320AE");
            PS3.Extension.WriteBytes(adores, s);


            
        }

        byte[] stringTobytes(string bytest)
        {
            int length = bytest.Length / 2;//文字数からバイト数取得
            byte[] bytes = new byte[length];//バイト数分の配列
            int j = 0;
            for (int i = 0; i < length; i++)
            {
                bytes[i] = Convert.ToByte(bytest.Substring(j, 2), 16);
                j += 2;
            }

            return bytes;
        }

    }
}
