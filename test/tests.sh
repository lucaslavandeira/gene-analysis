#!/usr/bin/env bash
chmod +x run.sh free_port.sh;./run.sh $(./free_port.sh 10001) valgrind
