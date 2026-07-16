FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    wine64 \
    wine32 \
    winetricks \
    gnutls-bin \
    libgnutls30 \
    libgnutls-dev \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY lubv.exe /app/lubv.exe

CMD ["wine64", "/app/lubv.exe"]