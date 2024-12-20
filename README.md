本レポジトリはアルゴリズムとデータ構造Bのグループワークのソースコードとorgdataを保存するためのレポジトリである。　　
UbuntuのインストールはWindowsならWSL、Macはわからないので、頑張って調べてください、、AzureLinuxでは一応動くらしいので、
そっちを使ってください。本番の環境は　Ubuntuなので、最後はUbuntuで確認したいです。

本番では本レポジトリのファイル(orgdata含まない)をtarに圧縮して以下のコードを実行する（らしい）
```
tar xvf grpwk24.tar
cd grpwk24
make
chmod 755 test.sh
./test.sh 1 0 0 0 0 0
```
が、ローカル環境では、
```
make
chmod 755 test.sh
./test.sh 1 0 0 0 0 0
```
でよい。
