#!/bin/bash

set -e

# Run the command
nrfutil toolchain-manager launch /bin/bash -- -c "$*"