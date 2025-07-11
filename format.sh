find src/ -not -path "src/3rdparty/*" -not -path "src/agent/sslagent/behaviors/*" -not -path "src/agent/sslagent/compoundbehaviors/*" \( -iname '*.hh' -o -iname '*.cc' \) | xargs clang-format -i
