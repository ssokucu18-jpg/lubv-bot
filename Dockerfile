FROM alpine:latest

RUN apk add --no-cache wine

WORKDIR /app

COPY lubv.exe /app/lubv.exe

CMD ["wine", "/app/lubv.exe"]