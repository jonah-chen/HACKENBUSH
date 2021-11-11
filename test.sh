#!/bin/bash
for fp in tests/*.sh
do
    echo "Running $fp"
    bash $fp
done