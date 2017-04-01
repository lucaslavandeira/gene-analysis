#!/usr/bin/env bash
make -f
chmod +x run.sh free_port.sh;./run.sh $(./free_port.sh 10001) valgrind
