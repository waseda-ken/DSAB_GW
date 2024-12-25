## 目的
本レポジトリはアルゴリズムとデータ構造Bのグループワークの提出するファイル(enc_13.c, dec_13.c, conf_13.txt)を保存するためのレポジトリである。  

## 注意事項
本番ではenc_13.c, dec_13.cと設定ファイルconf_13.txtしか提出せず、他のファイルは向こうが用意するので、上記３ファイル以外は触らないように注意してください。
## 環境構築
UbuntuのインストールはWindowsならWSLでできます。Macはわからないので、頑張って調べてください、、AzureLinuxでは一応動くらしいので、
一旦そっちを使ってください。本番の環境はUbuntuなので、最後はUbuntuで確認したいです。  
以下のコードをターミナルで実行してください。
```shell
git clone https://github.com/nobu-h-o/DSAB_GW.git
```

## テスティング
本番では本レポジトリのファイル(orgdata含まない)をtarに圧縮して以下のコードを実行する（らしい）
```shell
tar xvf grpwk24.tar
cd grpwk24
make
chmod 755 test.sh
./test.sh 1 0 0 0 0 0
```
が、ローカル環境では、
```shell
make
chmod 755 test.sh
./test.sh 1 0 0 0 0 0
```
でよい。また、付随のPythonファイルを利用し、生成したファイルをすべて削除することができる。
