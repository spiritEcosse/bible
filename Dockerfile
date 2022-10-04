FROM coderus/sailfishos-platform-sdk-aarch64:4.4.0.64

RUN sudo zypper -n install python3-pip openssl curl pigz openssh \
    git sqlite-devel qt5-qttest perl-IO-Compress perl-Module-Load-Conditional

RUN sudo pip install awscli
