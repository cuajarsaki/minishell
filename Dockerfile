# Use Ubuntu as the base image
FROM ubuntu:latest

# Set non-interactive mode to avoid prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install required dependencies, including readline
RUN apt update && apt install -y \
    gcc \
    make \
    gdb \
    valgrind \
    vim \
    git \
    libreadline-dev \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container
WORKDIR /app

# Copy your shell source code into the container
COPY . /app

# Compile the shell
RUN make

# Set the default command to open a bash shell
CMD ["/bin/bash"]
