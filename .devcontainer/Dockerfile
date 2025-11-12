# Ubuntu ベース
FROM ubuntu:22.04

# 開発ツールをインストール
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    gdb \
    make \
    git \
    && apt-get clean

# 作業ディレクトリ
WORKDIR /workspace

CMD ["/bin/bash"]