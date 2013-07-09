#!/bin/sh
user=para
set -e
exec su - $user -c 'tmuxinator archtrack'
