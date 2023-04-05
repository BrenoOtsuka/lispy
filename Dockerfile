FROM alpine:3.14

RUN apk add build-base libedit-dev

WORKDIR /usr/lispy

COPY src /usr/lispy/src
COPY include /usr/lispy/include

RUN gcc -std=c99 -Wall src/**.c src/lispy/*.c -ledit -I include/ -o lispy

CMD ["./lispy"]
