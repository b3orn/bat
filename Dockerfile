FROM ubuntu:latest

RUN apt update && apt install -y build-essential

WORKDIR /mnt