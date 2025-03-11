OUR_PROTO_DIR="./"
OUR_OUT_DIR="./pb"

mkdir -p "$OUR_OUT_DIR"

if [ ! -d "$OUR_PROTO_DIR" ]; then
    echo "Error: Directory $OUR_PROTO_DIR does not exist."
    exit 1
fi

# Find all .proto files in the specified directory and compile them
for proto_file in "$OUR_PROTO_DIR"/*.proto; do
    if [ -f "$proto_file" ]; then
        protoc --cpp_out="$OUR_OUT_DIR" --grpc_out="$OUR_OUT_DIR" --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` --proto_path="$OUR_PROTO_DIR" "$proto_file"
    else
        echo "Warning: No .proto files found in $OUR_PROTO_DIR."
        exit 1
    fi
done