//
// �C���e���J�[�\���p		2002/7/1
//
//

<!-- ���΃p�X

var PicPathSrc = "pic/";
var PicPath = "pic/";

function MakePicPath( Level )
{
	var i = 0;
	PicPath = PicPathSrc;
	while ( Level > 0 )
	{
		PicPath = "../" + PicPath;
		Level--;
	}
}
//-->

<!-- �ŏI�X�V��
function LastUpdate ()
{
	var str = "<FONT SIZE='2' COLOR='#ff8080'>�ŏI�X�V���F";
	var last = new Date ( document.lastModified );
	var wk = new Array ( "��","��","��","��","��","��","�y" );
	var yy = last.getYear ();
	var mm = last.getMonth () + 1;
	var dd = last.getDate ();
	var ww = last.getDay ();
	var ho = last.getHours ();
	var mi = last.getMinutes ();
	if ( yy < 2000 ) yy += 1900;
	str = str + "<B>" + yy + "</B>�N<B>" + mm + "</B>��<B>" + dd + "</B>��" + "�i<B>" + wk[ww] + "</B>�j"
	str = str + "<B>" + ho + "</B>��<B>" + mi + "</B>��" + "</FONT>";
	document.write ( str );
}
//-->

<!-- �y�[�W�g�b�v
function PageTop ( Level,Title )
{
	MakePicPath( Level );
	var str1 = '<CENTER><TABLE><TBODY>';
	str1 += '<TR><TD><IMG src="'+PicPath+'nell_idx.gif" alt="���A�l�������" align="right" width="56" height="56"></TD>';
	str1 += '<TD valign="middle" align="left"><FONT size="5"><STRONG>';
	var str2 = '</STRONG></FONT></TD></TR>';
	str2 += '<TR><TD colspan="2" align="center"><SCRIPT language="JavaScript">LastUpdate()</SCRIPT></TR>';
	str2 += '<TR><TD colspan="2" align="center"><IMG src="'+PicPath+'ttl_help.gif" alt="������" width="480" height="20"></TD></TR>';
	str2 += '</TBODY></TABLE></CENTER><HR>';

	document.write ( str1 + Title + str2 );
}
//-->

<!-- �y�[�W�{�g��
function PageBottom ( Level )
{
	MakePicPath( Level );
	var str = '<HR><CENTER><TABLE><TBODY>';
   	str += '<TR><TD align="center" valign="middle" colspan="2"><A href="javaScript:history.go(-1)"><IMG src="'+PicPath+'back_n.gif" width="72" height="54" border="0" alt="�߂�"></A>'
   	str += '<A href="#top"><IMG src="'+PicPath+'top_n.gif" width="72" height="54" border="0" alt="�y�[�W�擪��"></A> <A href="javaScript:history.go(1)"><IMG src="'+PicPath+'next_n.gif" width="72" height="54" border="0" alt="�i��"></A></TD></TR>'
	str += '<TR><TD colspan="2" align="center"><SCRIPT language="JavaScript">LastUpdate()</SCRIPT></TD></TR>';
	str += '<TR><TD><A href="http://homepage3.nifty.com/ic/"><IMG src="'+PicPath+'bann_ic.png" width="88" height="31" border="0" alt="�C���e���J�[�\�� �T�|�[�g�T�C�g"></A></TD>';
	str += '<TD><FONT size="2"> ���ӌ���<A href="mailto:tatsuo.sugihara@nifty.ne.jp"><STRONG>tatsuo.sugihara@nifty.ne.jp</STRONG></A>�܂�</FONT></TD></TR>';
	str += '</TBODY></TABLE></CENTER>';

	document.write ( str );
}
//-->

<!-- �y�[�W�{�g��
function PageBottomIndex ( Level )
{
	MakePicPath( Level );
	var str = '<HR><CENTER><TABLE cellpadding="3"><TBODY>';
    	str += '<TR><TD align="center" valign="middle" colspan="3"><A href="javaScript:history.go(-1)"><IMG src="'+PicPath+'back_n.gif" width="72" height="54" border="0" alt="�߂�"></A>';
	str += ' <A href="#top"><IMG src="'+PicPath+'top_n.gif" width="72" height="54" border="0" alt="�y�[�W�擪��"></A> <A href="javaScript:history.go(1)"><IMG src="'+PicPath+'next_n.gif" width="72" height="54" border="0" alt="�i��"></A></TD></TR>';
	str += '<TR><TD colspan="3" align="center"><SCRIPT language="JavaScript">LastUpdate()</SCRIPT></TD></TR>';
    	str += '<TR><TD><FONT size="2">���ӌ���<A href="mailto:tatsuo.sugihara@nifty.ne.jp" title = "���C�y�ɁI"><STRONG>tatsuo.sugihara@nifty.ne.jp</STRONG></A>�܂�</FONT></TD>';
    	str += '<TD><FONT size="2"><A href="http://homepage3.nifty.com/ic/"><IMG src="'+PicPath+'/bann_ic.png" width="88" height="31" border="0" alt="�C���e���J�[�\�� �T�|�[�g�T�C�g"></FONT></TD>';
    	str += '<TD><IMG src="http://hpcounter3.nifty.com/cgi-bin/counter.cgi?u=yfa27838&p=31&c=6"></TD></TR>';
	str += '</TBODY></TABLE></CENTER>';

	document.write ( str );
}
//-->

<!-- �J�E���^�[
function Counter ()
{
	var str = '<P align="center"><IMG src="http://hpcounter3.nifty.com/cgi-bin/counter.cgi?u=yfa27838&p=31&c=6"></P>';
	document.write ( str );
}
//-->

<!-- New
function IndexNew ( Level,index )
{
	MakePicPath( Level );
	var str = '<P><IMG src="'+PicPath+'newanim.gif" width="40" height="20" border="0" align="left" alt="NEW"><B> <FONT size="+1"> ';
	str += index;
	str += '</FONT></B></P>'
	document.write ( str );
}
//-->

<!-- Version up
function IndexVerup ( Level,index )
{
	MakePicPath( Level );
	var str = '<P><IMG src="'+PicPath+'vupani.gif" width="32" height="20" border="0" align="left" alt="Ver.up"><B> <FONT size="+1"> ';
	str += index;
	str += '</FONT></B></P>'
	document.write ( str );
}
//-->

<!-- �o�O�C��
function IndexBugfix ( Level,index )
{
	MakePicPath( Level );
	var str = '<P><IMG src="'+PicPath+'bugani.gif" width="20" height="20" border="0" align="left" alt="�o�O�C�� m(_ _)m"><B> <FONT size="+1"> ';
	str += index;
	str += '</FONT></B></P>'
	document.write ( str );
}
//-->

<!-- ���o����
function IndexL ( Level,index )
{
	MakePicPath( Level );
	var str = '<P align="left"><IMG src="'+PicPath+'leaf.gif" width="20" height="20" alt="�Ηt"><FONT size="4"><STRONG> ';
	str += index;
	str += '</STRONG></FONT></P>'
	document.write (  str );
}
//-->

<!-- ���o��
function Index ( Level,index )
{
	MakePicPath( Level );
	var str = '<P align="left"><IMG src="'+PicPath+'leaf2.gif" width="16" height="16" alt="�o�t"><FONT size="3"><STRONG> ';
	str += index;
	str += '</STRONG></FONT></P>'
	document.write (  str );
}
//-->

<!-- ����
function Attention ( Level,index )
{
	MakePicPath( Level );
	var str = '<P align="left"><IMG src="'+PicPath+'attent.gif" width="24" height="24" alt="����"><FONT size="3" color="#ff0000"><STRONG> ';
	str += index;
	str += '</STRONG></FONT></P>'
	document.write (  str );
}
//-->

<!-- �q���g
function Hint ( Level,index )
{
	MakePicPath( Level );
	var str = '<P align="left"><IMG src="'+PicPath+'hint.gif" width="48" height="24" alt="�q���g"><FONT size="3" color="#00aa00"><STRONG> ';
	str += index;
	str += '</STRONG></FONT></P>'
	document.write (  str );
}
//-->







<!--HPB_SCRIPT_ROV_50
//
//  Licensed Materials - Property of IBM
//  11P5743
//  (C) Copyright IBM Corp. 1998, 2000 All Rights Reserved.
//

// PreloadImage:
//
function PreloadImage()
{
  var appVer=parseInt(navigator.appVersion);
  var isNC=(document.layers && (appVer >= 4));
  var isIE=(document.all    && (appVer >= 4));
  if (isNC || isIE)
  {
    if (document.images)
    {
      var imgName = PreloadImage.arguments[0];
      var cnt;
      swImg[imgName] = new Array;
      for (cnt = 1; cnt < PreloadImage.arguments.length; cnt++)
      {
        swImg[imgName][PreloadImage.arguments[cnt]] = new Image();
        swImg[imgName][PreloadImage.arguments[cnt]].src = HpbImgPreload.arguments[cnt];
      }
    }
  }
}
// HpbImgFind:
//
function HpbImgFind(doc, imgName)
{
  for (var i=0; i < doc.layers.length; i++)
  {
    var img = doc.layers[i].document.images[imgName];
    if (!img) img = HpbImgFind(doc.layers[i], imgName);
    if (img) return img;
  }
  return null;
}
// HpbImgSwap:
//
function HpbImgSwap(imgName, imgSrc)
{
  var appVer=parseInt(navigator.appVersion);
  var isNC=(document.layers && (appVer >= 4));
  var isIE=(document.all    && (appVer >= 4));
  if (isNC || isIE)
  {
    if (document.images)
    {
      var img = document.images[imgName];
      if (!img) img = HpbImgFind(document, imgName);
      if (img) img.src = imgSrc;
    }
  }
}
var swImg; swImg=new Array;
//-->

