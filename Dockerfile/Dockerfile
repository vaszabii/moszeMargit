FROM ubuntu:20.04
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install --no-install-recommends -y \
  make \
   doxygen \
   graphviz \
   && apt-get install -y git \
   g++ \
   g++-10 \
   texlive-latex-recommended \
   texlive-pictures \
   valgrind \
   texlive-latex-extra \
   cppcheck \
   libgtest-dev \
   cmake \   
&& rm -rf /var/lib/apt/lists/*