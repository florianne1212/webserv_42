# start image
FROM ubuntu:18.04

## installation process
#
RUN apt-get -y update
RUN apt-get install -y git wget vim clang make zsh valgrind siege

RUN sh -c "$(wget -O- https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get install -y php php-cgi php-mysql

RUN mkdir /app

WORKDIR /app
EXPOSE 9000

COPY . /app

CMD valgrind --track-fds=yes /app/webserv /app/configuration_file/testDocker.conf