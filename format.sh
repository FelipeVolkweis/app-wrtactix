find src/ -not -path "src/3rdparty/*" -not -path "src/agent/sslagent/behaviors/*" \( -iname '*.hh' -o -iname '*.cc' \) | xargs clang-format -i
