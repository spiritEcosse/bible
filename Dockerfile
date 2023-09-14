ARG ARCH
ARG RELEASE
FROM coderus/sailfishos-platform-sdk-${ARCH}:${RELEASE}

RUN sudo zypper -n install python3-pip openssl curl pigz openssh

RUN sudo pip install awscli
RUN mkdir -p ~/rpm/
COPY rpm/harbour-freebible.spec /home/mersdk/rpm/harbour-freebible.spec
RUN mb2 build-init
RUN mb2 build-requires
RUN curl https://raw.githubusercontent.com/spiritEcosse/aws-sailfish-sdk/master/install.sh | bash -s -- --func=install_sshpass
