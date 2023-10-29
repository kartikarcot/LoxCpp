# Use an Ubuntu base image
FROM ubuntu:latest

# Install Python 3.8 and other dependencies
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    sudo \
    clang-12 \
    ccls \
    gdb \
    make \
    git \
    coreutils \
    bash \
    cmake

# Install Flask for Python 3.8
RUN pip3 install Flask

# Copy source code into container
COPY ./ /app/

# Run build.sh to build the C++ binaries
WORKDIR /app
RUN chmod +x build.sh
RUN mkdir -p /app/build && \
    cd /app/build && \
    cmake .. && \
    make

# # Command to run server.py
WORKDIR /app/build
CMD ["make", "start_server"]
