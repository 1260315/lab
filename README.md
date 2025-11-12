# Dockerコンテナ利用方法

今回作成したコンテナは、ubuntu:22.04をベースに、C言語プログラムの開発・コンパイル・実行を行える環境を構築するもの。
VScodeのDevContainerで接続することで、以下のことができるようになっている。
* VScodeでのテキストファイルの編集
* ホストディレクトリとコンテナ内の作業ディレクトリの同期
* Gitの認証を、VScodeの設定を利用して行うことができる

DockerコンテナをDevContainerで接続するには、以下の手順を実行する

### 1. Gitをクローンする。
クローンしたいディレクトリで、以下のコマンドを実行する。
```
git clone https://github.com/1260315/lab.git
```
### 2. Dev Container: Reopen in Folder を実行する
クローンしたディレクトリをVScodeで開く。
  →vscodeの左上の「ファイル」→「フォルダーを開く」→クローンしたディレクトリを選択

Ctrl + Shift + P で画面上部に選択肢が出てくるので、Dev Container: Reopen in Folderを選択する。

これだけで、イメージのビルド・コンテナの起動・Shellへの接続　が実行される。
