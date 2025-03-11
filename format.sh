find src/ -not -path "src/3rdparty/*" \( -iname '*.hh' -o -iname '*.cc' \) | xargs clang-format -i
