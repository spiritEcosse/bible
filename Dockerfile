FROM rabits/qt:5.13-desktop

RUN sudo apt-get update -qq && sudo apt-get install -y xvfb libquazip5-headers libquazip5-1 libquazip5-dev qt5-default qttools5-dev-tools qt5-qmake lcov

USER root 

RUN git clone https://github.com/google/googletest.git /googletest && \
	cd /googletest/ && \
	mkdir -p build && \
	cd build && \
	cmake .. && \
	make install

COPY . /app
RUN mkdir -p /app/tests/build
WORKDIR /app/tests/build
