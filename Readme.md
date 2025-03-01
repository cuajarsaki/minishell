to test run: 

docker build -t minishell .

docker run -it --rm -v "$(pwd)":/app -w /app minishell /bin/bash
