ARG ARCH
ARG RELEASE
FROM coderus/sailfishos-platform-sdk-${ARCH}:${RELEASE}

RUN sudo zypper -n install python3-pip openssl curl pigz openssh

RUN sudo pip install awscli
RUN mkdir -p ~/rpm/
COPY rpm/bible.spec /home/mersdk/rpm/bible.spec
RUN mb2 build-init
RUN mb2 build-requires
RUN curl https://raw.githubusercontent.com/spiritEcosse/aws-sailfish-sdk/master/install.sh | bash -s -- --func=install_sshpass
