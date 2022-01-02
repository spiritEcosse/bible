FROM coderus/sailfishos-platform-sdk:4.3.0.12

USER root
RUN zypper install -y cmake ninja gcc gcc-c++ qt5-qttest
WORKDIR /app
COPY . .
RUN cmake -G Ninja . && cmake --build . --target 3rdparty

USER mersdk
