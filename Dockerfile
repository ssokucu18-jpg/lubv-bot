FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    wine64 \
    wine \
    winetricks \
    gnutls-bin \
    libgnutls30 \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY lubv.exe /app/lubv.exe

CMD ["wine64", "/app/lubv.exe"]