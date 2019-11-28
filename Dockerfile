FROM ubuntu:16.04

RUN apt-get update
RUN apt-get install -y software-properties-common
RUN add-apt-repository ppa:beineri/opt-qt562-xenial
RUN apt-get update -qq
RUN apt-get install -y xvfb libquazip5-headers libquazip5-1 libquazip5-dev qt5-default qttools5-dev-tools qt5-qmake
RUN apt-get install -y lcov
RUN apt-get install -y libquazip-headers

RUN apt-get install -qq qt56base
RUN update-alternatives --install /usr/bin/gcov gcov /usr/bin/gcov-5 90

COPY entrypoint /entrypoint
RUN sed -i 's/\r$//g' /entrypoint
RUN chmod +x /entrypoint

RUN mkdir -p /app
WORKDIR /app
COPY . /app

RUN sed -i 's/\r$//g' /app/coverage
RUN chmod +x /app/coverage

ENTRYPOINT ["/entrypoint"]
