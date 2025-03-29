#!/bin/bash
# generate_docs.sh - Run from project root

# Configuration
PROJECT_ROOT=$(pwd)
DIAGRAM_SRC_DIR="$PROJECT_ROOT/docs/diagrams/src/class"
DIAGRAM_OUT_DIR="$PROJECT_ROOT/docs/diagrams/out"
FINAL_PDF="$PROJECT_ROOT/docs/all_diagrams.pdf"

# Ensure output directory exists
mkdir -p "$DIAGRAM_OUT_DIR"

# Check for PlantUML
if ! command -v plantuml &> /dev/null; then
    echo "Error: PlantUML not found. Please install it first."
    echo "On Ubuntu/Debian: sudo apt install plantuml"
    echo "On macOS: brew install plantuml"
    exit 1
fi

# Convert all .puml files to SVG
echo "Generating SVG diagrams..."
cd "$DIAGRAM_SRC_DIR" || exit
for puml_file in *.puml; do
    plantuml -tsvg -o "$DIAGRAM_OUT_DIR" "$puml_file"
done
cd "$PROJECT_ROOT" || exit

# Check if any SVGs were generated
if [ -z "$(ls -A "$DIAGRAM_OUT_DIR"/*.svg 2>/dev/null)" ]; then
    echo "Error: No SVG files were generated in $DIAGRAM_OUT_DIR"
    exit 1
fi

# Convert to PDF and merge
if command -v inkscape &> /dev/null; then
    echo "Converting SVGs to PDFs using Inkscape..."
    for svg_file in "$DIAGRAM_OUT_DIR"/*.svg; do
        pdf_file="${svg_file%.svg}.pdf"
        inkscape --export-type=pdf --export-filename="$pdf_file" "$svg_file"
    done
    
    echo "Merging PDFs..."
    if command -v pdfunite &> /dev/null; then
        pdfunite "$DIAGRAM_OUT_DIR"/*.pdf "$FINAL_PDF"
    elif command -v gs &> /dev/null; then
        gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -sOutputFile="$FINAL_PDF" "$DIAGRAM_OUT_DIR"/*.pdf
    else
        echo "PDF merging tools not found. Individual PDFs are in $DIAGRAM_OUT_DIR"
        exit 1
    fi
else
    echo "Inkscape not found. Please install it for best results."
    echo "SVG files are available in $DIAGRAM_OUT_DIR"
    exit 1
fi

echo "Successfully generated $FINAL_PDF"