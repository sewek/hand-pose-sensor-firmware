FROM --platform=linux/x86_64 ubuntu:22.04

ARG TOOLCHAIN_VERSION=v2.6.1
ARG SKD_NRF_BRANCH=v2.6.1

ENV DEBIAN_FRONTEND=noninteractive
ENV NRFUTIL_HOME=/usr/local/share/nrfutil

ENV LC_TIME=C.UTF-8 

# Update and setup environment
RUN apt-get update
RUN apt-get upgrade -y
RUN apt-get install -y wget clang-format gcc-multilib
RUN apt-get clean -y

# Install dependencies
WORKDIR /preparation

RUN wget https://developer.nordicsemi.com/.pc-tools/nrfutil/x64-linux/nrfutil -O nrfutil && \
    mv nrfutil /usr/local/bin/nrfutil && \
    chmod +x /usr/local/bin/nrfutil


RUN wget https://nsscprodmedia.blob.core.windows.net/prod/software-and-other-downloads/desktop-software/nrf-command-line-tools/sw/versions-10-x-x/10-24-2/nrf-command-line-tools_10.24.2_amd64.deb -O nrf-command-line-tools_10.24.2_amd64.deb && \
    apt-get install -y ./nrf-command-line-tools_10.24.2_amd64.deb

RUN nrfutil install toolchain-manager
RUN nrfutil toolchain-manager install --ncs-version $TOOLCHAIN_VERSION
RUN nrfutil toolchain-manager list

RUN rm -rf /preparation

# Set the working directory
WORKDIR /workdir

SHELL [ "nrfutil", "toolchain-manager", "launch", "/bin/bash", "--", "-c" ]

RUN west init -m https://github.com/nrfconnect/sdk-nrf --mr ${SKD_NRF_BRANCH} .
RUN west update
# --narrow -o=--depth=1

# Set the entrypoint
COPY entrypoint.sh entrypoint.sh
ENTRYPOINT [ "entrypoint.sh" ]
# ENTRYPOINT [ "nrfutil", "toolchain-manager", "launch", "/bin/bash", "--", "/workdir/entrypoint.sh"]
