## 目的
本レポジトリは早稲田大学基幹理工学部清水佳奈教授「アルゴリズムとデータ構造B」のグループワーク課題、グループ13の成果物を保存するためのレポジトリである。  

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
makeでコンパイルをし、test.shでそれらを実行します。test.shのあとの数字でbs, npを指定できます(pdf参照)。
```shell
make
chmod 755 test.sh
./test.sh 1 0 0 0 0 0
```
でよい。また、
```shell
make clean
```
を実行することで実行コマンドで生成されたファイルを削除することができる。
![image](https://github.com/user-attachments/assets/f985d436-4ae4-4207-850a-55391def1978)
