# FROM ubuntu:22.04
FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu-22.04
ENV DEBIAN_FRONTEND=noninteractive 

# Install dependencies.
RUN apt update && apt-get install -y git make cmake build-essential python-is-python3 python-dev-is-python3 python3-pip libdw-dev openssh-client

# Update pip and switch to Tsinghua source.
RUN python -m pip install -i https://pypi.tuna.tsinghua.edu.cn/simple --upgrade pip && pip config set global.index-url https://pypi.tuna.tsinghua.edu.cn/simple