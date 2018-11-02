# tinyCPU
2018年度 情報科学Ⅱにおいて提示された「あるプロセッサの命令セット」を実装したエミュレータです。

## 命令セット
講義資料中の表1.1に示された、以下の命令セットを実装しています。

|命令|計算の内容|
|:-|:-|
|ADD|2つの数の足し算を行う（演算レジスタ+引数）|
|SUBTRACT|2つの数の引き算を行う（引数-演算レジスタ）|
|MULTIPLY|2つの数の掛け算を行う（演算レジスタ*引数）|
|DIVIDE|2つの数の割り算を行う（演算レジスタ/引数）|
|LOAD|メモリから演算レジスタにデータを読み出す|
|STORE|メモリに演算レジスタのデータを書き込む|
|JUMP|指定されたメモリ番地にジャンプする|
|JUMPZERO|演算レジスタの内容が0ならジャンプする|
|HALT|停止する|
|PRINT|演算レジスタの内容を印刷する|

## 使い方
### クローンとコンパイル

ターミナル上で、
```bash
cd && git clone https://github.com/greenlaver/tinyCPU.git && \
cd ~/tinyCPU && make
```
とすれば、エミュレータ本体のプログラム`tinyCPU`が生成されます。

### プログラム実行
同梱しているアセンブリ言語が記述された`ASM`ファイルを実行する場合、
```bash
./tinyCPU ASM
```
とすれば
```console
$ ./tinyCPU ASM
[PRINT] : 10300.000000
[PRINT] : 10609.000000
[PRINT] : 10927.270000
...
[PRINT] : 186588.660003
[PRINT] : 192186.319803
[HALT]
```
のように実行結果が得られます。

---
This program written by [Aonori](https://twitter.com/aonr_jp) and [なぎ@nagin](https://twitter.com/nagi_kknn)