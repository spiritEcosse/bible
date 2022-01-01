FROM coderus/sailfishos-platform-sdk:4.3.0.12

USER root
RUN zypper install cmake
USER mersdk
WORKDIR /app
COPY . .
RUN cmake --build . --target all
