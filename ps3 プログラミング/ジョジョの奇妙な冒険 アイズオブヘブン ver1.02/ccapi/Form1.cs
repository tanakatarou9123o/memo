using PS3Lib;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
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

        bool flag1 = false;
        bool flag2 = false;
        bool flag3 = false;
        bool flag4 = false;
        bool flag5 = false;

        private void Form1_Load(object sender, EventArgs e)
        {
            button1.Enabled = false;
            button2.Enabled = false;
            button3.Enabled = false;
            button5.Enabled = false;
            button6.Enabled = false;

            string[] files = System.IO.Directory.GetFiles(Application.StartupPath,"*.txt");
            Regex regex = new Regex(@"\\[0-9.][0-9.][0-9.][0-9.].*?.txt$");
            MatchCollection matchCol = regex.Matches(files[0]);
            foreach (Match match in matchCol)
            {
                Match xxx = match;
                string a=xxx.ToString().Replace(".txt", "");
                a = a.ToString().Replace("\\", "");
                textBox1.Text = a;
            }

            webBrowser1.ScriptErrorsSuppressed = true;
            webBrowser1.Navigate("http://games24.blog.fc2.com/");

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

        private void button1_Click(object sender, EventArgs e)
        {

            adores = 0x000893D4;

            if (!flag1)
            {
                string bytest = "91230000";
                byte[] s = stringTobytes(bytest);

                PS3.Extension.WriteBytes(adores, s);
                adores = 0x0089368;
                byte[] sss = stringTobytes("91690000");
                PS3.Extension.WriteBytes(adores, sss);

                flag1 = true;
                label1.Text = "ON";
                return;
            }
            
            string bytest1 = "90030000";
            byte[] ss = stringTobytes(bytest1);

            PS3.Extension.WriteBytes(adores, ss);
            adores = 0x0089368;
            byte[] ssss = stringTobytes("90090000");
            PS3.Extension.WriteBytes(adores, ssss);

            flag1 = false;
            label1.Text = "OFF";

        }

        private void button2_Click(object sender, EventArgs e)
        {

            adores = 0x007EC710;
            if (!flag2)
            {
                string bytest = "60000000";
                byte[] s = stringTobytes(bytest);

                PS3.Extension.WriteBytes(adores, s);
                
                flag2 = true;
                label2.Text = "ON";
                return;
            }
            string bytestt = "409D000C";
            byte[] ss = stringTobytes(bytestt);

            PS3.Extension.WriteBytes(adores, ss);
            flag2 = false;
            label2.Text = "OFF";
        }

        private void button3_Click(object sender, EventArgs e)
        {
            adores = 0x007EC77C;
            if (!flag3)
            {
                string bytest = "60000000";
                byte[] s = stringTobytes(bytest);

                PS3.Extension.WriteBytes(adores, s);

                flag3 = true;
                label3.Text = "ON";
                return;

            }
            string bytest1 = "409D000C";
            byte[] ss = stringTobytes(bytest1);

            PS3.Extension.WriteBytes(adores, ss);
            flag3 = false;
            label3.Text = "OFF";
        }

        private void button4_Click(object sender, EventArgs e)
        {
            int fl = PS3.ConnectTarget(textBox1.Text);
            if (!PS3.SUCCESS(fl))
            {
                MessageBox.Show("PS3.ConnectTarget失敗");
                return;
            }

            PS3.AttachProcess();

            button1.Enabled = true;
            button2.Enabled = true;
            button3.Enabled = true;
            button5.Enabled = true;
            button6.Enabled = true;
        }
        

        private void button5_Click(object sender, EventArgs e)
        {
            adores = 0x0006006C;
            if (!flag4)
            {
                string bytest = "60000000";
                byte[] s = stringTobytes(bytest);

                PS3.Extension.WriteBytes(adores, s);

                flag4 = true;
                label4.Text = "ON";
                return;

            }
            string bytest1 = "409D0008";
            byte[] ss = stringTobytes(bytest1);

            PS3.Extension.WriteBytes(adores, ss);
            flag4 = false;
            label4.Text = "OFF";
        }

        private void button6_Click(object sender, EventArgs e)
        {
            adores = 0x0005EB64;
            if (!flag5)
            {
                int jj=int.Parse(textBox2.Text);
                string bytest = "1D7C00"+jj.ToString("X2");
                byte[] s = stringTobytes(bytest);

                PS3.Extension.WriteBytes(adores, s);

                flag5 = true;
                label5.Text = "ON";
                return;

            }
            string bytest1 = "7F8BE378";
            byte[] ss = stringTobytes(bytest1);

            PS3.Extension.WriteBytes(adores, ss);
            flag5 = false;
            label5.Text = "OFF";
        }
    }
}
