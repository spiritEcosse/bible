FROM coderus/sailfishos-platform-sdk:4.3.0.12

RUN zypper install cmake
WORKDIR /app
COPY . .
RUN cmake --build . --target all
