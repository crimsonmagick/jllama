FROM debian:sid-slim

RUN apt-get update && apt-get install -y \
    build-essential \
    curl \
    clang \
    git \
    libssl-dev \
    zlib1g-dev \
    mingw-w64 \
    openjdk-17-jdk \
    unzip \
    wget

WORKDIR /usr/src/workspace

# install a version of cmake that fully supports `find_package(JNI)`
ARG CMAKE_VERSION=3.28.0
ARG MAJOR_MINOR_VERSION=3.28

RUN wget https://cmake.org/files/v${MAJOR_MINOR_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.sh \
    && chmod +x cmake-${CMAKE_VERSION}-linux-x86_64.sh \
    && ./cmake-${CMAKE_VERSION}-linux-x86_64.sh --skip-license --prefix=/usr/local \
    && rm cmake-${CMAKE_VERSION}-linux-x86_64.sh

ARG JDK_ARCHIVE_DIR=jdk-17.0.9+9

# install Windows JDK
ARG JDK_WIN_URL=https://github.com/adoptium/temurin17-binaries/releases/download/jdk-17.0.9%2B9.1/OpenJDK17U-jdk_x64_windows_hotspot_17.0.9_9.zip
ARG JDK_WIN_ARCHIVE_NAME=jdk-windows.zip
ARG JDK_WIN_LOCATION=/usr/local/include/jdk-17-win

RUN wget -O ${JDK_WIN_ARCHIVE_NAME} "${JDK_WIN_URL}" \
    && mkdir -p ${JDK_WIN_LOCATION} \
    && unzip ${JDK_WIN_ARCHIVE_NAME} \
    && mv ${JDK_ARCHIVE_DIR}/* ${JDK_WIN_LOCATION} \
    && rm -dr ${JDK_ARCHIVE_DIR} \
    && rm ${JDK_WIN_ARCHIVE_NAME}

ENV JDK_WIN_LOCATION=${JDK_WIN_LOCATION}

# install Mac JDK
ARG JDK_MAC_URL=https://github.com/adoptium/temurin17-binaries/releases/download/jdk-17.0.9%2B9/OpenJDK17U-jdk_aarch64_mac_hotspot_17.0.9_9.tar.gz
ARG JDK_MAC_ARCHIVE_NAME=jdk-mac.tar.gz
ARG JDK_MAC_LOCATION=/usr/local/include/jdk-17-mac

RUN wget -O ${JDK_MAC_ARCHIVE_NAME} "${JDK_MAC_URL}" \
    && mkdir -p ${JDK_MAC_LOCATION} \
    && tar -xzf ${JDK_MAC_ARCHIVE_NAME} \
    && mv ${JDK_ARCHIVE_DIR}/* ${JDK_MAC_LOCATION} \
    && rm -dr ${JDK_ARCHIVE_DIR} \
    && rm ${JDK_MAC_ARCHIVE_NAME}

ENV JDK_MAC_LOCATION=${JDK_MAC_LOCATION}

# install MacOSX SDK
ENV UNATTENDED=1
ENV OSX_VERSION_MIN=12.x

ARG OSXCROSS_REPO=https://github.com/tpoechtrager/osxcross
ARG OSXCROSS_BRANCH=master
ARG OSXCROSS_SRC=osxcross
ARG OSXCROSS_TARGET=/usr/local/osxcross
ARG OSX_SDK_URL=https://github.com/joseluisq/macosx-sdks/releases/download/12.0/MacOSX12.0.sdk.tar.xz

RUN git clone -b ${OSXCROSS_BRANCH} ${OSXCROSS_REPO} ${OSXCROSS_SRC} \
    && wget -P ${OSXCROSS_SRC}/tarballs/ ${OSX_SDK_URL} \
    && chmod +x ./${OSXCROSS_SRC}/build.sh \
    && TARGET_DIR=${OSXCROSS_TARGET} ./${OSXCROSS_SRC}/build.sh \
    && rm -dr ${OSXCROSS_SRC}

COPY release_jllama.sh /usr/src/workspace
RUN chmod +x /usr/src/workspace/release_jllama.sh
ENTRYPOINT ["/bin/bash", "/usr/src/workspace/release_jllama.sh"]
