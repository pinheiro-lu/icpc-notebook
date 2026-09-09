# In ~/.bashrc (or terminal):
g() { g++ -Wconversion -fsanitize=undefined,address -g "$@"; }
export -f g
