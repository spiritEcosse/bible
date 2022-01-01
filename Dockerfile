FROM coderus/sailfishos-platform-sdk:4.3.0.12

WORKDIR /app
COPY . .
RUN cmake --build . --target all
