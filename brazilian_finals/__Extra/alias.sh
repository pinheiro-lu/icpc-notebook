g() { g++ -Wconversion -fsanitize=undefined,address -g "$@"; }
export -f g
