#!/bin/bash

VENV_DIR="venv"

if [ ! -d "$VENV_DIR" ]; then
	echo "Creating virtual environment..."
	python3 -m venv "$VENV_DIR"
fi

echo "Activating virtual environment..."
source "$VENV_DIR/bin/activate"

echo "Installing required packages..."
pip install --upgrade pip
pip install pyserial

echo "Installed versions:"
python --version
pip show pyserial | grep -E 'Name|Version'

echo "You are now in the virtual environment."
echo "Running arduino_to_csv.py"
python arduino_data_collection.py


