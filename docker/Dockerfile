# Download base image ubuntu 18.04
FROM ubuntu:18.04

MAINTAINER Andreu Servera andreumarc12@gmail.com

#LABEL about the custom image
LABEL description="Development of a Centralized Network Controller and mechanisms for its Duplication over Time-Sensitive Networking (TSN) Ethernet Networks."

# Disable Prompt During Packages Installation
# ARG DEBIAN_FRONTEND=noninteractive

# Update Ubuntu Software repository and install dependencies.
RUN apt-get update && apt-get dist-upgrade -y && apt-get install -y \
	systemd \	
	cmake \	
	git \	
	libpcre3 \
	libpcre3-dev \
	zlib1g-dev \
	libssl-dev \
	build-essential \
	mysql-client-core-5.7 \
	default-libmysqlclient-dev \
	mysql-server \
	&& rm -rf /var/lib/apt/lists/*


# Upload the files onto the image
COPY ./libs /libs/


# Install libssh
RUN cd ./libs/libssh/ && \
	mkdir ./build && \
	cd build && \
	cmake .. && \
	make && \
	make install


# Install libyang
RUN	cd ./libs/libyang/ && \
	mkdir ./build && \
	cd build && \
	cmake .. && \
	make && \
	make install


# Install sysrepo
RUN	cd ./libs/sysrepo/ && \
	mkdir ./build && \
	cd build && \
	cmake .. && \
	make && \
	make install


# Install libnetconf2
RUN	cd ./libs/libnetconf2/ && \
	mkdir ./build && \
	cd build && \
	cmake .. && \
	make && \
	make install


# Install netopeer2. ldconfig enables make to find shared objects.
RUN	ldconfig && \
	cd ./libs/netopeer2/ && \
	mkdir ./build && \
	cd build && \
	cmake .. && \
	make && \
	make install


# Configure MySQL database
RUN /etc/init.d/mysql start && sleep 2 &&\
	mysql -uroot -e "CREATE DATABASE mydata;" && \
	mysql mydata < ./libs/mydata.sql


COPY ./docker-files /docker-files/
RUN ["chmod", "+x", "/docker-files/wrapper.sh"]
CMD ./docker-files/wrapper.sh
	


