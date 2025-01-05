# Use an Ubuntu base image
FROM ubuntu:latest

# Install Python 3.8 and other dependencies
RUN apt-get update && apt-get install -y \
    python3 \
    python3-pip \
    python3.12-venv \
    sudo \
    clang \
    ccls \
    gdb \
    make \
    git \
    coreutils \
    bash \
    cmake

# Create and activate virtual environment
RUN python3 -m venv /opt/venv
ENV PATH="/opt/venv/bin:$PATH"

# Install Flask in the virtual environment
RUN pip install Flask

# Copy source code into container
COPY ./ /app/

# Run build.sh to build the C++ binaries
WORKDIR /app
RUN chmod +x build.sh
RUN mkdir -p /app/build && \
    cd /app/build && \
    cmake .. && \
    make

# Command to run server.py
WORKDIR /app/build
CMD ["make", "start_server"]