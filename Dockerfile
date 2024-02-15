FROM ubuntu:22.04
RUN apt update
RUN apt -y install git build-essential perl cmake autoconf libtool zlib1g-dev
RUN mkdir -p /home/labsec/

ENV WORKSPACE=/home/labsec/quantumsafe
ENV BUILD_DIR=$WORKSPACE/build
RUN mkdir -p $BUILD_DIR/lib64
RUN ln -s $BUILD_DIR/lib64 $BUILD_DIR/lib

WORKDIR $WORKSPACE

RUN git clone https://github.com/openssl/openssl.git
WORKDIR $WORKSPACE/openssl

RUN ./Configure --prefix=$BUILD_DIR no-ssl no-tls1 no-tls1_1 no-afalgeng no-shared threads -lm

RUN make -j $(nproc)
RUN make -j $(nproc) install_sw install_ssldirs

WORKDIR $WORKSPACE

RUN git clone https://github.com/open-quantum-safe/liboqs.git
WORKDIR $WORKSPACE/liboqs

RUN mkdir build

WORKDIR $WORKSPACE/liboqs/build

RUN cmake \
  -DCMAKE_INSTALL_PREFIX=$BUILD_DIR \
  -DBUILD_SHARED_LIBS=ON \
  -DOQS_USE_OPENSSL=OFF \
  -DCMAKE_BUILD_TYPE=Release \
  -DOQS_BUILD_ONLY_LIB=OFF \
  -DOQS_DIST_BUILD=ON ..

RUN make -j $(nproc)
RUN make -j $(nproc) install

WORKDIR $WORKSPACE

RUN git clone https://github.com/open-quantum-safe/oqs-provider.git
WORKDIR $WORKSPACE/oqs-provider

RUN liboqs_DIR=$BUILD_DIR cmake \
  -DCMAKE_INSTALL_PREFIX=$WORKSPACE/oqs-provider \
  -DOPENSSL_ROOT_DIR=$BUILD_DIR \
  -DCMAKE_BUILD_TYPE=Release \
  -S . \
  -B _build

RUN cmake --build _build

# Manually copy the lib files into the build dir
RUN cp _build/lib/* $BUILD_DIR/lib/

# We need to edit the openssl config to use the oqsprovider
RUN sed -i "s/default = default_sect/default = default_sect\noqsprovider = oqsprovider_sect/g" $BUILD_DIR/ssl/openssl.cnf
RUN sed -i "s/\[default_sect\]/\[default_sect\]\nactivate = 1\n\[oqsprovider_sect\]\nactivate = 1\n/g" $BUILD_DIR/ssl/openssl.cnf

# These env vars need to be set for the oqsprovider to be used when using OpenSSL
ENV OPENSSL_CONF=$BUILD_DIR/ssl/openssl.cnf
ENV OPENSSL_MODULES=$BUILD_DIR/lib
RUN $BUILD_DIR/bin/openssl list -providers -verbose -provider oqsprovider

# Installing libcryptosec



WORKDIR /home/labsec/

RUN mkdir certificate_build

COPY . ./certificate_build

WORKDIR /home/labsec/certificate_build

