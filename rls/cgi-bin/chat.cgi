#!/usr/local/bin/perl
$ver = ''; 
$quick = 1;
require './jcode.pl' if (!$quick);
$title = "CS 160 Fastchat";
$master2 = "<font face=\"times new roman\" size=\"4\">Tseng has cleared chat!</font>";
$sweep_msg = "";
$t_color = "#666999";
$t_size = '18pt';
$b_size = '12pt';
$body = '<body bgcolor="#000000" text="#999cc">';
$homepage = 'https://sjsu.desire2learn.com/';
$max = 1000;
@COLORS = ('FFD684','FF0000','00FF00','0000EE','00FFFF','FFFF00','FFCCFF','FF800','000080','804000','800080','FFCCFF','C0C0C0');
@IROIRO = ('Default','Red','Green','Blue','Aqua','Yellow','Pink','Orange','Navy','Brown','Purple','Pink','Grey');
@retime = (10,20,30,40,50,60);
$retime = 20;
$sikiri = '<br>';
$in_msg  = " has come to chat!";
$out_msg = " has left chat!";
$rep_color = "#878796";
$tagkey = 1;
$method = 'GET';
$script  = './chat.cgi';
$logfile = 'check.txt';
$lockkey = 0;
$lockfile = './ponny.lok';
&decode;
if ($in{'mode'} eq "form") { &form1; }
elsif ($in{'mode'} eq "into") { &form2; }
elsif ($in{'comment'} && $in{'mode'} eq "regist") { &regist; }
elsif ($in{'mode'} eq "byebye") { &byebye; }
&log_view;
sub form1 {
&header('body');
print <<"EOM";
<center>
<form method="$method" action="$script" target="form" name="ponny">
<input type=hidden name=mode value="into">
<table border=0 cellspacing=0>
<tr><th colspan=2><font color="$t_color" size=5><b style="font-size:$t_size">$title</b></font></th></tr>
<tr><td><b>NICKNAME: </b> <input type=text name=name size=20></td></tr>
<tr><td>Refresh: <select name=retime>
EOM
$in{'retime'} = $retime;
foreach (@retime) {
if ($in{'retime'} == $_) { print "<option value=$_ selected>$_不ec\n"; }
else { print "<option value=$_>$_不ec\n"; }
}
print "</select> Color: <select name=color>\n";
foreach (0 .. $#COLORS) {
print "<option value=\"$COLORS[$_]\">$IROIRO[$_]\n";
}
print <<"EOM";
</select></td></tr></table>
<table cellpadding=0 cellspacing=0><tr>
<th><input type=submit value=" Cruise In! "></th></form>
<th><form action="$homepage" target="_top"><input type=submit value=" To D2L"></th>
</form></tr></table>
<SCRIPT LANGUAGE="JavaScript">
<!--
self.document.ponny.name.focus();
//-->
</SCRIPT>
</body></html>
EOM
exit;
}
sub form2 {
&regist('into');
if ($ENV{'HTTP_USER_AGENT'} =~ /MSIE/i) { $text_width = 90; }
else { $text_width = 50; }
&header;
print <<"EOM";
<SCRIPT LANGUAGE="JavaScript">
<!--
function autoclear() {
 if (self.document.send) {
  if (self.document.cmode && self.document.cmode.autoclear) {
   if (self.document.cmode.autoclear.checked) {
    if (self.document.send.comment) {
      self.document.send.comment.value = "";
      self.document.send.comment.focus();
    }
   }
  }
 }
}
// -->
</SCRIPT>
</head><killbanner>
$body
<form name=send method=$method action="$script" target="log" onSubmit="setTimeout(&quot;autoclear()&quot;,10)">
<input type=hidden name=mode value="regist">
<input type=hidden name=name value="$in{'name'}">
<b><font color=ffff33>Message </b> </font>: <input type=text size="$text_width" name=comment><br>
<font color=ffff33>Refresh</font>: <select name=retime>
EOM
foreach (@retime) {
if ($in{'retime'} == $_) { print "<option value=$_ selected>$_不ec\n"; }
else { print "<option value=$_>$_不ec\n"; }
}
print "</select> <font color=ffff33>Color</font>: <select name=color>\n";
foreach (0 .. $#COLORS) {
if ($in{'color'} eq "$COLORS[$_]") {
print "<option value=$COLORS[$_] selected>$IROIRO[$_]\n";
} else {
print "<option value=$COLORS[$_]>$IROIRO[$_]\n";
}
}
print <<"EOM";
</select>
<input type=submit value="Send to"   style=\"color:#ffffcc; background-color:#000000;\"><input type=reset value="Clear!"   style=\"color:#ffffcc; background-color:#000000;\"> </form>
<form action="$script" method="$method" target=form>
<table><tr>
<td>
  <input type=submit value="Bail Out!">
  <input type=hidden name=mode value="byebye">
  <input type=hidden name=name value="$in{'name'}">
</td></form>
<td valign=top>
  <form name="cmode">
  <input type="checkbox" name="autoclear" checked><font color=ffff33>Auto-Clear! </font>
</td></form></tr></table>
</body></html>
EOM
exit;
}
sub log_view {
if ($in{'retime'} eq "") { $in{'retime'} = $retime; }
&header;
if ($in{'retime'} != 0) {
print "<META HTTP-EQUIV=\"refresh\" CONTENT=\"$in{'retime'}; URL=$script?retime=$in{'retime'}\">\n";
}
print "</head>\n$body\n
<div align=right><font color=aaFFaa face=GameSpyFixSm=2 selected><i>Reload Time:</i></font> ";

if ($in{'retime'} == 0) { print "手動"; } else { print "$in{'retime'} seconds. "; }
print "</div><hr>\n";
open(IN,"$logfile") || &error("Open Error : $logfile");
while (<IN>) {
($date,$name,$com,$color) = split(/<>/);
print "<font color=\"$color\"><b>$name</b> > $com</font> <font face=impact color=1b2b39 size=-2> $date</font> <br>\n";
}
close(IN);
print "<center><small><!-- $ver -->\n";
print "- <a href='http://www.kent-web.com/chat/ponny.html' target='_blank'>see where I obtained this chat script</a> -\n";
print "</small></center>\n</body></html>\n";
exit;
}
sub regist {
($sec,$min,$hour,$mday,$mon) = localtime(time);
$date = sprintf("%02d/%02d-%02d:%02d:%02d",$mon+1,$mday,$hour,$min,$sec);
if ($_[0] eq 'into') {
$in{'comment'} = "<b>$in{'name'}</b>$in_msg";
$name  = "Tseng says";
$color = $rep_color;
} elsif ($_[0] eq 'bye') {
$in{'comment'} = "<b>$in{'name'}</b>$out_msg";
$name  = "Tseng says";
$color = $rep_color;
} else {
$name  = $in{'name'};
$color = $in{'color'};
}
if ($lockkey) { &lock; }
open(IN,"$logfile") || &error("Open Error : $logfile");
@lines = <IN>;
close(IN);


      if ($in{'comment'} eq 'clear'){
                @temp=();
                local($match)=0;
                foreach (@lines) {
                        ($name) = (split(/<>/))[5];
                        if ($name eq $name) { $match=1; }
                        else { push(@temp,$_); }
                }
                if ($match) { @lines=@temp;
                $in{'comment'} = "$sweep_msg";
                $name  = "<b>$master2</b>";
                $color = $rep_color; }
    }



while ($max <= @lines) { pop(@lines); }
unshift (@lines,"$date<>$name<>$in{'comment'}<>$color<>\n");
open(OUT,">$logfile") || &error("Write Error : $logfile");
print OUT @lines;
close(OUT);
unlink($lockfile) if (-e $lockfile);
}
sub decode {
if ($ENV{'REQUEST_METHOD'} eq "POST") {
read(STDIN, $buffer, $ENV{'CONTENT_LENGTH'});
} else { $buffer = $ENV{'QUERY_STRING'}; }
@pairs = split(/&/, $buffer);
foreach (@pairs) {
($name,$value) = split(/=/);
$value =~ tr/+/ /;
$value =~ s/%([a-fA-F0-9][a-fA-F0-9])/pack("C", hex($1))/eg;
&jcode'convert(*value,'sjis') if (!$quick);
$value =~ s/\r//g;
$value =~ s/\n//g;
if ($tagkey) { $value =~ s/<>/&lt;&gt;/g; }
else { $value =~ s/</&lt;/g; $value =~ s/>/&gt;/g; }
$in{$name} = $value;
}
if ($in{'name'} eq "") { $in{'name'} = ""; }
}
sub byebye {
&regist('bye');
&header('body');
print <<"EOM";
<center><h3>$in{'name'}: May you enscribe your saga upon the scrolls of legendry!</h3>
[<a href="$homepage" target="_top">EXIT</a>]</center>
</body></html>
EOM
exit;
}
sub error {
unlink($lockfile) if (-e $lockfile);
&header('body') if (!$head_flag);
print "<center><hr width='70%'><h3>ERROR !</h3>\n";
print "<font color=red><B>$_[0]</B></font>\n";
print "<P><hr width='70%'></center>\n</body></html>\n";
exit;
}
sub header {
$head_flag=1;
print "Content-type: text/html\n\n";
print <<"EOM";
<html><head><killbanner>
<META HTTP-EQUIV="Content-type" CONTENT="text/html; charset=Shift_JIS">
<STYLE TYPE="text/css">
<!-- body,tr,td,th { font-size:$b_size } -->
</STYLE>
<title>$title</title>
EOM
if ($_[0] eq "body") { print "</head>\n$body\n"; }
}
sub lock {
local($flag)=0;
foreach (1 .. 5) {
if (-e $lockfile) { sleep(1); }
else {
open(LOCK,">$lockfile") || &error("Lock Error");
close(LOCK);
$flag=1; last;
}
}
if (!$flag) { &error("LOCK is BUSY"); }
}