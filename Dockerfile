# Use Ubuntu as the base image
FROM ubuntu:latest

# Set non-interactive mode to avoid prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install required dependencies
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

# Default command: Open a Bash shell
CMD ["/bin/bash"]
