# in16-dclock

このリポジトリは IN-16 ニキシー管を用いたニキシー管時計の C ソースコードを管理するリポジトリです．

気が向いたら Qiita や elchika で記事を書きたいと思います．

(この README.md をはじめリポジトリ自体の整理なども追々やっていくつもりです．)

## 環境

| 項目 | 使用した物 |
|:---:|:---:|
| マイコン | PIC16F1829 |
| IDE | MPLABX |

MPLABX のバージョンとかもちゃんと調べておきたいですね．

## ToDo

- [ ] ソースコードのリファクタリング
  - 「とりあえず動けばよし！」ポリシーでコードを書いたので現状めちゃくちゃ汚い．
  - しかもこのコードを書いたのはプログラミング初心者みたいな頃だったので尚更汚い．
    - 今でもプログラミング入門者みたいなものだが．
  - なのでリファクタリングをしたい．
- [ ] ソースコードの使い方
  - 「MPLABX (PIC マイコン向けの IDE) でプロジェクトを作ってこのソースコードを貼り付けて焼き込むと動くよ」って話をどこかでドキュメント化したい．
    - 「IDE を使うと config statements なんかも GUI ポチポチで自動生成してくれるよ」なんて話も書きたい．
- [ ] 想定している回路図
  - このプログラムがどういう回路を想定したものなのかを示したい．
    - でないと何故このピンを high / low しているの？とかがわからないから．

## 参考

今回のニキシー管時計やそのプログラムを作成するにあたって参考にさせて頂いたサイト等です．
ありがとうございました．

- [【STEINS;GATE】ダイバージェンスメーターver2.000　作ってみました【くろや】](https://www.youtube.com/watch?v=zzIE0lq0-xY&t=412s)
- [【STEINS;GATE】ダイバージェンスメーターver2.001　作ってみました【くろや】](https://www.youtube.com/watch?v=g4h8LvY5Uvw)
- [ダイバージェンスメーターTV版作ってみました【くろや】　Divergence　Meter](https://www.youtube.com/watch?v=EB0NIFx7f5E)
- [ダイバージェンスメーター](https://www.youtube.com/watch?v=7qoxnkneIpA)
- [ダイバージェンスメーター(ニキシー管時計)](https://www.youtube.com/watch?v=6DML1LP6F_M)
- [ニキシー管時計（IN-16）](https://ehbtj.com/portfolio/interior/nixie-tube-clock-in-16/)
- [IN-16 (ИН-16) Nixie Röhre/Nixie Tube](www.tube-tester.com/sites/nixie/data/in16.htm)
- [タイマー１の使い方](http://www.picfun.com/pic24.html)
- [タイマの使い方](http://www.picfun.com/f1/f03.html)
- [自作ニキシー管時計（１） : まぢぽん製作所](http://mgpn.livedoor.blog/archives/51734103.html)
- [NTPで合わせるニキシー管時計](https://n.mtng.org/ele/arduino/nixie/)
- [ニキシー管時計 (with Arduino) 製作記 【Part 4】](http://binarymacaron.xyz/first-nixie-tube-digital-clock-making-report-4/)
- [ニキシー管駆動回路再考](http://poallo.cocolog-nifty.com/blog/2010/03/post-8c5a.html)
- [[連載]第5回　ニキシー管を点灯させるお話](https://inter-culture.jp/library/3d_print/nixie05.html)
- [ニキシー管時計の制作 - たるたるソース](http://taltalp.hatenablog.jp/entry/2015/09/28/031452)
- [FETの使い方＆選定ガイド | マルツオンライン](https://www.marutsu.co.jp/pc/static/large_order/fet_3)
- [クロック発振回路の考え方](www.picfun.com/pic20.html)
- [割込みの使い方](http://www.picfun.com/pic08.html)
