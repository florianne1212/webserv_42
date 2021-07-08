# start image
FROM debian:buster

## installation process
#
RUN apt-get -y update \
&& apt-get -y upgrade \
\
#php
&& apt-get -y install php php-cgi php-cli php-mbstring php-fpm php-mysql \
#using wordpress
php-curl php-gd php-intl php-soap php-xml php-xmlrpc php-zip \
#wget for downloading 
wget vim \
#to compile project
clang make

COPY srcs/ /root/srcs/
COPY Makefile /root/
COPY configuration_file/ /root/configuration_file/

WORKDIR /root/
EXPOSE 8080