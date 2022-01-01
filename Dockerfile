FROM coderus/sailfishos-platform-sdk:4.3.0.12

USER root
RUN zypper install -y cmake
WORKDIR /app
COPY . .
RUN cd /app && cmake --build . --target all

USER mersdk
