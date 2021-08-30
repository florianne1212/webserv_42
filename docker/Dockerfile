# start image
FROM ubuntu:18.04

## installation process
#
RUN apt-get -y update
RUN apt-get install -y git wget vim clang make zsh valgrind siege

RUN sh -c "$(wget -O- https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"

#RUN apt-get install -y php php-cgi php-mysql
#php php-cgi php-cli php-mbstring php-fpm php-mysql

RUN mkdir /app

WORKDIR /app
EXPOSE 9000

COPY . /app

CMD valgrind --track-fds=yes /app/webserv configuration_file/testDocker.conf